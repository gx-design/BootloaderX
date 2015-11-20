/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _GXBOOTLOADER_H_
#define _GXBOOTLOADER_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "IBoard.h"
#include "GxBootloaderHidDevice.h"

class GxBootloader
{
#pragma mark Public Members
  public:
    GxBootloader (IBoard& board);
    ~GxBootloader ();

    const float Version = 0.89;

    void Run (void);
    void Initialise (void);


#pragma mark Private Members
  private:
    IBoard& board;
    Dispatcher mainDispatcher;
    GxBootloaderHidDevice UsbInterface;
    uint32_t currentAddress;

    void InitialiseFlags ();
    void SetState (BootloaderState state);
};

#endif
