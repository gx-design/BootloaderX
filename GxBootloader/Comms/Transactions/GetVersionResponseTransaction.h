/******************************************************************************
*       Description:
*
*       Author:
*         Date: 17 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef GETVERSIONRESPONSEPACKET_H_
#define GETVERSIONRESPONSEPACKET_H_

#pragma mark Includes
#include "Transaction.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 *  GetVersionResponsePacket
 */
class GetVersionResponsePacket : public Transaction
{
#pragma mark Public Members
  public:
    float version;
    char buildStringData[20];

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
        return 0xA003;
    }

#pragma mark Protected Members
  protected:
#pragma mark Private Members
  private:
};

#endif
