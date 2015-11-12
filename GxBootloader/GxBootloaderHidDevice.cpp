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


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloaderHidDevice::GxBootloaderHidDevice (IUsbHidDevice& hidDevice)
    : GxInstrumentationHidDevice (hidDevice)
{
    packetBuffer.elements = &packetData[0];
    packetBuffer.length = sizeof (packetData);

    packet.SetBuffer (&packetBuffer);

    DataReceived += [this](void* sender, EventArgs& e)
    {
        ProcessDataReceived (sender, e);
    };

    parser.PacketReceived += [this](void* sender, EventArgs& e)
    {
        ProcessPacketReceived (sender, e);
    };
}

GxBootloaderHidDevice::~GxBootloaderHidDevice ()
{
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

void GxBootloaderHidDevice::ProcessPacketReceived (void* sender, EventArgs& e)
{
    auto command = parser.receiveBuffer->Read ();

    packet.Reset ();
    packet.Add (command);

    switch (command)
    {
    case 0x80:
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
                packet.Add (0.02f);
                break;

            default:
                break;
            }

            packet.Finalise ();

            SendData (packet.GetPacket ());
        }
        break;
    }
}
