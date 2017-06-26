using Gx;
using Gx.Transactions;
using GX.Utils;
using HidLibrary;
using IntegratedDebugProtocol;
using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive;
using System.Reactive.Linq;
using System.Reactive.Subjects;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GreenLightApplicationDevice
{
    public class BagSealer : IBootloaderApplicationDevice
    {
        private const UInt16 vid = 0xFC02;
        private const UInt16 pid = 0x02;

        private bool isDetected = false;

        private HidDevice configurationDevice;
        private HidDevice inputDevice;
        private JobRunner configurationCommsThread;
        private JobRunner inputDeviceCommsThread;
        private CancellationTokenSource cancellationSource;
        private IdpInterface configurationIdpInterface;
        private IdpInterface inputIdpInterface;
        private IObservable<IdpPacket> packetStream;
        private IObservable<IdpPacket> statusPacket;
        private ReplaySubject<bool> isConnected;
        private CommandManager commandManager;

        public IObservable<bool> CableAttached { get; private set; }
        public IObservable<bool> IsConnected { get; private set; }
        public IObservable<bool> IsAttached { get; private set; }

        public BagSealer()
        {
            isConnected = new ReplaySubject<bool>(1);

            IsConnected = isConnected;
            IsAttached = isConnected;

            isConnected.OnNext(false);
        }

        public void Disconnect()
        {
            cancellationSource?.Cancel();
        }

        public bool Detect()
        {
            bool result = true;

            var devices = HidDevices.Enumerate(vid, pid);

            if (devices.Count() == 0)
            {
                result = false;
            }

            if(result != isDetected)
            {
                isDetected = result;
                
                if(result)
                {
                    Initialise();
                }

                isConnected.OnNext(result);
            }

            return result;
        }

        private bool Initialise()
        {
            bool result = false;
            
            var devices = HidDevices.Enumerate(vid, pid);

            if (devices.Count() == 0)
            {
                configurationDevice = null;
                inputDevice = null;
                result = false;
                isDetected = false;
            }
            else
            {
                isDetected = true;
                configurationDevice = devices.FirstOrDefault(x => x.DevicePath.Contains("&col01"));
                inputDevice = devices.FirstOrDefault(x => x.DevicePath.Contains("&col02"));

                if (configurationDevice != null && inputDevice != null)
                {
                    var attached = Observable.FromEvent<InsertedEventHandler, Unit>(h => () => h(Unit.Default), h => configurationDevice.Inserted += h, h => configurationDevice.Inserted -= h);
                    var detached = Observable.FromEvent<RemovedEventHandler, Unit>(h => () => h(Unit.Default), h => configurationDevice.Removed += h, h => configurationDevice.Removed -= h);

                    CableAttached = attached.Select(s => true).Merge(detached.Select(s => false));

                    CableAttached.Subscribe(async connected =>
                    {
                        if (connected)
                        {
                            await Connect();
                        }
                        else
                        {
                            cancellationSource.Cancel();
                            inputDevice.CloseDevice();
                            configurationDevice.CloseDevice();
                            isConnected.OnNext(false);
                        }
                    });

                    configurationIdpInterface = new IdpInterface();

                    inputIdpInterface = new IdpInterface();

                    commandManager = new CommandManager();

                    commandManager.RegisterInterface(configurationIdpInterface);
                    commandManager.RegisterInterface(inputIdpInterface);

                    InitialiseCommands();

                    packetStream = inputIdpInterface.PacketParsed.Merge(configurationIdpInterface.PacketParsed);

                    Task.Run(() => Connect()).Wait();

                    result = true;
                }
            }

            return result;
        }

        private void InitialiseCommands()
        {
            commandManager.RegisterCommand<EnterBootloaderRequestTransaction>();
            commandManager.RegisterCommand<EnterBootloaderResponseTransaction>();

            commandManager.RegisterCommand<GetVersionRequestTransaction>();
            commandManager.RegisterCommand<GetVersionResponseTransaction>();
        }

        public async Task<bool> Connect()
        {
            if (configurationDevice != null)
            {
                configurationDevice.OpenDevice();

                await StartComms();

                isConnected.OnNext(true);

                return true;
            }

            return false;
        }

        public async Task EnterBootloader()
        {
            await Transaction<EnterBootloaderRequestTransaction, EnterBootloaderResponseTransaction>(_ => { }, _ => { }, 250);
        }

        private async Task Transaction<TRequest, TResponse>(Action<TRequest> setValues, Action<TResponse> getValues, int timeout = 400) where TRequest : Transaction where TResponse : Transaction
        {
            var command = commandManager.GetCommand<TRequest>();
            var payload = command.CreatePayload();

            setValues(payload);

            var packet = command.Encode(1, 1, payload);

            var responsePacket = await SendPacketTaskAsync<TResponse>(packet, timeout);

            if (responsePacket != null)
            {
                getValues(responsePacket);
            }
        }

        private async Task<T> SendPacketTaskAsync<T>(IdpPacket packet, int timeout) where T : Transaction
        {
            T result = null;
            List<int> x = new List<int>();

            var command = commandManager.GetCommand<T>();
            var packetReceivedTcs = new TaskCompletionSource<T>();

            EventHandler<T> commandReceivedHandler = (sender, e) =>
            {
                if (!packetReceivedTcs.Task.IsCompleted)
                {
                    packetReceivedTcs.SetResult(e);
                }
            };

            command.CommandReceived += commandReceivedHandler;

            configurationCommsThread.InvokeAsync(() =>
            {
                packet.Insert(0, (byte)packet.Count);
                packet.Insert(0, 0x01);
                configurationDevice.WriteFeatureData(packet.ToArray());
            });

            if (await Task.WhenAny(packetReceivedTcs.Task, Task.Delay(timeout)) == packetReceivedTcs.Task)
            {
                result = packetReceivedTcs.Task.Result;
            }
            else
            {
                result = null;
            }

            command.CommandReceived -= commandReceivedHandler;

            return result;
        }

        private async Task StartComms()
        {
            cancellationSource?.Cancel();
            cancellationSource = new CancellationTokenSource();

            TaskCompletionSource<JobRunner> inputDeviceThreadStarted = new TaskCompletionSource<JobRunner>();
            TaskCompletionSource<JobRunner> configurationThreadStarted = new TaskCompletionSource<JobRunner>();

#pragma warning disable CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed
            Task.Factory.StartNew(() =>
            {
                configurationCommsThread = new JobRunner();

                configurationThreadStarted.SetResult(configurationCommsThread);

                configurationCommsThread.RunLoop(cancellationSource.Token);
            });
#pragma warning restore CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed

            await configurationThreadStarted.Task;

            configurationCommsThread.InvokeAsync(ReceiveFeatureReport);
        }

        private void ReceiveFeatureReport()
        {
            byte[] data;
            
            configurationDevice.ReadFeatureData(out data, 1);

            if (data != null && data.Length > 0)
            {
                for (int i = 2; i - 2 < data[1]; i++)
                {
                    configurationIdpInterface.ProcessByte(data[i]);
                }
            }
            else
            {
                Thread.Sleep(10);
            }

            configurationCommsThread.InvokeTaskAsync(ReceiveFeatureReport);
        }

        public async Task<VersionInformation> GetVersion()
        {
            VersionInformation result = null;

            await Transaction<GetVersionRequestTransaction, GetVersionResponseTransaction>(req => 
            {
            }, 
            resp => 
            {
                result = new VersionInformation
                {
                    ApplicationVersion = resp.Version,
                    BootloaderVersion = resp.BootloaderVersion
                };
            });

            return result;
        }
    }
}
