/******************************************************************************
 *  Description:
 *
 *       Author:
 *
 ******************************************************************************/

#include "BootloaderXServiceNode.h"

BootloaderXServiceNode::BootloaderXServiceNode (
    IBootloaderService& bootloaderService)
    : IdpNode ("182D7497-F3EE-4975-AD4D-F45686EBCB6E", "BootloaderX.Service")
{
    Manager ().RegisterCommand (0xB501,
                                [&](std::shared_ptr<IncomingTransaction> i,
                                    std::shared_ptr<OutgoingTransaction> o) {
                                    bootloaderService.SetBootloaderFlag ();
                                    bootloaderService.SystemReset ();

                                    return IdpResponseCode::OK;
                                });
}

BootloaderXServiceNode::~BootloaderXServiceNode ()
{
}
