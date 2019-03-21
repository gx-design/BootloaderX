/******************************************************************************
 *  Description:
 *
 *       Author:
 *
 *******************************************************************************/
#pragma once

#include "IBootloaderService.h"
#include "IdpNode.h"
#include <stdbool.h>
#include <stdint.h>

/**
 *  BootloaderXServiceNode
 */
class BootloaderXServiceNode : public IdpNode
{
  public:
    /**
     * Instantiates a new instance of BootloaderXServiceNode
     */
    BootloaderXServiceNode (IBootloaderService& bootloaderService);

    virtual ~BootloaderXServiceNode ();
};
