/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 28 July 2016
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas
#pragma once

#pragma mark Includes
#include "Application.h"

#include "IBootloaderBoard.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Main App class that implements Application bootstrapping.
 */
class BootloaderApp : public Application
{
#pragma mark Public Members
  public:
    /**
     * Instantiates a new instance of App.
     * @param board - a reference to the board interface for this platform.
     */
    BootloaderApp (IPlatformImpl& platform, IBootloaderBoard& board,
                   uint32_t encryptionKey);

    virtual ~BootloaderApp ()
    {
    }

#pragma mark Application Implementation
    void OnBeforeStartup ();

    void OnStartup ();

    DispatcherActions& GetDispatcherActions ();

#pragma mark Private Members
  private:
    IBootloaderBoard& _board; //!< A reference to the board interface.
    uint32_t _encryptionKey;
};