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
#include "Transactions/BeginFlashOperationRequest.h"
#include "Transactions/BeginFlashOperationResponseTransaction.h"
#include "Transactions/FlashDataRequestTransaction.h"
#include "Transactions/FlashDataResponseTransaction.h"
#include "Transactions/GetVersionRequestTransaction.h"
#include "Transactions/GetVersionResponseTransaction.h"
#include "Transactions/PingRequestTransaction.h"
#include "Transactions/PingResponseTransaction.h"
#include "Transactions/ValidateImageRequestTransaction.h"
#include "Transactions/ValidateImageResponseTransaction.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
CommsHandlers::CommsHandlers (GxBootloader& bootloader, IBootloaderService& bootloaderService)
    : _bootloader (bootloader), _bootloaderService (bootloaderService),
      _commandManager (bootloader.UsbInterface.GetCommandManager ())
{
    _commandManager.RegisterCommand<PingRequestPacket> ()
    .CommandReceived.Subscribe<CommsHandlers, &CommsHandlers::PingRequestReceived> (this);

    _commandManager.RegisterCommand<GetVersionRequestPacket> ()
    .CommandReceived.Subscribe<CommsHandlers, &CommsHandlers::GetVersionCommandRecieved> (this);

    _commandManager.RegisterCommand<BeginFlashOperationRequest> ()
    .CommandReceived.Subscribe<CommsHandlers, &CommsHandlers::BeginFlashOperationRequestReceived> (this);

    _commandManager.RegisterCommand<FlashDataRequestTransaction> ()
    .CommandReceived.Subscribe<CommsHandlers, &CommsHandlers::FlashDataRequestReceived> (this);

    _commandManager.RegisterCommand<ValidateImageRequestTransaction> ()
    .CommandReceived.Subscribe<CommsHandlers, &CommsHandlers::ValidateImageRequestReceived> (this);
}

CommsHandlers::~CommsHandlers ()
{
}

void CommsHandlers::BeginFlashOperationRequestReceived (void* sender, EventArgs& e)
{
    Command<BeginFlashOperationRequest>& request = *static_cast<Command<BeginFlashOperationRequest>*> (sender);
    Command<BeginFlashOperationResponseTransaction> response;

    _bootloaderService.EraseFirmware ();
    _bootloader.currentAddress = 0x08010000;

    IDPInterface& responseInterface = request.GetReceivedInterface ();
    IDPPacket& packet = responseInterface.GetEmptyPacket (1);

    BeginFlashOperationResponseTransaction responsePayload = response.CreateTransaction ();

    response.Encode (packet, 1, 1, responsePayload);
    _bootloader.UsbInterface.ReportIdpPacket (packet);

    _bootloader.scrambleKey = _bootloader.encryptionKey;
}

void CommsHandlers::GetVersionCommandRecieved (void* sender, EventArgs& e)
{
    Command<GetVersionRequestPacket>& request = *static_cast<Command<GetVersionRequestPacket>*> (sender);
    Command<GetVersionResponsePacket> response;

    IDPInterface& responseInterface = request.GetReceivedInterface ();
    IDPPacket& packet = responseInterface.GetEmptyPacket (1);

    GetVersionResponsePacket responsePayload = response.CreateTransaction ();
    responsePayload.version = _bootloader.Version;

    response.Encode (packet, 1, 1, responsePayload);
    _bootloader.UsbInterface.ReportIdpPacket (packet);
}

void CommsHandlers::PingRequestReceived (void* sender, EventArgs& e)
{
    Command<PingRequestPacket>& request = *static_cast<Command<PingRequestPacket>*> (sender);
    Command<PingResponsePacket> response;

    IDPInterface& responseInterface = request.GetReceivedInterface ();
    IDPPacket& packet = responseInterface.GetEmptyPacket (1);

    PingResponsePacket responsePayload = response.CreateTransaction ();
    responsePayload.pingId = request.GetReceivedPayload ().pingId;
    response.Encode (packet, 1, 1, responsePayload);

    _bootloader.UsbInterface.ReportIdpPacket (packet);
}

void CommsHandlers::FlashDataRequestReceived (void* sender, EventArgs& e)
{
    Command<FlashDataRequestTransaction>& request = *static_cast<Command<FlashDataRequestTransaction>*> (sender);
    Command<FlashDataResponseTransaction> response;

    FlashDataRequestTransaction& requestPayload = request.GetReceivedPayload ();

    for (uint8_t i = 0; i < requestPayload.length;)
    {
        uint8_t remainingSize = requestPayload.length - i;

        if (remainingSize >= 4)
        {
            uint32_t* data = reinterpret_cast<uint32_t*> (&requestPayload.data[i]);

            *data = GxBootloader::EncryptDecrypt (_bootloader.encryptionKey, _bootloader.scrambleKey, *data);

            _bootloaderService.FlashData (_bootloader.currentAddress, *data);

            i += sizeof (uint32_t);
            _bootloader.currentAddress += sizeof (uint32_t);
        }
        else if (remainingSize >= 2)
        {
            uint16_t* data = reinterpret_cast<uint16_t*> (&requestPayload.data[i]);

            _bootloaderService.FlashData (_bootloader.currentAddress, *data);

            i += sizeof (uint16_t);
            _bootloader.currentAddress += sizeof (uint16_t);
        }
        else
        {
            _bootloaderService.FlashData (_bootloader.currentAddress, requestPayload.data[i++]);
            _bootloader.currentAddress++;
        }
    }

    IDPInterface& responseInterface = request.GetReceivedInterface ();
    IDPPacket& packet = responseInterface.GetEmptyPacket (1);

    FlashDataResponseTransaction responsePayload = response.CreateTransaction ();

    responsePayload.success = true;

    response.Encode (packet, 1, 1, responsePayload);
    _bootloader.UsbInterface.ReportIdpPacket (packet);
}

void CommsHandlers::ValidateImageRequestReceived (void* sender, EventArgs& e)
{
    Command<ValidateImageRequestTransaction>& request =
    *static_cast<Command<ValidateImageRequestTransaction>*> (sender);

    Command<ValidateImageResponseTransaction> response;

    _bootloader.SetState (BootloaderState::Normal);

    IDPInterface& responseInterface = request.GetReceivedInterface ();
    IDPPacket& packet = responseInterface.GetEmptyPacket (1);

    ValidateImageResponseTransaction responsePayload = response.CreateTransaction ();

    response.Encode (packet, 1, 1, responsePayload);
    _bootloader.UsbInterface.ReportIdpPacket (packet);

    _bootloaderService.SystemReset ();
}
