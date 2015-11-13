/******************************************************************************
*       Description:
*
*       Author:
*         Date: 10 November 2015
*
*******************************************************************************/
#pragma mark Compiler Pragmas


#pragma mark Includes
#include "DiscoveryBoard.h"
#include "stm32f4xx_hal.h"
#include "STM32UsbHidDevice.h"
#include "STM32BootloaderService.h"

#pragma mark Definitions and Constants


#pragma mark Static Data


#pragma mark Static Functions
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

    RCC_ClkInitStruct.ClockType =
    RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


#pragma mark Member Implementations
DiscoveryBoard::DiscoveryBoard ()
{
    HAL_Init ();

    SystemClock_Config ();

    __GPIOA_CLK_ENABLE ();

    static STM32UsbHidDevice device;
    hidDevice = &device;

    dispatcherActions = new DispatcherActions ();

    dispatcherActions->EnterCriticalSection = []
    {
        __disable_irq ();
    };

    dispatcherActions->ExitCriticalSection = []
    {
        __enable_irq ();
    };

    BootloaderService = new STM32BootloaderService ();
}

DiscoveryBoard::~DiscoveryBoard ()
{
}
