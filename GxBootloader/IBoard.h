/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _IBOARD_H_
#define _IBOARD_H_

#pragma mark Includes
#include "GxBootloaderHidDevice.h"
#include "IBootloaderService.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class IBoard
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

#endif
