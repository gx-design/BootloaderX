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

class GxBootloaderHidDevice : public GxInstrumentationHidDevice
{
#pragma mark Public Members
  public:
    GxBootloaderHidDevice (IUsbHidDevice& hidDevice);
                           
    ~GxBootloaderHidDevice ();


#pragma mark Private Members
private:

};

#endif
