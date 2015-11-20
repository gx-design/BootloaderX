/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "GxBootloaderHidDevice.h"
#include "BootloaderSettings.h"

#pragma mark Definitions and Constants
static const uint16_t vendorId = 0xFC01;
static const uint16_t productId = 0x0001;

#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloaderHidDevice::GxBootloaderHidDevice (IUsbHidDevice& hidDevice,
                                              Dispatcher& mainDispatcher)
    : GxInstrumentationHidDevice (hidDevice, vendorId, productId)
{
    packetBuffer.elements = &packetData[0];
    packetBuffer.length = sizeof (packetData);

    packet.SetBuffer (&packetBuffer);

    DataReceived += [&](void* sender, EventArgs& e)
    {
        mainDispatcher.BeginInvoke ([&]
                                    {
                                        ProcessDataReceived (sender, e);
                                    });
    };

    parser.PacketReceived += [this](void* sender, EventArgs& e)
    {
        ProcessPacketReceived (sender, e);
    };
}

GxBootloaderHidDevice::~GxBootloaderHidDevice ()
{
}

void GxBootloaderHidDevice::Acknowlege (uint8_t command)
{
    packet.Reset ();
    packet.Add (command);
    packet.Add (true);
    packet.Finalise ();

    SendData (packet.GetPacket ());
}

void GxBootloaderHidDevice::ProcessDataReceived (void* sender, EventArgs& e)
{
    uint8_t index = 0;
    while (HasBytes () && index < 64)
    {
        parser.ProcessByte (ReceiveData ());
        index++;
    }
}

void GxBootloaderHidDevice::SendBootloaderVersion (float version)
{
    packet.Reset ();
    packet.Add ((uint8_t)80);
    packet.Add (version);
    packet.Finalise ();

    SendData (packet.GetPacket ());
}

void GxBootloaderHidDevice::ProcessPacketReceived (void* sender, EventArgs& e)
{
    auto command = parser.receiveBuffer->Read ();

    switch (command)
    {
        case 0x80: // variable commands
        {
            auto settingId = (SettingId)parser.ReadByte ();
            bool write = parser.ReadBool ();

            if (write)
            {
            }
            else
            {
                switch (settingId)
                {
                    case SettingId::BootloaderVersion:
                        if (BootloaderVersionRequested != nullptr)
                        {
                            EventArgs args;
                            BootloaderVersionRequested (this, args);
                        }
                        break;

                    default:
                        break;
                }
            }
        }
        break;

        case 0x81: // bootloader commands
        {
            auto flashCmd = parser.ReadByte ();

            switch (flashCmd)
            {
                case 0: // begin flash operation.
                {
                    if (EraseFirmwareRequested != nullptr)
                    {
                        EventArgs args;
                        EraseFirmwareRequested (this, args);
                    }
                }
                break;

                case 1:
                {
                    if (FlashDataRequested != nullptr)
                    {
                        FlashDataEventArgs args;
                        args.length = parser.ReadByte ();

                        for (uint32_t i = 0; i < args.length; i++)
                        {
                            currentFlashData[i] = parser.ReadByte ();
                        }

                        args.data = &currentFlashData[0];

                        FlashDataRequested (this, args);
                    }
                }
                break;

                case 2:
                {
                    if (FinaliseImageRequested != nullptr)
                    {
                        EventArgs args;

                        FinaliseImageRequested (this, args);
                    }
                }
                break;
            }
        }
        break;
    }
}
