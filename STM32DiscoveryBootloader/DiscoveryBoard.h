/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _DISCOVERYBOARD_H_
#define _DISCOVERYBOARD_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "IBoard.h"
#include "GxBootloader.h"

class DiscoveryBoard : public IBoard
{
#pragma mark Public Members
  public:
    DiscoveryBoard ();
    ~DiscoveryBoard ();

#pragma mark Private Members
  private:
    
};

#endif
