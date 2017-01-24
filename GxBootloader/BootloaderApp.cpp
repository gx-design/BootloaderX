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

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
BootloaderApp::BootloaderApp (IBoard& board, uint32_t encryptionKey) : _board (board)
{
    _encryptionKey = encryptionKey;
}


void BootloaderApp::OnBeforeStartup ()
{
}

void BootloaderApp::OnStartup ()
{
    _board.Initialise ();
    new GxBootloader (_board, _encryptionKey);
}

DispatcherActions& BootloaderApp::GetDispatcherActions ()
{
    return _board.BoardDispatcherActions;
}
