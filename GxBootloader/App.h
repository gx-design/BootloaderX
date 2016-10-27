/******************************************************************************
*       Description:
*
*       Author:
*         Date: 28 July 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef APP_H_
#define APP_H_

#pragma mark Includes
#include "Application.h"

#include "IBoard.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Main App class that implements Application bootstrapping.
 */
class App : public Application
{
#pragma mark Public Members
  public:
    /**
     * Instantiates a new instance of App.
     * @param board - a reference to the board interface for this platform.
     */
    App (IBoard& board, uint32_t encryptionKey);

#pragma mark Application Implementation
    void OnBeforeStartup ();

    void OnStartup ();

    DispatcherActions& GetDispatcherActions ();

#pragma mark Private Members
  private:
    IBoard& _board; //!< A reference to the board interface.
    uint32_t _encryptionKey;
};

#endif