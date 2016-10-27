/******************************************************************************
*       Description:
*
*       Author:
*         Date: 17 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef GETVERSIONREQUESTPACKET_H_
#define GETVERSIONREQUESTPACKET_H_

#pragma mark Includes
#include "Transaction.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  GetVersionRequestPacket
 */
class GetVersionRequestPacket : public Transaction
{
#pragma mark Public Members
  public:
    float version;
    bool Encode (IDPPacket& packet)
    {
        packet.Write (version);

        return true;
    }

    bool Decode (Buffer& payloadData)
    {
        version = payloadData.Read<float> ();

        return true;
    }

    uint16_t GetId ()
    {
        return 0xA002;
    }

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
};

#endif
