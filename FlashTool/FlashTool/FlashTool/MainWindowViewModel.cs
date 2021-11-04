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
using System.Net.Sockets;
using System.IO;
using IdpProtocol;
using System.Reactive;
using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;

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

        private bool _isConnected;

        private string _version;

        private string _status = "IDLE";
        private bool isFlashing;
        private PropertyHelper<double> progressProperty;
        private PropertyHelper<string> connectionStatusProperty;
        private PropertyHelper<BootloaderConnectionState> connectionStateProperty;

        private Client _bootloaderClient; 

        public ReactiveCommand<Unit, Unit> ConnectCommand { get; }

        public int i = 0;
        public MainWindowViewModel ()
        {
            _bootloaderClient = new Client();



            ConnectCommand = ReactiveCommand.CreateFromTask(async ()=> 
            {
                if (!_isConnected)
                {
                    Console.WriteLine("connecting");
                    _bootloaderClient.Connect(RxApp.MainThreadScheduler, "ilmd", 1239);

                    Console.WriteLine("waiting for  enum");
                    await _bootloaderClient.WaitForEnumeration();

                    Console.WriteLine("connecting interfaces");
                    await _bootloaderClient.ConnectInterfaces();

                    if (_bootloaderClient.BootloaderClientNode is { })
                    {
                        await _bootloaderClient.BootloaderClientNode.ConnectAsync();
                    }


                    _isConnected = true;
                } else
                {
                    _bootloaderClient.Dispose();
                    _isConnected = false;
                    _bootloaderClient = new Client();
                }             

                /* while(true)
                {
                    var version = await _bootloaderClient.BootloaderClientNode.GetVersion();

                    Version = version.ToString();
                    
                    ConnectionStatus = $"{i++}";

                    await Task.Delay(100);
                }*/

            });

            UpdateCommand = ReactiveCommand.CreateFromTask(async ()=> 
            {
                var dlg = new OpenFileDialog();
                dlg.Title = "Select Firmware Update File";

                dlg.Filters.Add(new FileDialogFilter
                {
                    Name = "Firmware Update File",
                    Extensions = new List<string>() { "firmware" }
                });

                dlg.InitialFileName = string.Empty;
                var result = await dlg.ShowAsync((Application.Current.ApplicationLifetime as IClassicDesktopStyleApplicationLifetime)?.MainWindow);

                if (result != null && result.Count() >= 1)
                {
                    IsFlashing = true;

                    //TODO reset into bootloader.
                    if (_bootloaderClient.BootloaderClientNode is null)
                    {
                        await _bootloaderClient.ServiceNode.StartBootloader();

                        await Task.Delay(5000);

                        await _bootloaderClient.ConnectBootloaderInterfaces();
                    }
                    //await RequestConnectionState(BootloaderConnectionState.Bootloader);

                    //TODO wait for bootloader to connect.

                    ConnectionStatus = "Erasing...";
                    
                    var erased = await _bootloaderClient.BootloaderClientNode.EraseFlash();
                    
                    if(erased) 
                    {
                        ConnectionStatus = "Erased";
                    } 
                    else
                    {
                        ConnectionStatus = "Failed";
                    }

                    var data = await File.ReadAllBytesAsync(result[0]);

                    var blocksize = 32;
                    var length = data.Length;
                    var blocks = length / blocksize;
                    var remain = length % blocksize;

                    for(int i = 0; i < blocks; i++) 
                    {
                        var offset = (i * blocksize);
                        var bytesToWrite = blocksize;

                        if(offset + blocksize > data.Length) 
                        {
                            bytesToWrite = (data.Length - offset);
                        }

                        var currentBlock = new byte[bytesToWrite];

                        Buffer.BlockCopy(data, i * blocksize, currentBlock, 0, bytesToWrite);

                        ConnectionStatus = $"Flashing block {i}";

                        await _bootloaderClient.BootloaderClientNode.WriteBlock(currentBlock);
                    }

                    await _bootloaderClient.BootloaderClientNode.Commit();

                    IsFlashing = false;

                }
            });
            
            
            /* progressProperty = _bootloaderClient.FlashStatus.Select(status =>
            {
                return ((double)status.BytesFlashed / (double)status.TotalBytes) * 100.0;
            }).ToObservableProperty(this, x => x.Progress);*/

           /* UpdateCommand = ReactiveCommand.Create(async () =>
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
            });*/
        }


        public ReactiveCommand<Unit, Unit> UpdateCommand { get; }
        
        public double Progress => 0;
        
        public string ConnectionStatus
        {
            get { return _status; }
            set { this.RaiseAndSetIfChanged(ref _status, value); }
        }

        public BootloaderConnectionState ConnectionState => BootloaderConnectionState.Bootloader;
        
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
