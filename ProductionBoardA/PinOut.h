/******************************************************************************
*       Description:
*
*       Author:
*         Date: 29 July 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas
#ifndef _PINOUT_H_
#define _PINOUT_H_

#pragma mark Includes
#include "Interrupts.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef* Port;
    uint32_t Pin;
} PortPin;

typedef struct
{
    PortPin GPIO;
    uint32_t AlternateFunctionId;
} AlternateFunctionPin;

typedef struct
{
    TIM_HandleTypeDef* Handle;
    uint32_t AlternateFunctionId;
    IrqNumber UpdateIRQ;
    IrqNumber Channel1IRQ;
    IrqNumber Channel2IRQ;
    IrqNumber Channel3IRQ;
    IrqNumber Channel4IRQ;
} TimerConfig;

typedef struct
{
    PortPin GPIO;
    TimerConfig timerConfig;
} SignalPin;

typedef struct
{
    PortPin Port;
    uint32_t AlternateFunctionId;
} PeripheralPin;

typedef struct
{
    SignalPin EnableA;
    SignalPin EnableB;
    PortPin M0;
    PortPin M1;
    PortPin Dir;
} DRV8834Pins;

typedef struct
{
    SignalPin EnableA;
    PortPin DirA;

    SignalPin EnableB;
    PortPin DirB;
} DRV8814Pins;

typedef struct
{
    SignalPin Step;
    PortPin Dir;
    PortPin Enable;
} DRV882xPins;

extern UART_HandleTypeDef Uart4;

extern const TimerConfig Timer1Config;
extern const TimerConfig Timer2Config;
extern const TimerConfig Timer3Config;
extern const TimerConfig Timer4Config;
extern const TimerConfig Timer5Config;
extern const TimerConfig Timer8Config;
extern const TimerConfig Timer9Config;
extern const TimerConfig Timer10Config;
extern const TimerConfig Timer11Config;
extern const TimerConfig Timer12Config;
extern const TimerConfig Timer13Config;
extern const TimerConfig Timer14Config;


extern const PortPin LED1;
extern const PortPin LED2;
extern const PortPin LED3;

extern const PortPin Relay;
extern const PortPin RfidEnable;

extern const PortPin SolenoidInhibit;
extern const PortPin SolenoidCS;

extern const PortPin SPICS1;
extern const PortPin SPICS2;
extern const PortPin SPICS3;
extern const PortPin SPICS4;

extern const AlternateFunctionPin SPI2_MOSI;
extern const AlternateFunctionPin SPI2_SCK;

extern const PortPin TempChannel1;
extern const PortPin TempChannel2;
extern const PortPin TempChannel3;
extern const PortPin TempChannel4;
extern const PortPin VrefSample;

extern const PortPin OptoIn1;
extern const PortPin OptoIn2;
extern const PortPin OptoIn3;
extern const PortPin OptoIn4;
extern const PortPin OptoIn5;
extern const PortPin OptoIn6;
extern const PortPin OptoIn7;
extern const PortPin OptoIn8;
extern const PortPin OptoIn9;

extern const PortPin MuxAddr0;
extern const PortPin MuxAddr1;
extern const PortPin MuxAddr2;
extern const PortPin MuxAddr3;

extern const PortPin Speaker;

extern const PortPin Button1;
extern const PortPin Button2;
extern const PortPin Button3;
extern const PortPin Button4;
extern const PortPin RfidIRQPin;
extern const PortPin RfidAntSel;
extern const PortPin DisplayRs;
extern const PortPin DisplayE;
extern const PortPin DisplayRw;

extern const PortPin DisplayDB0;
extern const PortPin DisplayDB1;
extern const PortPin DisplayDB2;
extern const PortPin DisplayDB3;
extern const PortPin DisplayDB4;
extern const PortPin DisplayDB5;
extern const PortPin DisplayDB6;
extern const PortPin DisplayDB7;

extern const PeripheralPin OneWire;
extern const PeripheralPin Spi1Mosi;
extern const PeripheralPin Spi1Miso;
extern const PeripheralPin Spi1Clk;
extern const PeripheralPin Uart4Tx;
extern const PeripheralPin Uart4Rx;


extern const DRV882xPins Drive1;
extern const DRV882xPins Drive2;
extern const DRV882xPins Drive3;
extern const DRV882xPins Drive4;
extern const DRV882xPins Drive5;
extern const DRV882xPins Drive6;

extern const DRV8814Pins DCDriver1;
extern const DRV8814Pins DCDriver2;

// clang-format on

extern const SignalPin FanA;
extern const SignalPin FanB;

extern const SignalPin DCPwm1;
extern const SignalPin DCPwm2;
extern const SignalPin ServoA;
extern const SignalPin ServoB;

extern const SignalPin PowerOutA;
extern const SignalPin PowerOutB;

extern const SignalPin Encoder1A;
extern const SignalPin Encoder1B;

// Encoder 2
extern const SignalPin Encoder2A;
extern const SignalPin Encoder2B;

// Encoder 3
extern const SignalPin Encoder3A;
extern const SignalPin Encoder3B;

// Encoder 4
extern const SignalPin Encoder4A;
extern const SignalPin Encoder4B;

#endif