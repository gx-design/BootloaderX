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
                                  productString, serialString)
{
    DataReceived += [&](auto sender, auto& e) { OnDataReceived (sender, e); };
}

GxBootloaderHidDevice::~GxBootloaderHidDevice ()
{
}

void GxBootloaderHidDevice::OnDataReceived (void* sender, EventArgs& e)
{
    Dispatcher::Invoke ([&] { ProcessDataReceived (); });
}

void GxBootloaderHidDevice::ProcessDataReceived ()
{
}
