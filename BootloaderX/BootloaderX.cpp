/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 10 November 2015
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "BootloaderX.h"
#include "CRC.h"
#include "Comms/BootloaderXNode.h"
#include "IdpRouter.h"
#include "Kernel.h"

#pragma mark Definitions and Constants
static const uint16_t vendorId = 0xFD02;
static const uint16_t productId = 0x0002;
static const char* manufacturerString = "GX Design";
static const char* productString = "GX Device Firmware Bootloader";
static const char* serialString = "00000001";

#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
BootloaderX::BootloaderX (IBootloaderBoard& board, uint32_t encryptionKey)
    : UsbInterface (BootloaderXHidDevice (*board.HidDevice, vendorId, productId,
                                          manufacturerString, productString,
                                          serialString)),
      board (board)
{
    this->encryptionKey = encryptionKey;
    Dispatcher::Invoke ([&] { Initialise (); });
}

BootloaderX::~BootloaderX ()
{
}

void BootloaderX::InitialiseFlags (IBootloaderBoard& board)
{
    BootloaderFlags flags;

    flags.Version = BootloaderX::Version;

    board.BootloaderService->WriteFlags (&flags);
}

void BootloaderX::SetState (IBootloaderBoard& board, BootloaderState state)
{
    auto flags = *board.BootloaderService->ReadFlags ();

    flags.State = state;

    board.BootloaderService->WriteFlags (&flags);
}

void BootloaderX::SetState (BootloaderState state)
{
    BootloaderX::SetState (board, state);
}

void BootloaderX::Initialise ()
{
    board.HidDevice->InitialiseStack ();

    auto router = new IdpRouter ();

    router->Timeout (10000);

    router->AddAdaptor (UsbInterface);

    router->AddNode (*new BootloaderXNode (*this, *board.BootloaderService));
}

uint32_t BootloaderX::EncryptDecrypt (uint32_t key, uint32_t& scrambleKey,
                                      uint32_t data)
{
    uint8_t* scrambleBytes = reinterpret_cast<uint8_t*> (&scrambleKey);

    uint16_t crc = static_cast<uint16_t> (scrambleKey);

    for (int i = 0; i < sizeof (uint32_t); i++)
    {
        crc = CRC::Crc16 (crc, (int8_t) *scrambleBytes++);
    }

    scrambleKey = scrambleKey & 0xFFFF0000;
    scrambleKey |= crc;

    scrambleBytes = reinterpret_cast<uint8_t*> (&scrambleKey);

    crc = static_cast<uint16_t> (scrambleKey);

    for (int i = 0; i < sizeof (uint32_t); i++)
    {
        crc = CRC::Crc16 (crc, (int8_t) *scrambleBytes++);
    }

    scrambleKey = scrambleKey & 0x0000FFFF;
    scrambleKey |= ((uint32_t) crc) << 16;

    return data ^ key ^ scrambleKey;
}
