/******************************************************************************
*       Description:
*
*       Author:
*         Date: 28 July 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "App.h"
#include "GxBootloader.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
App::App (IBoard& board) : _board (board)
{
}


void App::OnBeforeStartup ()
{
}

void App::OnStartup ()
{
    _board.Initialise ();
    new GxBootloader (_board);
}

DispatcherActions& App::GetDispatcherActions ()
{
    return _board.BoardDispatcherActions;
}
