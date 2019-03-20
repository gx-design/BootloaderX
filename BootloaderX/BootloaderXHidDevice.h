/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 10 November 2015
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas
#pragma once

#pragma mark Includes
#include "Dispatcher.h"
#include "GxInstrumentationHidDevice.h"
#include "IAdaptor.h"
#include "IStream.h"
#include "IdpPacketParser.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


class FlashDataEventArgs : public EventArgs
{
  public:
    uint32_t length;
    uint8_t* data;
};

class GxBootloaderHidDevice : public GxInstrumentationHidDevice,
                              public INotifyingStream,
                              public IAdaptor
{
#pragma mark Public Members
  public:
    GxBootloaderHidDevice (IUsbHidDevice& hidDevice, uint16_t vid, uint16_t pid,
                           const char* manufacturerString,
                           const char* productString, const char* serialString);

    ~GxBootloaderHidDevice ();

    bool Transmit (std::shared_ptr<IdpPacket> packet);

    bool IsValid ();
    int32_t BytesReceived ();

    void Close ();
    int32_t Read (void* buffer, uint32_t length);
    int32_t Write (const void* data, uint32_t length);

    const char* Name ()
    {
        return "Bootloader.USB";
    }

#pragma mark Private Members
  private:
    IdpPacketParser& parser;
};
