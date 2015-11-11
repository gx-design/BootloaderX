/******************************************************************************
*       Description:
*
*       Author:
*         Date: 22 April 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas

#pragma mark Includes
#include "GxBootloader.h"
#include "DiscoveryBoard.h"

static DiscoveryBoard board = DiscoveryBoard ();


int main (void)
{
    static GxBootloader application = GxBootloader (board);

    application.Run ();
}
