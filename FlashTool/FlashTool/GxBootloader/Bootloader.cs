using Gx;
using Gx.Transactions;
using GX.Utils;
using HidLibrary;
using IntegratedDebugProtocol;
using IntegratedDebugProtocol.Transactions;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reactive;
using System.Reactive.Linq;
using System.Reactive.Subjects;
using System.Threading;
using System.Threading.Tasks;

namespace GX
{
    public class GxBootloader
    {
        private const UInt16 vid = 0xFC02;
        private const UInt16 pid = 0x01;
        private bool isDetected = false;

        private HidDevice configurationDevice;
        private JobRunner configurationCommsThread;
        private JobRunner flashDataThread;
        private CancellationTokenSource cancellationSource;
        private IdpInterface configurationIdpInterface;
        private ReplaySubject<bool> isConnected;
        private CommandManager commandManager;
        private Subject<FlashStatus> flashStatus;

        public IObservable<bool> CableAttached { get; private set; }
        public IObservable<bool> IsConnected { get; private set; }
        public IObservable<bool> IsAttached { get; private set; }

        private const uint BlockSize = 28; 

        public GxBootloader()
        {
            isConnected = new ReplaySubject<bool>(1);
            flashStatus = new Subject<FlashStatus>();

            IsConnected = isConnected;
            IsAttached = isConnected;

            isConnected.OnNext(false);
        }

        public bool Detect()
        {
            bool result = true;

            var devices = HidDevices.Enumerate(vid, pid);

            if (devices.Count() == 0)
            {
                result = false;
            }

            if (result != isDetected)
            {
                isDetected = result;

                if (result)
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
                result = false;
                isDetected = false;
            }
            else
            {
                isDetected = true;
                configurationDevice = devices.FirstOrDefault();

                var attached = Observable.FromEvent<InsertedEventHandler, Unit>(h => () => h(Unit.Default), h => configurationDevice.Inserted += h, h => configurationDevice.Inserted -= h);
                var detached = Observable.FromEvent<RemovedEventHandler, Unit>(h => () => h(Unit.Default), h => configurationDevice.Removed += h, h => configurationDevice.Removed -= h);

                CableAttached = attached.Select(s => true).Merge(detached.Select(s => false));

                configurationIdpInterface = new IdpInterface();

                commandManager = new CommandManager();

                commandManager.RegisterInterface(configurationIdpInterface);

                if (configurationDevice != null)
                {
                    configurationDevice.MonitorDeviceEvents = true;
                }

                InitialiseCommands();

                Task.Run(()=>Connect()).Wait();

                result = true;
            }

            return result;
        }

        public async Task BeginFlashOperation()
        {
            await Transaction<BeginFlashOperationRequestTransaction, BeginFlashOperationResponseTransaction>(_ => { }, _ => { }, 10000);
        }

        object flashLock = new object();

        public async Task FlashBinary(string fileName)
        {
            var data = File.ReadAllBytes(fileName);

            var lines = data.Length / BlockSize;

            if (data.Length % BlockSize != 0)
            {
                lines++;
            }

            uint totalWritten = 0;

            await flashDataThread.InvokeTaskAsync(() =>
            {
                for (UInt32 line = 0; line < lines; line++)
                {
                    UInt32 offset = line * BlockSize;

                    UInt32 bytesToWrite = BlockSize;

                    if (offset + BlockSize > data.Length)
                    {
                        bytesToWrite = (uint)data.Length - offset;
                    }

                    byte[] currentLine = new byte[bytesToWrite];
                    Buffer.BlockCopy(data, (int)(line * BlockSize), currentLine, 0, (int)bytesToWrite);

                    flashStatus.OnNext(new FlashStatus() { TotalBytes = (uint)data.Length, BytesFlashed = totalWritten });

                    lock (flashLock)
                    {
                        FlashData(currentLine);
                    }

                    totalWritten += (uint)currentLine.Length;
                }
            });

            await FinaliseImage();
        }

        public async Task<bool> FinaliseImage()
        {
            bool result = false;

            await Transaction<ValidateImageRequestTransaction, ValidateImageResponseTransaction>(req => { }, resp => { result = resp.Success; });

            return result;
        }

        public IObservable<FlashStatus> FlashStatus
        {
            get
            {
                return flashStatus;
            }
        }

        public bool FlashData(byte[] data)
        {
            bool result = false;

            Transaction<FlashDataRequestTransaction, FlashDataResponseTransaction>(req =>
            {
                req.Length = (byte)data.Length;
                req.Data = new List<byte>();

                foreach (byte b in data)
                {
                    req.Data.Add(b);
                }

            }, resp => { result = resp.Success; }, 100000).Wait();

            return result;
        }

        private void InitialiseCommands()
        {
            commandManager.RegisterCommand<BeginFlashOperationRequestTransaction>();
            commandManager.RegisterCommand<BeginFlashOperationResponseTransaction>();

            commandManager.RegisterCommand<FlashDataRequestTransaction>();
            commandManager.RegisterCommand<FlashDataResponseTransaction>();

            commandManager.RegisterCommand<ValidateImageRequestTransaction>();
            commandManager.RegisterCommand<ValidateImageResponseTransaction>();
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

        private void ReceiveFeatureReport()
        {
            byte[] data;
            configurationDevice.ReadFeatureData(out data, 1);

            if (data != null)
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

        public async Task<bool> Connect()
        {
            cancellationSource = new CancellationTokenSource();

            if (configurationDevice != null)
            {
                configurationDevice.OpenDevice();

                await StartComms();

                return true;
            }

            return false;
        }

        public async Task Disconnect()
        {

        }

        private async Task StartComms()
        {
            TaskCompletionSource<JobRunner> inputDeviceThreadStarted = new TaskCompletionSource<JobRunner>();
            TaskCompletionSource<JobRunner> configurationThreadStarted = new TaskCompletionSource<JobRunner>();

#pragma warning disable CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed
            Task.Factory.StartNew(() =>
            {
                configurationCommsThread = new JobRunner();

                configurationThreadStarted.SetResult(configurationCommsThread);

                configurationCommsThread.RunLoop(cancellationSource.Token);
            });

            new Thread(delegate () {
                flashDataThread = new JobRunner();

                flashDataThread.RunLoop(cancellationSource.Token);
            }).Start();
#pragma warning restore CS4014 // Because this call is not awaited, execution of the current method continues before the call is completed

            await configurationThreadStarted.Task;

            configurationCommsThread.InvokeAsync(ReceiveFeatureReport);
        }
    }
}
