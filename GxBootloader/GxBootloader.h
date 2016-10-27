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
#include "GxBootloaderHidDevice.h"
#include "IBoard.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class GxBootloader
{
#pragma mark Public Members
  public:
    GxBootloader (IBoard& board, uint32_t encryptionKey);
    ~GxBootloader ();

    const float Version = 0.89;

    void Run (void);
    void Initialise (void);

    GxBootloaderHidDevice UsbInterface;

    void SetState (BootloaderState state);

    static uint32_t EncryptDecrypt (uint32_t key, uint32_t& scrambleKey, uint32_t data);

    uint32_t currentAddress;
    uint32_t scrambleKey;
    uint32_t encryptionKey;


#pragma mark Private Members
  private:
    IBoard& board;

    void InitialiseFlags ();
};

#endif
