/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "GxBootloader.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloader::GxBootloader (IBoard& board) : board (board), UsbInterface(GxBootloaderHidDevice(*board.hidDevice))
{
}

GxBootloader::~GxBootloader ()
{
}

void GxBootloader::Run ()
{
    Initialise ();
    
    uint32_t i = 0;

    while (true)
    {
        i++;
    }
}

void GxBootloader::Initialise ()
{
    board.hidDevice->InitialiseStack ();
}
