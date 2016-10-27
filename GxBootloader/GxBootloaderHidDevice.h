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
#include "Dispatcher.h"
#include "GxInstrumentationHidDevice.h"
#include "IDPStack.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


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
    GxBootloaderHidDevice (IUsbHidDevice& hidDevice);

    ~GxBootloaderHidDevice ();
    
    IDPCommandManager& GetCommandManager ();

#pragma mark Private Members
  private:
    void OnDataReceived (void* sender, EventArgs& e);
    void ProcessDataReceived ();
    void ProcessPacketReceived (void* sender, EventArgs& e);

    IDPRouter& router;
    IDPStack& stack;
};

#endif
