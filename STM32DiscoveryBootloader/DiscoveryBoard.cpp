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

#define SECTOR_MASK ((uint32_t)0xFFFFFF07)


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

    // user_erase_flash (0, 0);
}

DiscoveryBoard::~DiscoveryBoard ()
{
}

void DiscoveryBoard::EraseFirmware ()
{
    HAL_FLASH_Unlock ();

    __HAL_FLASH_CLEAR_FLAG (FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
                            FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
                            FLASH_FLAG_PGSERR);

    FLASH_Erase_Sector (FLASH_SECTOR_3, FLASH_VOLTAGE_RANGE_3);

    FLASH_WaitForLastOperation (10000);

    FLASH_Erase_Sector (FLASH_SECTOR_4, FLASH_VOLTAGE_RANGE_3);

    FLASH_WaitForLastOperation (10000);

    FLASH_Erase_Sector (FLASH_SECTOR_5, FLASH_VOLTAGE_RANGE_3);

    FLASH_WaitForLastOperation (10000);

    FLASH_Erase_Sector (FLASH_SECTOR_6, FLASH_VOLTAGE_RANGE_3);
    FLASH_WaitForLastOperation (10000);

    // HAL_FLASH_Lock ();
}

void DiscoveryBoard::FlashData (uint32_t address, uint64_t data)
{
    uint32_t* ptr = (uint32_t*)&data;

    HAL_FLASH_Program (TYPEPROGRAM_FASTWORD, address, *ptr++);
    FLASH_WaitForLastOperation (10000);
    HAL_FLASH_Program (TYPEPROGRAM_FASTWORD, address, *ptr);
    FLASH_WaitForLastOperation (10000);
}

void DiscoveryBoard::FlashData (uint32_t address, uint32_t data)
{
    HAL_FLASH_Program (TYPEPROGRAM_FASTWORD, address, data);
    FLASH_WaitForLastOperation (10000);
}

void DiscoveryBoard::FlashData (uint32_t address, uint16_t data)
{
    HAL_FLASH_Program (TYPEPROGRAM_FASTHALFWORD, address, data);
    FLASH_WaitForLastOperation (10000);
}

void DiscoveryBoard::FlashData (uint32_t address, uint8_t data)
{
    HAL_FLASH_Unlock ();
    HAL_FLASH_Program (TYPEPROGRAM_FASTBYTE, address, data);
    FLASH_WaitForLastOperation (10000);
}

typedef void (*pFunction) (void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t ApplicationAddress;

void DiscoveryBoard::JumpToApplication ()
{
/* Check Vector Table: Test if user code is programmed starting from address

"APPLICATION_ADDRESS" */
#define APPLICATION_ADDRESS (0x0800C000)

    auto initsp = ((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000);


    if (((initsp & 3) == 0) && initsp == 0x20000000)
    // in regular RAM region 128KB

    {

        SCB->VTOR = FLASH_BASE | 0xC000;
        /* Jump to user application */

        JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);

        Jump_To_Application = (pFunction)JumpAddress;

        /* Initialize user application's Stack Pointer */

        __set_MSP (*(__IO uint32_t*)APPLICATION_ADDRESS);

        Jump_To_Application ();
    }
}
