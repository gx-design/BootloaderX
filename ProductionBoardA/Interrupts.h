/******************************************************************************
*       Description:
*
*       Author:
*         Date: 29 July 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#pragma mark Includes
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef enum IrqNumber
{
    Timer1UpdateIRQ = 0,
    Timer2UpdateIRQ,
    Timer3UpdateIRQ,
    Timer4UpdateIRQ,
    Timer5UpdateIRQ,
    Timer8UpdateIRQ,
    Timer9UpdateIRQ,
    Timer10UpdateIRQ,
    Timer11UpdateIRQ,
    Timer12UpdateIRQ,
    Timer13UpdateIRQ,
    Timer14UpdateIRQ,

    Timer1Channel1IRQ,
    Timer1Channel2IRQ,
    Timer1Channel3IRQ,
    Timer1Channel4IRQ,

    Timer2Channel1IRQ,
    Timer2Channel2IRQ,
    Timer2Channel3IRQ,
    Timer2Channel4IRQ,

    Timer3Channel1IRQ,
    Timer3Channel2IRQ,
    Timer3Channel3IRQ,
    Timer3Channel4IRQ,

    Timer4Channel1IRQ,
    Timer4Channel2IRQ,
    Timer4Channel3IRQ,
    Timer4Channel4IRQ,

    Timer5Channel1IRQ,
    Timer5Channel2IRQ,
    Timer5Channel3IRQ,
    Timer5Channel4IRQ,
    
    Timer8Channel1IRQ,
    Timer8Channel2IRQ,
    Timer8Channel3IRQ,
    Timer8Channel4IRQ,

    Timer9Channel1IRQ,
    Timer9Channel2IRQ,

    Timer10Channel1IRQ,
    
    Timer11Channel1IRQ,

    Timer12Channel1IRQ,
    Timer12Channel2IRQ,
    
    Timer13Channel1IRQ,
    
    Timer14Channel1IRQ,
    
    HopperFeedOptoIRQ,
    CardEntryOptoIRQ,
    CardExitOptoIRQ,
    
    TapeEntryUpperOptoIRQ,
    TapeExitUpperOptoIRQ,
    TapeEntryLowerOptoIRQ,
    TapeExitLowerOptoIRQ,
    
    Button1IRQ,
    Button2IRQ,
    Button3IRQ,
    Button4IRQ,
    
    RfidIRQ,
    
    Uart4IRQ,

    SysTickIRQ
} IrqNumber;

#endif
