/******************************************************************************
*       Description:
*
*       Author:
*         Date: 24 September 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "PinOut.h"
#include "Interrupts.h"

#pragma mark Definitions and Constants
static TIM_HandleTypeDef htim1 = {.Instance = TIM1 };
static TIM_HandleTypeDef htim2 = {.Instance = TIM2 };
static TIM_HandleTypeDef htim3 = {.Instance = TIM3 };
static TIM_HandleTypeDef htim4 = {.Instance = TIM4 };
static TIM_HandleTypeDef htim5 = {.Instance = TIM5 };
static TIM_HandleTypeDef htim8 = {.Instance = TIM8 };
static TIM_HandleTypeDef htim9 = {.Instance = TIM9 };
static TIM_HandleTypeDef htim10 = {.Instance = TIM10 };
static TIM_HandleTypeDef htim11 = {.Instance = TIM11 };
static TIM_HandleTypeDef htim12 = {.Instance = TIM12 };
static TIM_HandleTypeDef htim13 = {.Instance = TIM13 };
static TIM_HandleTypeDef htim14 = {.Instance = TIM14 };

UART_HandleTypeDef Uart4 = {.Instance = UART4 };


// clang-format off
const TimerConfig Timer1Config = {.Handle = &htim1,
                                   .AlternateFunctionId = GPIO_AF1_TIM1,
                                   .UpdateIRQ = Timer1UpdateIRQ,
                                   .Channel1IRQ = Timer1Channel1IRQ,
                                   .Channel2IRQ = Timer1Channel2IRQ,
                                   .Channel3IRQ = Timer1Channel3IRQ,
                                   .Channel4IRQ = Timer1Channel4IRQ};

const TimerConfig Timer2Config = {.Handle = &htim2,
                                   .AlternateFunctionId = GPIO_AF1_TIM2,
                                   .UpdateIRQ = Timer2UpdateIRQ,
                                   .Channel1IRQ = Timer2Channel1IRQ,
                                   .Channel2IRQ = Timer2Channel2IRQ,
                                   .Channel3IRQ = Timer2Channel3IRQ,
                                   .Channel4IRQ = Timer2Channel4IRQ};

const TimerConfig Timer3Config = {.Handle = &htim3,
                                   .AlternateFunctionId = GPIO_AF2_TIM3,
                                   .UpdateIRQ = Timer3UpdateIRQ,
                                   .Channel1IRQ = Timer3Channel1IRQ,
                                   .Channel2IRQ = Timer3Channel2IRQ,
                                   .Channel3IRQ = Timer3Channel3IRQ,
                                   .Channel4IRQ = Timer3Channel4IRQ};

const TimerConfig Timer4Config = {.Handle = &htim4,
                                   .AlternateFunctionId = GPIO_AF2_TIM4,
                                   .UpdateIRQ = Timer4UpdateIRQ,
                                   .Channel1IRQ = Timer4Channel1IRQ,
                                   .Channel2IRQ = Timer4Channel2IRQ,
                                   .Channel3IRQ = Timer4Channel3IRQ,
                                   .Channel4IRQ = Timer4Channel4IRQ};

const TimerConfig Timer5Config = {.Handle = &htim5,
                                   .AlternateFunctionId = GPIO_AF2_TIM5,
                                   .UpdateIRQ = Timer5UpdateIRQ,
                                   .Channel1IRQ = Timer5Channel1IRQ,
                                   .Channel2IRQ = Timer5Channel2IRQ,
                                   .Channel3IRQ = Timer5Channel3IRQ,
                                   .Channel4IRQ = Timer5Channel4IRQ};

const TimerConfig Timer8Config = {.Handle = &htim8,
                                   .AlternateFunctionId = GPIO_AF3_TIM8,
                                   .UpdateIRQ = Timer8UpdateIRQ,
                                   .Channel1IRQ = Timer8Channel1IRQ,
                                   .Channel2IRQ = Timer8Channel2IRQ,
                                   .Channel3IRQ = Timer8Channel3IRQ,
                                   .Channel4IRQ = Timer8Channel4IRQ};

const TimerConfig Timer9Config = {.Handle = &htim9, 
                                   .AlternateFunctionId = GPIO_AF3_TIM9, 
                                   .UpdateIRQ = Timer9UpdateIRQ,
                                   .Channel1IRQ = Timer9Channel1IRQ, 
                                   .Channel2IRQ = Timer9Channel2IRQ};

const TimerConfig Timer10Config = {.Handle = &htim10, 
                                    .AlternateFunctionId = GPIO_AF3_TIM10,
                                    .UpdateIRQ = Timer10UpdateIRQ,
                                    .Channel1IRQ = Timer10Channel1IRQ};

const TimerConfig Timer11Config = {.Handle = &htim11,
                                    .AlternateFunctionId = GPIO_AF3_TIM11,
                                    .UpdateIRQ = Timer11UpdateIRQ, 
                                    .Channel1IRQ = Timer11Channel1IRQ};

const TimerConfig Timer12Config = {.Handle = &htim12, 
                                    .AlternateFunctionId = GPIO_AF9_TIM12, 
                                    .UpdateIRQ = Timer12UpdateIRQ, 
                                    .Channel1IRQ = Timer12Channel1IRQ, 
                                    .Channel2IRQ = Timer12Channel2IRQ };

const TimerConfig Timer13Config = {.Handle = &htim13,
                                    .AlternateFunctionId = GPIO_AF9_TIM13,
                                    .UpdateIRQ = Timer13UpdateIRQ, 
                                    .Channel1IRQ = Timer13Channel1IRQ};

const TimerConfig Timer14Config = {.Handle = &htim14,
                                    .AlternateFunctionId = GPIO_AF9_TIM14,
                                    .UpdateIRQ = Timer14UpdateIRQ,
                                    .Channel1IRQ = Timer14Channel1IRQ};


const DRV882xPins Drive1 = 
{    
    .Step = {{.Port = GPIOC, .Pin = GPIO_PIN_6}, .timerConfig=Timer8Config },
    .Dir = {.Port = GPIOE, .Pin = GPIO_PIN_4},
    .Enable = {.Port = GPIOB, .Pin = GPIO_PIN_11},
};

const DRV882xPins Drive2 = 
{    
    .Step = {{.Port = GPIOC, .Pin = GPIO_PIN_7}, .timerConfig=Timer8Config },
    .Dir = {.Port = GPIOF, .Pin = GPIO_PIN_2},
    .Enable = {.Port = GPIOB, .Pin = GPIO_PIN_12},
};

const DRV882xPins Drive3 = 
{    
    .Step = {{.Port = GPIOC, .Pin = GPIO_PIN_8}, .timerConfig=Timer8Config },
    .Dir = {.Port = GPIOF, .Pin = GPIO_PIN_3},
    .Enable = {.Port = GPIOB, .Pin = GPIO_PIN_9},
};

const DRV882xPins Drive4 = 
{    
    .Step = {{.Port = GPIOC, .Pin = GPIO_PIN_9}, .timerConfig=Timer8Config },
    .Dir = {.Port = GPIOE, .Pin = GPIO_PIN_12},
    .Enable = {.Port = GPIOB, .Pin = GPIO_PIN_8},
};

const DRV882xPins Drive5 = 
{    
    .Step = {{.Port = GPIOF, .Pin = GPIO_PIN_6}, .timerConfig=Timer10Config },
    .Dir = {.Port = GPIOE, .Pin = GPIO_PIN_1},
    .Enable = {.Port = GPIOB, .Pin = GPIO_PIN_2},
};

const DRV882xPins Drive6 = 
{    
    .Step = {{.Port = GPIOF, .Pin = GPIO_PIN_7}, .timerConfig=Timer11Config },
    .Dir = {.Port = GPIOE, .Pin = GPIO_PIN_0},
    .Enable = {.Port = GPIOA, .Pin = GPIO_PIN_4},
};

const DRV8814Pins DCDriver1 = {
    .EnableA = {{.Port = GPIOA, .Pin = GPIO_PIN_8}, .timerConfig=Timer1Config },
    .DirA = {.Port = GPIOC, .Pin = GPIO_PIN_13},
    .EnableB = {{.Port = GPIOE, .Pin = GPIO_PIN_11}, .timerConfig=Timer1Config },
    .DirB = {.Port = GPIOC, .Pin = GPIO_PIN_14},
};

const DRV8814Pins DCDriver2 = {
    .EnableA = {{.Port = GPIOE, .Pin = GPIO_PIN_13}, .timerConfig=Timer1Config },
    .DirA = {.Port = GPIOE, .Pin = GPIO_PIN_2},
    .EnableB = {{.Port = GPIOE, .Pin = GPIO_PIN_14}, .timerConfig=Timer1Config },
    .DirB = {.Port = GPIOE, .Pin = GPIO_PIN_3},
};

const SignalPin DCPwm1 = { { GPIOA, GPIO_PIN_8}, Timer1Config};
const SignalPin DCPwm2 = { { GPIOE, GPIO_PIN_11}, Timer1Config};
const SignalPin ServoA = { { GPIOB, GPIO_PIN_8 }, Timer10Config };
const SignalPin ServoB = { { GPIOB, GPIO_PIN_9 }, Timer11Config };

const SignalPin PowerOutA = { { GPIOE, GPIO_PIN_5 }, Timer9Config };
const SignalPin PowerOutB = { { GPIOE, GPIO_PIN_6 }, Timer9Config };

const SignalPin Encoder1A = { { GPIOA, GPIO_PIN_15 }, Timer2Config };
const SignalPin Encoder1B = { { GPIOB, GPIO_PIN_3 }, Timer2Config };
const SignalPin Encoder2A = { { GPIOB, GPIO_PIN_4 }, Timer3Config };
const SignalPin Encoder2B = { { GPIOB, GPIO_PIN_5 }, Timer3Config };

const SignalPin Encoder3A = { { GPIOB, GPIO_PIN_6 }, Timer4Config };
const SignalPin Encoder3B = { { GPIOB, GPIO_PIN_7 }, Timer4Config };
const SignalPin Encoder4A = { { GPIOA, GPIO_PIN_0 }, Timer5Config };
const SignalPin Encoder4B = { { GPIOA, GPIO_PIN_1 }, Timer5Config };

const SignalPin FanA = { { GPIOB, GPIO_PIN_14 }, Timer12Config };
const SignalPin FanB = { { GPIOB, GPIO_PIN_15 }, Timer12Config };

// clang-format on

const PortPin LED1 = { GPIOG, GPIO_PIN_14 };
const PortPin LED2 = { GPIOF, GPIO_PIN_15 };
const PortPin LED3 = { GPIOF, GPIO_PIN_1 };

const PeripheralPin OneWire = { { GPIOC, GPIO_PIN_10 }, GPIO_AF8_UART4 };

const PeripheralPin Spi1Mosi = { { GPIOA, GPIO_PIN_7 }, GPIO_AF5_SPI1 };
const PeripheralPin Spi1Miso = { { GPIOA, GPIO_PIN_6 }, GPIO_AF5_SPI1 };
const PeripheralPin Spi1Clk = { { GPIOA, GPIO_PIN_5 }, GPIO_AF5_SPI1 };
const PeripheralPin Uart4Tx = { { GPIOC, GPIO_PIN_10 }, GPIO_AF8_UART4 };
const PeripheralPin Uart4Rx = { { GPIOC, GPIO_PIN_11 }, GPIO_AF8_UART4 };

const PortPin SolenoidInhibit = { GPIOA, GPIO_PIN_10 };
const PortPin SolenoidCS = { GPIOA, GPIO_PIN_9 };

const PortPin Relay = { GPIOC, GPIO_PIN_4 };
const PortPin RfidEnable = { GPIOF, GPIO_PIN_13 };
const PortPin RfidAntSel = { GPIOF, GPIO_PIN_14 };

const PortPin DisplayRs = { GPIOF, GPIO_PIN_0 };
const PortPin DisplayE = { GPIOD, GPIO_PIN_7 };
const PortPin DisplayRw = { GPIOD, GPIO_PIN_5 };
const PortPin DisplayDB0 = { GPIOD, GPIO_PIN_14 };
const PortPin DisplayDB1 = { GPIOD, GPIO_PIN_15 };
const PortPin DisplayDB2 = { GPIOD, GPIO_PIN_0 };
const PortPin DisplayDB3 = { GPIOD, GPIO_PIN_1 };
const PortPin DisplayDB4 = { GPIOE, GPIO_PIN_7 };
const PortPin DisplayDB5 = { GPIOE, GPIO_PIN_8 };
const PortPin DisplayDB6 = { GPIOE, GPIO_PIN_9 };
const PortPin DisplayDB7 = { GPIOE, GPIO_PIN_10 };

const PortPin SPICS1 = { GPIOF, GPIO_PIN_12 };
const PortPin SPICS2 = { GPIOA, GPIO_PIN_15 };
const PortPin SPICS3 = { GPIOA, GPIO_PIN_8 };
const PortPin SPICS4 = { GPIOC, GPIO_PIN_14 };

const AlternateFunctionPin SPI2_MOSI = { { GPIOB, GPIO_PIN_15 }, GPIO_AF5_SPI2 };

const AlternateFunctionPin SPI2_SCK = { { GPIOB, GPIO_PIN_13 }, GPIO_AF5_SPI2 };

const PortPin TempChannel1 = { GPIOB, GPIO_PIN_0 };
const PortPin TempChannel2 = { GPIOB, GPIO_PIN_1 };
const PortPin TempChannel3 = { GPIOC, GPIO_PIN_0 };
const PortPin TempChannel4 = { GPIOC, GPIO_PIN_1 };

const PortPin VrefSample = { GPIOA, GPIO_PIN_2 };

const PortPin OptoIn1 = { GPIOG, GPIO_PIN_0 }; // Hopper Feed
const PortPin OptoIn2 = { GPIOG, GPIO_PIN_1 }; // Card Entry
const PortPin OptoIn3 = { GPIOG, GPIO_PIN_2 }; // upper tape entered
const PortPin OptoIn4 = { GPIOG, GPIO_PIN_3 }; // upper tape exited
const PortPin OptoIn5 = { GPIOG, GPIO_PIN_4 }; // lower tape entered
const PortPin OptoIn6 = { GPIOG, GPIO_PIN_5 }; // lower tape exited
const PortPin OptoIn7 = { GPIOG, GPIO_PIN_6 }; 
const PortPin OptoIn8 = { GPIOG, GPIO_PIN_7 };
const PortPin OptoIn9 = { GPIOG, GPIO_PIN_8 };

const PortPin Button1 = { GPIOG, GPIO_PIN_9 };
const PortPin Button2 = { GPIOG, GPIO_PIN_10 };
const PortPin Button3 = { GPIOG, GPIO_PIN_11 };
const PortPin Button4 = { GPIOG, GPIO_PIN_12 };
const PortPin RfidIRQPin = { GPIOG, GPIO_PIN_13 };

const PortPin MuxAddr0 = { GPIOD, GPIO_PIN_8 };
const PortPin MuxAddr1 = { GPIOD, GPIO_PIN_9 };
const PortPin MuxAddr2 = { GPIOD, GPIO_PIN_10 };
const PortPin MuxAddr3 = { GPIOD, GPIO_PIN_11 };

const PortPin Speaker = { GPIOF, GPIO_PIN_10 };

#pragma mark Static Data


#pragma mark Static Functions


#pragma mark Function Implementations
