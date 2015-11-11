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


#pragma mark Private Members
private:

};

#endif
