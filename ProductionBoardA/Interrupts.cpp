/******************************************************************************
*       Description:
*
*       Author:
*         Date: 29 July 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "Event.h"
#include "IBoard.h"
#include "Interrupt.h"
#include "Interrupts.h"
#include "PinOut.h"
#pragma mark Definitions and Constants


#pragma mark Static Data
IBoard* localBoard;

#pragma mark Static Functions

extern "C" {
void HAL_SYSTICK_Callback (void)
{
    // TODO find a way to dispatch this immediately.
    Interrupt::Call (SysTickIRQ);
}

void UART4_IRQHandler (void)
{
    if (__HAL_UART_GET_FLAG (&Uart4, UART_FLAG_RXNE) != RESET || __HAL_UART_GET_FLAG (&Uart4, UART_FLAG_TXE))
    {
        Interrupt::Call (Uart4IRQ);

        __HAL_UART_CLEAR_FLAG (&Uart4, UART_FLAG_RXNE);
        __HAL_UART_CLEAR_FLAG (&Uart4, UART_FLAG_TXE);
    }
}
}

#pragma mark Member Implementations
