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
GxBootloader::GxBootloader (IBoard& board)
    : board (board), mainDispatcher (Dispatcher (*board.dispatcherActions)),
      UsbInterface (GxBootloaderHidDevice (*board.hidDevice, mainDispatcher))
{
}

GxBootloader::~GxBootloader ()
{
}


void GxBootloader::Run ()
{
    board.JumpToApplication ();
    
    Initialise ();   

    UsbInterface.EraseFirmwareRequested += [&](void* sender, EventArgs& args)
    {
        board.EraseFirmware ();

        currentAddress = 0x0800C000;
        UsbInterface.Acknowlege (0);
    };

    UsbInterface.FlashDataRequested += [&](void* sender, EventArgs& args)
    {
        auto flashArgs = (FlashDataEventArgs&)args;

        for (uint8_t i = 0; i < flashArgs.length;)
        {
            uint8_t remainingSize = flashArgs.length - i;

            if (remainingSize >= 4)
            {
                uint32_t* data = (uint32_t*)&flashArgs.data[i];
                board.FlashData (currentAddress, *data);
                i += 4;
                currentAddress += 4;
            }
            else if (remainingSize >= 2)
            {
                uint16_t* data = (uint16_t*)&flashArgs.data[i];
                board.FlashData (currentAddress, *data);
                i += 2;
                currentAddress += 2;
            }
            else if (remainingSize >= 1)
            {
                board.FlashData (currentAddress, flashArgs.data[i]);
                i++;
                currentAddress++;
            }
        }

        UsbInterface.Acknowlege (1);
    };

    while (true)
    {
        mainDispatcher.RunSingle ();
    }
}

void GxBootloader::Initialise ()
{
    board.hidDevice->InitialiseStack ();
}
