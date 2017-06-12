/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "BootloaderSettings.h"
#include "Dispatcher.h"
#include "GxBootloaderHidDevice.h"
#include "IDPStack.h"

#pragma mark Definitions and Constants
static const uint16_t vendorId = 0xFC01;
static const uint16_t productId = 0x0001;

#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloaderHidDevice::GxBootloaderHidDevice (IUsbHidDevice& hidDevice)
    : GxInstrumentationHidDevice (hidDevice, vendorId, productId), router (*new IDPRouter ()),
      stack (IDPStack::CreateIDPStack (1, NULL, router))
{

    DataReceived.Subscribe<GxBootloaderHidDevice, &GxBootloaderHidDevice::OnDataReceived> (this);
}

GxBootloaderHidDevice::~GxBootloaderHidDevice ()
{
}

IDPCommandManager& GxBootloaderHidDevice::GetCommandManager ()
{
    return stack.GetCommandManager ();
}

void GxBootloaderHidDevice::OnDataReceived (void* sender, EventArgs& e)
{
    Dispatcher::Invoke ([&] { ProcessDataReceived (); });
}

void GxBootloaderHidDevice::ProcessDataReceived ()
{
    while (HasBytes ())
    {
        stack.GetInterface ().ProcessByte (ReceiveData ());
    }
}
