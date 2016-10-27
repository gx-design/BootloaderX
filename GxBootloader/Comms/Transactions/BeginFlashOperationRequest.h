/******************************************************************************
*       Description: 
*
*       Author: 
*         Date: 20 October 2016
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef BEGINFLASHOPERATIONREQUEST_H_
#define BEGINFLASHOPERATIONREQUEST_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "Transaction.h"

/**
 *  BeginFlashOperationRequest
 */
class BeginFlashOperationRequest : public Transaction
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
        return 0xB004;
    }

#pragma mark Protected Members
protected:

#pragma mark Private Members
private:

};

#endif
