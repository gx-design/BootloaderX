/******************************************************************************
*       Description:
*
*       Author:
*         Date: 17 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef PINGREQUESTPACKET_H_
#define PINGREQUESTPACKET_H_

#pragma mark Includes
#include "Transaction.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  PingRequestPacket
 */
class PingRequestPacket : public Transaction
{
#pragma mark Public Members
  public:
    uint16_t pingId;

    bool Encode (IDPPacket& packet)
    {
        packet.Write (pingId);

        return true;
    }

    bool Decode (Buffer& payloadData)
    {
        pingId = payloadData.Read<uint16_t> ();

        return true;
    }

    uint16_t GetId ()
    {
        return 0xA000;
    }

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
};

#endif
