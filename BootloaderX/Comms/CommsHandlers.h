/******************************************************************************
 *       Description:
 *
 *       Author:
 *         Date: 19 September 2016
 *
 *******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef COMMSHANDLERS_H_
#define COMMSHANDLERS_H_

#pragma mark Includes
#include "BootloaderX.h"
#include "IBootloaderService.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  CommsHandlers
 */
class CommsHandlers
{
#pragma mark Public Members
  public:
    /**
     * Instantiates a new instance of CommsHandlers.
     */
    CommsHandlers (GxBootloader& bootloader,
                   IBootloaderService& bootloaderService);
    ~CommsHandlers ();

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
    GxBootloader& _bootloader;
    IBootloaderService& _bootloaderService;
    // IDPCommandManager& _commandManager;
};

#endif
