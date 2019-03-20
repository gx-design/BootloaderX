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
#include "Application.h"
#include "Dispatcher.h"
#include "IAdaptor.h"
#include "IBootloaderService.h"
#include "IUsbHidDevice.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class IBootloaderBoard : public ISystemTime
{
#pragma mark Public Members
  public:
    virtual void Initialise () = 0;

    IUsbHidDevice* HidDevice;

    DispatcherActions PlatformDispatcherActions;

    IBootloaderService* BootloaderService;

    virtual bool ForceBootloadRequested () = 0;

    virtual void PostInitialise ()
    {
    }

#pragma mark Private Members
  private:
};
