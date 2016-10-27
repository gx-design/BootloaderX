/******************************************************************************
*       Description:
*
*       Author:
*         Date: 20 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef FLASHDATARESPONSETRANSACTION_H_
#define FLASHDATARESPONSETRANSACTION_H_

#pragma mark Includes
#include "Transaction.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  FlashDataResponseTransaction
 */
class FlashDataResponseTransaction : public Transaction
{
#pragma mark Public Members
  public:
    bool success;

    bool Encode (IDPPacket& packet)
    {
        packet.Write (success);

        return true;
    }

    bool Decode (Buffer& payloadData)
    {
        success = payloadData.Read<bool> ();

        return true;
    }

    uint16_t GetId ()
    {
        return 0xB007;
    }

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
};

#endif
