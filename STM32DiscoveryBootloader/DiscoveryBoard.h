/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _DISCOVERYBOARD_H_
#define _DISCOVERYBOARD_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "IBoard.h"

class DiscoveryBoard : public IBoard
{
#pragma mark Public Members
  public:
    DiscoveryBoard ();
    ~DiscoveryBoard ();

    void EraseFirmware ();
    void FlashData (uint32_t address, uint64_t data);
    void FlashData (uint32_t address, uint32_t data);
    void FlashData (uint32_t address, uint16_t data);
    void FlashData (uint32_t address, uint8_t data);
    void JumpToApplication ();

#pragma mark Private Members
  private:
};

#endif
