/******************************************************************************
*       Description:
*
*       Author:
*         Date: 19 September 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "CommsHandlers.h"
#include "Thread.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
CommsHandlers::CommsHandlers (GxBootloader& bootloader, IBootloaderService& bootloaderService)
    : _bootloader (bootloader), _bootloaderService (bootloaderService),
      _commandManager (bootloader.UsbInterface.GetCommandManager ())
{
    // clang-format off
    _commandManager.CreateTransaction (0xA002).OnReceived ([&](auto& packet, auto& response) 
    {
          response.Write(0.0f);
          response.Write(GxBootloader::Version);
          
          response.Finalise();
    });
    
    _commandManager.CreateTransaction (0xB004).OnReceived([&](auto& packet, auto& response)
    {
        _bootloaderService.EraseFirmware ();
        _bootloader.currentAddress = 0x08010000;
        _bootloader.scrambleKey = _bootloader.encryptionKey;
    
        response.Finalise();        
    });
    
    _commandManager.CreateTransaction (0xB006).OnReceived([&](Buffer& packet, auto& response)
    {
        bool result = true;
        
        auto length = packet.Read<uint8_t>(); 
        
        for (uint8_t i = 0; i < length;)
        {
            uint8_t remainingSize = length - i;
    
            if (remainingSize >= 4)
            {
                auto data = packet.Read<uint32_t>();
    
                data = GxBootloader::EncryptDecrypt (_bootloader.encryptionKey, _bootloader.scrambleKey, data);
    
                _bootloaderService.FlashData (_bootloader.currentAddress, data);
    
                i += sizeof (uint32_t);
                _bootloader.currentAddress += sizeof (uint32_t);
            }
            else if (remainingSize >= 2)
            {
                result = false;
                // This would be a bug!
                auto data = packet.Read<uint16_t>();
    
                _bootloaderService.FlashData (_bootloader.currentAddress, data);
    
                i += sizeof (uint16_t);
                _bootloader.currentAddress += sizeof (uint16_t);
            }
            else
            {
                result = false;
                // This would be a bug!
                auto data = packet.Read<uint8_t>();
                _bootloaderService.FlashData (_bootloader.currentAddress, data);
                _bootloader.currentAddress++;
            }
        }

        response.Write(result);
        response.Finalise();
    });
    
    _commandManager.CreateTransaction (0xB008).OnReceived([&](auto& packet, auto& response)
    {
        _bootloader.SetState (BootloaderState::Normal);    
        _bootloaderService.SystemReset ();
    });

    // clang-format on
}

CommsHandlers::~CommsHandlers ()
{
}
