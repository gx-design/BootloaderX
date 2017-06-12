/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "CRC.h"
#include "Comms/CommsHandlers.h"
#include "GxBootloader.h"
#include "Kernel.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
GxBootloader::GxBootloader (IBoard& board, uint32_t encryptionKey)
    : UsbInterface (GxBootloaderHidDevice (*board.HidDevice)), board (board)
{
    this->encryptionKey = encryptionKey;
    Dispatcher::Invoke ([&] { Initialise (); });
}

GxBootloader::~GxBootloader ()
{
}

void GxBootloader::InitialiseFlags ()
{
    BootloaderFlags flags;

    flags.Version = GxBootloader::Version;

    board.BootloaderService->WriteFlags (&flags);
}

void GxBootloader::SetState (BootloaderState state)
{
    auto flags = *board.BootloaderService->ReadFlags ();

    flags.State = state;

    board.BootloaderService->WriteFlags (&flags);
}

void GxBootloader::Run ()
{
}

void GxBootloader::Initialise ()
{
    if (!board.BootloaderService->ReadFlags ()->IsBootloaderPresent ())
    {
        InitialiseFlags ();
    }

    if (board.BootloaderService->ReadFlags ()->Version != Version)
    {
        auto flags = *board.BootloaderService->ReadFlags ();

        flags.Version = Version;

        board.BootloaderService->WriteFlags (&flags);
    }

    if (board.ForceBootloadRequested ())
    {
        SetState (BootloaderState::Bootloader);
    }
    else if (board.BootloaderService->ReadFlags ()->State == BootloaderState::Normal)
    {
        board.BootloaderService->JumpToApplication ();

        SetState (BootloaderState::Bootloader);
    }

    board.PostInitialise ();

    board.HidDevice->InitialiseStack ();

    new CommsHandlers (*this, *board.BootloaderService);
}

uint32_t GxBootloader::EncryptDecrypt (uint32_t key, uint32_t& scrambleKey, uint32_t data)
{
    uint8_t* scrambleBytes = reinterpret_cast<uint8_t*> (&scrambleKey);

    uint16_t crc = static_cast<uint16_t> (scrambleKey);

    for (int i = 0; i < sizeof (uint32_t); i++)
    {
        crc = CRC::Crc16 (crc, (int8_t)*scrambleBytes++);
    }

    scrambleKey = scrambleKey & 0xFFFF0000;
    scrambleKey |= crc;

    scrambleBytes = reinterpret_cast<uint8_t*> (&scrambleKey);

    crc = static_cast<uint16_t> (scrambleKey);

    for (int i = 0; i < sizeof (uint32_t); i++)
    {
        crc = CRC::Crc16 (crc, (int8_t)*scrambleBytes++);
    }

    scrambleKey = scrambleKey & 0x0000FFFF;
    scrambleKey |= ((uint32_t)crc) << 16;

    return data ^ key ^ scrambleKey;
}
