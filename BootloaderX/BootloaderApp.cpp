/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 28 July 2016
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "BootloaderApp.h"
#include "BootloaderX.h"
#include "Kernel.h"
#include "Thread.h"
#include "Trace.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
BootloaderApp::BootloaderApp (IPlatformImpl& platform, IBootloaderBoard& board,
                              uint32_t encryptionKey)
    : Application (platform, board), _board (board)
{
    _encryptionKey = encryptionKey;
}


void BootloaderApp::OnBeforeStartup ()
{
}

void BootloaderApp::OnStartup ()
{
    _board.Initialise ();

    if (!_board.BootloaderService->ReadFlags ()->IsBootloaderPresent ())
    {
        BootloaderX::InitialiseFlags (_board);
    }

    if (_board.BootloaderService->ReadFlags ()->Version != BootloaderX::Version)
    {
        auto flags = *_board.BootloaderService->ReadFlags ();

        flags.Version = BootloaderX::Version;

        _board.BootloaderService->WriteFlags (&flags);
    }

    if (_board.BootloaderService->ReadFlags ()->State ==
        BootloaderState::Normal)
    {
        if (_board.ForceBootloadRequested ())
        {
            BootloaderX::SetState (_board, BootloaderState::Bootloader);
        }
        else
        {
            _board.BootloaderService->JumpToApplication ();

            BootloaderX::SetState (_board, BootloaderState::Bootloader);
        }
    }

    _board.PostInitialise ();
    new BootloaderX (_board, _encryptionKey);
}

DispatcherActions& BootloaderApp::GetDispatcherActions ()
{
    return _board.PlatformDispatcherActions;
}

Dispatcher& BootloaderApp::GetCurrentDispatcher ()
{
}

void BootloaderApp::RegisterDispatcherToThread (Dispatcher& dispatcher)
{
}
