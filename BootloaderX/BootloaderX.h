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
#include "BootloaderXHidDevice.h"
#include "IBootloaderBoard.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

class GxBootloader
{
#pragma mark Public Members
  public:
    GxBootloader (IBootloaderBoard& board, uint32_t encryptionKey);
    ~GxBootloader ();

#ifndef BL_VERSION
#define BL_VERSION 1.00
#endif

    constexpr static float Version = BL_VERSION;

    void Run (void);
    void Initialise (void);

    GxBootloaderHidDevice UsbInterface;

    static void SetState (IBootloaderBoard& board, BootloaderState state);

    void SetState (BootloaderState state);

    static uint32_t EncryptDecrypt (uint32_t key, uint32_t& scrambleKey,
                                    uint32_t data);

    static void InitialiseFlags (IBootloaderBoard& board);

    uint32_t currentAddress;
    uint32_t scrambleKey;
    uint32_t encryptionKey;


#pragma mark Private Members
  private:
    IBootloaderBoard& board;
};

#endif
