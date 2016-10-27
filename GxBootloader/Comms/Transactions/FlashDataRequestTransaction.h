/******************************************************************************
*       Description:
*
*       Author:
*         Date: 20 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef FLASHDATAREQUESTTRANSACTION_H_
#define FLASHDATAREQUESTTRANSACTION_H_

#pragma mark Includes
#include "Transaction.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  FlashDataRequestTransaction
 */
class FlashDataRequestTransaction : public Transaction
{
#pragma mark Public Members
  public:
    uint8_t length;
    uint8_t data[64];

    bool Encode (IDPPacket& packet)
    {
        packet.Write (length);

        for (uint32_t i = 0; i < length; i++)
        {
            packet.Write (data[i]);
        }

        return true;
    }

    bool Decode (Buffer& payloadData)
    {
        length = payloadData.Read<uint8_t> ();

        for (uint32_t i = 0; i < length; i++)
        {
            data[i] = payloadData.Read<uint8_t> ();
        }

        return true;
    }

    uint16_t GetId ()
    {
        return 0xB006;
    }

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
};

#endif
