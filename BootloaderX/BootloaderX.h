/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 10 November 2015
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas
#pragma once

#pragma mark Includes
#include "BootloaderXHidDevice.h"
#include "IBootloaderBoard.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef VERSION_MAJ
#define VERSION_MAJ (1)
#endif

#ifndef VERSION_MIN
#define VERSION_MIN (1)
#endif

#ifndef VERSION_REV
#define VERSION_REV (0)
#endif

#ifndef BUILD
#define BUILD (1)
#endif


class BootloaderX
{
#pragma mark Public Members
  public:
    BootloaderX (IBootloaderBoard& board, uint32_t encryptionKey);
    ~BootloaderX ();

#ifndef BL_VERSION
#define BL_VERSION 1.00
#endif

    constexpr static int VersionMajor = VERSION_MAJ;
    constexpr static int VersionMinor = VERSION_MIN;
    constexpr static int VersionRev = VERSION_REV;
    constexpr static int VersionBuild = BUILD;

    constexpr static float Version = BL_VERSION;

    void Run (void);
    void Initialise (void);

    BootloaderXHidDevice UsbInterface;

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
