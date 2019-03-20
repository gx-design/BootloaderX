/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 10 November 2015
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "BootloaderXHidDevice.h"
#include "BootloaderSettings.h"
#include "Dispatcher.h"
//#include "IDPStack.h"

#pragma mark Definitions and Constants

#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloaderHidDevice::GxBootloaderHidDevice (IUsbHidDevice& hidDevice,
                                              uint16_t vid, uint16_t pid,
                                              const char* manufacturerString,
                                              const char* productString,
                                              const char* serialString)
    : GxInstrumentationHidDevice (hidDevice, vid, pid, manufacturerString,
                                  productString, serialString),
      parser (*new IdpPacketParser ())
{
    parser.Stream (*this);

    parser.Start ();

    parser.DataReceived += [&](auto sender, auto& e) {
        auto& args = static_cast<DataReceivedEventArgs&> (e);
        IAdaptor::OnReceive (args.Packet);
    };
}

GxBootloaderHidDevice::~GxBootloaderHidDevice ()
{
}

bool GxBootloaderHidDevice::Transmit (std::shared_ptr<IdpPacket> packet)
{
    if (IsConnected ())
    {
        ReportIdpPacket (*packet);
        return true;
    }

    return false;
}

bool GxBootloaderHidDevice::IsValid ()
{
    return IsConnected ();
}

int32_t GxBootloaderHidDevice::BytesReceived ()
{
    return receiveBuffer->Count ();
}

void GxBootloaderHidDevice::Close ()
{
    // TODO implement hid device disconnect.
}

int32_t GxBootloaderHidDevice::Read (void* buffer, uint32_t length)
{
    uint32_t bytesRead = 0;

    while (!receiveBuffer->IsEmpty () && length > 0)
    {
        static_cast<uint8_t*> (buffer)[bytesRead++] = ReceiveData ();
        length--;
    }

    return bytesRead;
}

int32_t GxBootloaderHidDevice::Write (const void* data, uint32_t length)
{
    SendData (static_cast<const uint8_t*> (data), length);
    return length;
}
