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
#include "GxBootloader.h"
#include "Kernel.h"
#include "Thread.h"
#include "Trace.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
BootloaderApp::BootloaderApp (IBoard& board, uint32_t encryptionKey, GetSystemTimeDelegate delegate)
    : Application (delegate), _board (board)
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
        GxBootloader::InitialiseFlags (_board);
    }

    if (_board.BootloaderService->ReadFlags ()->Version != GxBootloader::Version)
    {
        auto flags = *_board.BootloaderService->ReadFlags ();

        flags.Version = GxBootloader::Version;

        _board.BootloaderService->WriteFlags (&flags);
    }

    if (_board.ForceBootloadRequested ())
    {
        GxBootloader::SetState (_board, BootloaderState::Bootloader);
    }
    else if (_board.BootloaderService->ReadFlags ()->State == BootloaderState::Normal)
    {
        _board.BootloaderService->JumpToApplication ();

        GxBootloader::SetState (_board, BootloaderState::Bootloader);
    }

    _board.PostInitialise ();
    new GxBootloader (_board, _encryptionKey);
}

DispatcherActions& BootloaderApp::GetDispatcherActions ()
{
    return _board.PlatformDispatcherActions;
}

void BootloaderApp::Run ()
{
    Thread::StartNew ([&] { Main (); }, 6000);
    Thread::StartNew ([&] { PriorityThread (); }, 2000);

    Kernel::Start ();
}


Dispatcher& BootloaderApp::GetCurrentDispatcher ()
{
    auto thread = Thread::GetCurrentThread ();
    return *(Dispatcher*)thread->UserData;
}

void BootloaderApp::RegisterDispatcherToThread (Dispatcher& dispatcher)
{
    auto thread = Thread::GetCurrentThread ();
    thread->UserData = &dispatcher;
}
