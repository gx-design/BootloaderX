using Avalonia.Controls;
using Gx;
using ReactiveUI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reactive.Linq;
using UiUtils.Reactive;
using GX;
using Avalonia.Threading;
using System.Reactive.Threading.Tasks;

namespace GxFlash
{
    enum BootloaderConnectionState
    {
        Disconnected,
        Application,
        Bootloader
    };

    class MainWindowViewModel : ReactiveObject
    {
        private IBootloaderApplicationDevice _device;

        private BootloaderConnectionState _currentConnectionState = BootloaderConnectionState.Disconnected;

        private IObservable<BootloaderConnectionState> _connectionState;


        private string _version;
        private bool isFlashing;
        private PropertyHelper<double> progressProperty;
        private PropertyHelper<string> connectionStatusProperty;
        private PropertyHelper<BootloaderConnectionState> connectionStateProperty;

        public MainWindowViewModel (GxBootloader bootloader, IBootloaderApplicationDevice applicationDevice)
        {
            _device = applicationDevice;

            _connectionState = applicationDevice.IsAttached.CombineLatest(bootloader.IsAttached, (a, b) =>
            {
                if (!a && !b)
                {
                    return BootloaderConnectionState.Disconnected;
                }
                else if (a && !b)
                {
                    return BootloaderConnectionState.Application;
                }
                else if (!a && b)
                {
                    return BootloaderConnectionState.Bootloader;
                }
                else
                {
                    return BootloaderConnectionState.Disconnected;
                }
            });

            this.connectionStateProperty = _connectionState.ObserveOnUi().ToObservableProperty(this, x => x.ConnectionState);

            var connectionState = _connectionState.Select(c =>
            {
                _currentConnectionState = c;

                switch (_currentConnectionState)
                {
                    default:
                    case BootloaderConnectionState.Disconnected:
                        return "Disconnected";

                    case BootloaderConnectionState.Application:
                        return "Connected (Application)";

                    case BootloaderConnectionState.Bootloader:
                        return "Connected (Bootloader)";
                }
            }).ObserveOnUi();

            this.connectionStatusProperty = connectionState.ToObservableProperty(this, x=>x.ConnectionStatus);

            _connectionState.Subscribe(async s =>
            {
                    switch (s)
                    {
                        case BootloaderConnectionState.Application:
                            {
                                var versionInfo = await applicationDevice.GetVersion();

                                Dispatcher.UIThread.InvokeAsync(() =>
                                {
                                    Version = $"BL: {versionInfo?.BootloaderVersion:F2}, FW: {versionInfo?.ApplicationVersion:F2}";
                                });
                            }
                            break;

                        case BootloaderConnectionState.Bootloader:
                            {
                                var versionInfo = await bootloader.GetVersion();

                                Dispatcher.UIThread.InvokeAsync(() =>
                                {
                                    Version = $"BL: {versionInfo?.BootloaderVersion:F2}";
                                });
                            }
                            break;

                        case BootloaderConnectionState.Disconnected:
                            Version = string.Empty;
                            break;
                    }
            });

            progressProperty = bootloader.FlashStatus.Select(status =>
            {
                return ((double)status.BytesFlashed / (double)status.TotalBytes) * 100.0;
            }).ToObservableProperty(this, x => x.Progress);

            UpdateCommand = ReactiveCommand.Create(this.WhenAny(x => x.ConnectionState, x=>x.IsFlashing, (state, flashing) => state.Value != BootloaderConnectionState.Disconnected && !flashing.Value));
            UpdateCommand.Subscribe(async _ =>
            {
                var dlg = new OpenFileDialog();
                dlg.Title = "Select Firmware Update File";

                dlg.Filters.Add(new FileDialogFilter
                {
                    Name = "Firmware Update File",
                    Extensions = new List<string>() { "firmware" }
                });

                dlg.InitialFileName = string.Empty;
                var result = await dlg.ShowAsync();

                if (result != null && result.Count() >= 1)
                {
                    IsFlashing = true;
                    await RequestConnectionState(BootloaderConnectionState.Bootloader);

                    bootloader.Detect();

                    await bootloader.BeginFlashOperation();

                    await bootloader.FlashBinary(result[0]);
                    IsFlashing = false;
                }
            });

            var connectionTimer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(50), IsEnabled = true };

            connectionTimer.Tick += (sender, e) =>
            {
                bootloader.Detect();
                applicationDevice.Detect();
            };
        }

        private async Task RequestConnectionState (BootloaderConnectionState requestedState)
        {
            if(_currentConnectionState == requestedState)
            {
                return;
            }

            switch(_currentConnectionState)
            {
                case BootloaderConnectionState.Application:
                    _device.Detect();
                    await _device.EnterBootloader();

                    await _connectionState.Where(s => s == requestedState).Take(1).ToTask();
                    break;

                    //TODO implement opposite transition.
            }
        }

        public ReactiveCommand<object> UpdateCommand { get; }
        
        public double Progress => progressProperty.Value;
        
        public string ConnectionStatus => connectionStatusProperty.Value;

        public BootloaderConnectionState ConnectionState => connectionStateProperty.Value;
        
        public bool IsFlashing
        {
            get { return isFlashing; }
            set { this.RaiseAndSetIfChanged(ref isFlashing, value); }
        }

        public string Version
        {
            get { return _version; }
            set { this.RaiseAndSetIfChanged(ref _version, value); }
        }
    }
}
