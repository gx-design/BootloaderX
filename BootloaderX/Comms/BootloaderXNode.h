/******************************************************************************
 *  Description:
 *
 *       Author:
 *
 *******************************************************************************/
#pragma once

#include "BootloaderX.h"
#include "IdpNode.h"
#include <stdbool.h>
#include <stdint.h>

/**
 *  BootloaderXNode
 */
class BootloaderXNode : public IdpNode
{
  public:
    /**
     * Instantiates a new instance of BootloaderXNode
     */
    BootloaderXNode (BootloaderX& bootloader,
                     IBootloaderService& bootloaderService);

    virtual ~BootloaderXNode ();

  private:
};
