/**
 ******************************************************************************
 * @file         stm32h7xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/

#include "stm32h7xx_hal.h"

/* Private function prototypes -----------------------------------------------*/

extern void ErrorHandler(void);

/* External functions --------------------------------------------------------*/

#if defined(HAL_PCD_MODULE_ENABLED)
/**
 * @brief PCD MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hpcd: PCD handle pointer
 * @retval None
 */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
    GPIO_InitTypeDef GPIO_InitStruct             = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hpcd->Instance == USB_OTG_FS)
    {
        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
        PeriphClkInitStruct.PLL3.PLL3M           = 4;
        PeriphClkInitStruct.PLL3.PLL3N           = 96;
        PeriphClkInitStruct.PLL3.PLL3P           = 2;
        PeriphClkInitStruct.PLL3.PLL3Q           = 4;
        PeriphClkInitStruct.PLL3.PLL3R           = 2;
        PeriphClkInitStruct.PLL3.PLL3RGE         = RCC_PLL3VCIRANGE_1;
        PeriphClkInitStruct.PLL3.PLL3FRACN       = 0;
        PeriphClkInitStruct.UsbClockSelection    = RCC_USBCLKSOURCE_PLL3;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            ErrorHandler();
        }

        /** Enable USB Voltage detector
         */
        HAL_PWREx_EnableUSBVoltageDetector();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USB_OTG_FS GPIO Configuration
        PA9     ------> USB_OTG_FS_VBUS
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        GPIO_InitStruct.Pin  = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    }
}

/**
 * @brief PCD MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hpcd: PCD handle pointer
 * @retval None
 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
    if (hpcd->Instance == USB_OTG_FS)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USB_OTG_FS_CLK_DISABLE();

        /**USB_OTG_FS GPIO Configuration
        PA9     ------> USB_OTG_FS_VBUS
        PA11     ------> USB_OTG_FS_DM
        PA12     ------> USB_OTG_FS_DP
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12);
    }
}
#endif /* HAL_PCD_MODULE_ENABLED */

#if defined(HAL_NAND_MODULE_ENABLED)
static void HAL_FMC_MspInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct             = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.FmcClockSelection    = RCC_FMCCLKSOURCE_D1HCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ErrorHandler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_ENABLE();

    /** FMC GPIO Configuration
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PD11   ------> FMC_CLE
    PD12   ------> FMC_ALE
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG7   ------> FMC_INT
    PC8   ------> FMC_NCE
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    */
    GPIO_InitStruct.Pin       = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_8;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FMC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void HAL_NAND_MspInit(NAND_HandleTypeDef *hnand)
{
    (void)(hnand);
    HAL_FMC_MspInit();
}

static void HAL_FMC_MspDeInit(void)
{
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    /** FMC GPIO Configuration
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PD11   ------> FMC_CLE
    PD12   ------> FMC_ALE
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG7   ------> FMC_INT
    PC8   ------> FMC_NCE
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_7);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8);
}

void HAL_NAND_MspDeInit(NAND_HandleTypeDef *hnand)
{
    (void)(hnand);
    HAL_FMC_MspDeInit();
}
#endif /* HAL_NAND_MODULE_ENABLED */
