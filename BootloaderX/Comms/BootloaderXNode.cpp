/******************************************************************************
 *  Description:
 *
 *       Author:
 *
 ******************************************************************************/

#include "BootloaderXNode.h"
#include "BitConverter.h"

BootloaderXNode::BootloaderXNode (BootloaderX& bootloader,
                                  IBootloaderService& bootloaderService)
    : IdpNode (Guid_t ("EBBF4FFB-0F3C-4975-83D2-B6FE4643D641"), "BootloaderX")
{
    Timeout (10000);

    Manager ().RegisterCommand (0xB001,
                                [&](std::shared_ptr<IncomingTransaction> i,
                                    std::shared_ptr<OutgoingTransaction> o) {
                                    o->Write(BootloaderX::VersionMajor);
                                    o->Write(BootloaderX::VersionMinor);
                                    o->Write(BootloaderX::VersionBuild);
                                    o->Write(BootloaderX::VersionRev);

                                    return IdpResponseCode::OK;
                                });

    Manager ().RegisterCommand (
        0xB002, [&](std::shared_ptr<IncomingTransaction> i,
                    std::shared_ptr<OutgoingTransaction> o) {
            o->Write (0.0f);
            o->Write (BootloaderX::Version);
            bootloaderService.EraseFirmware ();
            bootloader.currentAddress = 0x08010000;
            bootloader.scrambleKey = bootloader.encryptionKey;

            return IdpResponseCode::OK;
        });

    Manager ().RegisterCommand (
        0xB003, [&](std::shared_ptr<IncomingTransaction> i,
                    std::shared_ptr<OutgoingTransaction> o) {
            o->Write (0.0f);
            o->Write (BootloaderX::Version);
            bool result = true;

            auto length = i->Read<uint8_t> ();

            for (uint8_t n = 0; n < length;)
            {
                uint8_t remainingSize = length - n;

                if (remainingSize >= 4)
                {
                    auto data = BitConverter::SwapEndian (i->Read<uint32_t> ());

                    data = BootloaderX::EncryptDecrypt (
                        bootloader.encryptionKey, bootloader.scrambleKey, data);

                    bootloaderService.FlashData (bootloader.currentAddress,
                                                 data);

                    n += sizeof (uint32_t);
                    bootloader.currentAddress += sizeof (uint32_t);
                }
                else if (remainingSize >= 2)
                {
                    result = false;
                    // This would be a bug!
                    auto data = BitConverter::SwapEndian (i->Read<uint16_t> ());

                    bootloaderService.FlashData (bootloader.currentAddress,
                                                 data);

                    n += sizeof (uint16_t);
                    bootloader.currentAddress += sizeof (uint16_t);
                }
                else
                {
                    result = false;
                    // This would be a bug!
                    auto data = BitConverter::SwapEndian (i->Read<uint8_t> ());
                    bootloaderService.FlashData (bootloader.currentAddress,
                                                 data);
                    bootloader.currentAddress++;
                }
            }

            o->Write (result);
            return IdpResponseCode::OK;
        });

    Manager ().RegisterCommand (
        0xB004, [&](std::shared_ptr<IncomingTransaction> i,
                    std::shared_ptr<OutgoingTransaction> o) {
            o->Write (0.0f);
            o->Write (BootloaderX::Version);

            bootloader.SetState (BootloaderState::Normal);
            bootloaderService.SystemReset ();
            return IdpResponseCode::OK;
        });
}

BootloaderXNode::~BootloaderXNode ()
{
}
