/******************************************************************************
*       Description:
*
*       Author:
*         Date: 28 July 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "App.h"
#include "ProductionBoardA.h"

#pragma mark Definitions and Constats


#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
extern "C" int main ()
{
    static auto board = ProductionBoardA (); //!< Instance of the board interface.
    
    (*new App (board, 0x3A52C6E8)).Run ();
}
