/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _GXBOOTLOADERHIDDEVICE_H_
#define _GXBOOTLOADERHIDDEVICE_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "GxInstrumentationHidDevice.h"
#include "Dispatcher.h"
#include "IDP.h"

using namespace VitalElement::Threading;

class FlashDataEventArgs : public EventArgs
{
  public:
    uint32_t length;
    uint8_t* data;
};

class GxBootloaderHidDevice : public GxInstrumentationHidDevice
{
#pragma mark Public Members
  public:
    GxBootloaderHidDevice (IUsbHidDevice& hidDevice,
                           Dispatcher& mainDispatcher);

    ~GxBootloaderHidDevice ();

    Event<EventArgs> EraseFirmwareRequested;
    Event<FlashDataEventArgs> FlashDataRequested;
    Event<EventArgs> FinaliseImageRequested;
    Event<EventArgs> BootloaderVersionRequested;
    Event<EventArgs> FirmwareVersionRequested;

    void SendBootloaderVersion (float version);
    void Acknowlege (uint8_t command);

#pragma mark Private Members
  private:
    void ProcessDataReceived (void* sender, EventArgs& e);
    void ProcessPacketReceived (void* sender, EventArgs& e);
    uint8_t packetData[128];
    uint8_t currentFlashData[128];
    Buffer packetBuffer;
    IDPPacket packet;
    IDP parser;
};

#endif
