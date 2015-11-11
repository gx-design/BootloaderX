/******************************************************************************
*       Description:
*
*       Author:
*         Date: 28 July 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes


#pragma mark Definitions and Constants
extern "C" void system_init (void);
extern int main (void);
#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Member Implementations
extern "C" void Startup ()
{
    system_init ();

    main ();
}
