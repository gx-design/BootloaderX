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
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "GxBootloaderHidDevice.h"

class IBoard
{
#pragma mark Public Members
  public:
    IUsbHidDevice* hidDevice;
    DispatcherActions* dispatcherActions;

    virtual void EraseFirmware () = 0;
    virtual void FlashData (uint32_t address, uint64_t data) = 0;
    virtual void FlashData (uint32_t address, uint32_t data) = 0;
    virtual void FlashData (uint32_t address, uint16_t data) = 0;
    virtual void FlashData (uint32_t address, uint8_t data) = 0;

    virtual void JumpToApplication () = 0;


#pragma mark Private Members
  private:
};

#endif
