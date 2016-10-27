/******************************************************************************
*       Description:
*
*       Author:
*         Date: 20 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef BEGINFLASHOPERATIONRESPONSETRANSACTION_H_
#define BEGINFLASHOPERATIONRESPONSETRANSACTION_H_

#pragma mark Includes
#include "Transaction.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  BeginFlashOperationResponseTransaction
 */
class BeginFlashOperationResponseTransaction : public Transaction
{
#pragma mark Public Members
  public:
    bool Encode (IDPPacket& packet)
    {
        return true;
    }

    bool Decode (Buffer& payloadData)
    {
        return true;
    }

    uint16_t GetId ()
    {
        return 0xB005;
    }

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
};

#endif
