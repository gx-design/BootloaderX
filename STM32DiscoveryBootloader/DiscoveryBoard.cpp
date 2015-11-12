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
bool user_erase_flash (uint32_t Start_Addr, uint32_t End_Addr)
{
    FLASH_EraseInitTypeDef EraseInitStruct;

    auto flash_sr = FLASH->SR;
    auto flash_cr = FLASH->CR;

    HAL_FLASH_Unlock ();

    // flash_sr = FLASH->SR;
    // flash_cr = FLASH->CR;


    //__HAL_FLASH_CLEAR_FLAG (FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
    //                        FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR |
    //                        FLASH_FLAG_PGSERR);

    // flash_sr = FLASH->SR;
    // flash_cr = FLASH->CR;

    ///* If the previous operation is completed, proceed to erase the sector */
     FLASH->CR &= CR_PSIZE_MASK;
     FLASH->CR |= 512;
     FLASH->CR &= SECTOR_MASK;
     FLASH->CR |= FLASH_CR_SER | (FLASH_SECTOR_5 << POSITION_VAL
     (FLASH_CR_SNB));
     FLASH->CR |= FLASH_CR_STRT;

    FLASH_Erase_Sector (FLASH_SECTOR_5, FLASH_VOLTAGE_RANGE_3);
    FLASH_WaitForLastOperation (10000);
    FLASH_Erase_Sector (FLASH_SECTOR_6, FLASH_VOLTAGE_RANGE_3);
    FLASH_WaitForLastOperation (10000);

    HAL_FLASH_Program (TYPEPROGRAM_BYTE, 0x08010000, 0xAA);

    flash_sr = FLASH->SR;
    flash_cr = FLASH->CR;

    uint32_t* ptr = (uint32_t*)0x08010000;

    uint32_t value = *ptr;

    if (value == 0xFFFFFFFF)
    {
        return true;
    }
    else
    {
        return false;
    }
}


#pragma mark Member Implementations
DiscoveryBoard::DiscoveryBoard ()
{
    HAL_Init ();

    SystemClock_Config ();

    __GPIOA_CLK_ENABLE ();

    static STM32UsbHidDevice device;
    hidDevice = &device;

    //user_erase_flash (0, 0);
}

DiscoveryBoard::~DiscoveryBoard ()
{
}
