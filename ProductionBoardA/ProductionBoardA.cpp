#include "Kernel.h"
#include "PinOut.h"
#include "ProductionBoardA.h"
#include "STM32BootloaderService.h"
#include "STM32GpioPin.h"
#include "STM32SerialPort.h"
#include "STM32UsbHidDevice.h"
#include "Thread.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>

#ifdef __IAR_SYSTEMS_ICC__
#define UINT16_MAX (0xFFFF)
#endif

extern "C" {
void HAL_Delay (uint32_t timeMs)
{
    Thread::Sleep (timeMs);
}
uint32_t HAL_GetTick (void)
{
    return Kernel::GetSystemTime ();
}
}

static const PortPin OutputPins[] = { LED1, LED2, LED3 };

static const PortPin InputPins[] = { Button1, Button2, Button3, Button4 };

static void SystemClock_Config (void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    __PWR_CLK_ENABLE ();

    __HAL_PWR_VOLTAGESCALING_CONFIG (PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig (&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

static void InitialisePin (GPIO_InitTypeDef& init, const PortPin& portPin)
{
    init.Pin = portPin.Pin;

    HAL_GPIO_DeInit (portPin.Port, portPin.Pin);
    HAL_GPIO_Init (portPin.Port, &init);
}

static void InitialiseAnalogInputPin (const PortPin& portPin)
{
    GPIO_InitTypeDef init;
    memset (&init, 0, sizeof (GPIO_InitTypeDef));

    init.Mode = GPIO_MODE_ANALOG;
    init.Pull = GPIO_NOPULL;

    InitialisePin (init, portPin);
}

static void InitialiseInputPin (const PortPin& portPin)
{
    GPIO_InitTypeDef init;
    memset (&init, 0, sizeof (GPIO_InitTypeDef));

    init.Mode = GPIO_MODE_INPUT;
    init.Pull = GPIO_NOPULL;

    InitialisePin (init, portPin);
}

static void InitialiseOutputPin (const PortPin& portPin)
{
    GPIO_InitTypeDef init;
    memset (&init, 0, sizeof (GPIO_InitTypeDef));

    init.Mode = GPIO_MODE_OUTPUT_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_LOW;

    InitialisePin (init, portPin);
}

static void ResetPins ()
{
    PortPin pins = { GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
                            GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                            GPIO_PIN_15 };

    InitialiseAnalogInputPin (pins);

    pins.Pin = GPIO_PIN_All;

    pins.Port = GPIOB;
    InitialiseAnalogInputPin (pins);

    pins.Port = GPIOC;
    InitialiseAnalogInputPin (pins);

    pins.Port = GPIOD;
    InitialiseAnalogInputPin (pins);

    pins.Port = GPIOE;
    InitialiseAnalogInputPin (pins);

    pins.Port = GPIOF;
    InitialiseAnalogInputPin (pins);

    pins.Port = GPIOH;
    InitialiseAnalogInputPin (pins);
}

static void InitialiseOutputPins ()
{
    for (uint32_t i = 0; i < (sizeof (OutputPins) / sizeof (PortPin)); i++)
    {
        InitialiseOutputPin (OutputPins[i]);
    }
}

static void InitialiseInputPins ()
{
    for (uint32_t i = 0; i < (sizeof (InputPins) / sizeof (PortPin)); i++)
    {
        InitialiseInputPin (InputPins[i]);
    }
}

static void InitialiseGPIO (void)
{
    /* GPIO Ports Clock Enable */
    __GPIOA_CLK_ENABLE ();
    __GPIOB_CLK_ENABLE ();
    __GPIOC_CLK_ENABLE ();
    __GPIOD_CLK_ENABLE ();
    __GPIOE_CLK_ENABLE ();
    __GPIOF_CLK_ENABLE ();
    __GPIOG_CLK_ENABLE ();
    __GPIOH_CLK_ENABLE ();


    // Make All pins analog inputs.
    ResetPins ();

    InitialiseOutputPins ();

    InitialiseInputPins ();
}


ProductionBoardA::ProductionBoardA ()
{
    HAL_RCC_DeInit ();

    SystemInit ();

    HAL_Init ();

    SystemClock_Config ();

    SystemCoreClockUpdate ();

    BoardDispatcherActions.EnterCriticalSection = Action::Create (ProductionBoardA::DisableInterrupts);
    BoardDispatcherActions.ExitCriticalSection = Action::Create (ProductionBoardA::EnableInterrupts);
}

void ProductionBoardA::Initialise ()
{
    InitialiseGPIO ();

    hidDevice = new STM32UsbHidDevice ();

    BootloaderService = new STM32BootloaderService ();
}

void ProductionBoardA::DisableInterrupts ()
{
    __disable_irq ();
}

void ProductionBoardA::EnableInterrupts ()
{
    __enable_irq ();
}

bool ProductionBoardA::ForceBootloadRequested ()
{
    bool button1Held = HAL_GPIO_ReadPin (Button1.Port, Button1.Pin) == GPIO_PIN_RESET;
    bool button2Held = HAL_GPIO_ReadPin (Button2.Port, Button2.Pin) == GPIO_PIN_RESET;
    bool button3Held = HAL_GPIO_ReadPin (Button3.Port, Button3.Pin) == GPIO_PIN_RESET;
    bool button4Held = HAL_GPIO_ReadPin (Button4.Port, Button4.Pin) == GPIO_PIN_RESET;

    while (!button1Held && button2Held && button3Held && !button4Held && Kernel::GetSystemTime () < 5000)
    {
        button1Held = HAL_GPIO_ReadPin (Button1.Port, Button1.Pin) == GPIO_PIN_RESET;
        button2Held = HAL_GPIO_ReadPin (Button2.Port, Button2.Pin) == GPIO_PIN_RESET;
        button3Held = HAL_GPIO_ReadPin (Button3.Port, Button3.Pin) == GPIO_PIN_RESET;
        button4Held = HAL_GPIO_ReadPin (Button4.Port, Button4.Pin) == GPIO_PIN_RESET;
    }

    if (!(!button1Held && button2Held && button3Held && !button4Held))
    {
        return false;
    }

    Thread::Sleep (1000);

    button1Held = HAL_GPIO_ReadPin (Button1.Port, Button1.Pin) == GPIO_PIN_RESET;
    button2Held = HAL_GPIO_ReadPin (Button2.Port, Button2.Pin) == GPIO_PIN_RESET;
    button3Held = HAL_GPIO_ReadPin (Button3.Port, Button3.Pin) == GPIO_PIN_RESET;
    button4Held = HAL_GPIO_ReadPin (Button4.Port, Button4.Pin) == GPIO_PIN_RESET;

    while (!button1Held && !button2Held && button3Held && !button4Held && Kernel::GetSystemTime () < 11000)
    {
        button1Held = HAL_GPIO_ReadPin (Button1.Port, Button1.Pin) == GPIO_PIN_RESET;
        button2Held = HAL_GPIO_ReadPin (Button2.Port, Button2.Pin) == GPIO_PIN_RESET;
        button3Held = HAL_GPIO_ReadPin (Button3.Port, Button3.Pin) == GPIO_PIN_RESET;
        button4Held = HAL_GPIO_ReadPin (Button4.Port, Button4.Pin) == GPIO_PIN_RESET;
    }

    if (!(!button1Held && !button2Held && button3Held && !button4Held))
    {
        return false;
    }

    return true;
}

/*void ProductionBoardA::SetLED1 ()
{
    HAL_GPIO_WritePin (LED1.Port, LED1.Pin, GPIO_PIN_SET);
}

void ProductionBoardA::ClearLED1 ()
{
    HAL_GPIO_WritePin (LED1.Port, LED1.Pin, GPIO_PIN_RESET);
}*/
