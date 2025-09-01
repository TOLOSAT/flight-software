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

#if defined(HAL_QSPI_MODULE_ENABLED)
/**
 * @brief QSPI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hqspi: QSPI handle pointer
 * @retval None
 */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_InitStruct             = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hqspi->Instance == QUADSPI)
    {
        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
        PeriphClkInitStruct.QspiClockSelection   = RCC_QSPICLKSOURCE_D1HCLK;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            ErrorHandler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_QSPI_CLK_ENABLE();

        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        /**QUADSPI GPIO Configuration
        PG6     ------> QUADSPI_BK1_NCS
        PF6     ------> QUADSPI_BK1_IO3
        PF7     ------> QUADSPI_BK1_IO2
        PF8     ------> QUADSPI_BK1_IO0
        PF10     ------> QUADSPI_CLK
        PF9     ------> QUADSPI_BK1_IO1
        */
        GPIO_InitStruct.Pin       = GPIO_PIN_6;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    }
}

/**
 * @brief QSPI MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hqspi: QSPI handle pointer
 * @retval None
 */
void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef *hqspi)
{
    if (hqspi->Instance == QUADSPI)
    {
        /* Peripheral clock disable */
        __HAL_RCC_QSPI_CLK_DISABLE();

        /**QUADSPI GPIO Configuration
        PG6     ------> QUADSPI_BK1_NCS
        PF6     ------> QUADSPI_BK1_IO3
        PF7     ------> QUADSPI_BK1_IO2
        PF8     ------> QUADSPI_BK1_IO0
        PF10     ------> QUADSPI_CLK
        PF9     ------> QUADSPI_BK1_IO1
        */
        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6);

        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_10 | GPIO_PIN_9);
    }
}
#endif /* HAL_QSPI_MODULE_ENABLED */

#if defined(HAL_SDRAM_MODULE_ENABLED)
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
    PE1   ------> FMC_NBL1
    PE0   ------> FMC_NBL0
    PG15   ------> FMC_SDNCAS
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PG8   ------> FMC_SDCLK
    PF2   ------> FMC_A2
    PF1   ------> FMC_A1
    PF0   ------> FMC_A0
    PG5   ------> FMC_BA1
    PF3   ------> FMC_A3
    PG4   ------> FMC_BA0
    PG2   ------> FMC_A12
    PF5   ------> FMC_A5
    PF4   ------> FMC_A4
    PC2   ------> FMC_SDNE0
    PC3   ------> FMC_SDCKE0
    PE10   ------> FMC_D7
    PH5   ------> FMC_SDNWE
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PE9   ------> FMC_D6
    PE11   ------> FMC_D8
    PD15   ------> FMC_D1
    PD14   ------> FMC_D0
    PF12   ------> FMC_A6
    PF15   ------> FMC_A9
    PE12   ------> FMC_D9
    PE15   ------> FMC_D12
    PF11   ------> FMC_SDNRAS
    PG0   ------> FMC_A10
    PE8   ------> FMC_D5
    PE13   ------> FMC_D10
    PD10   ------> FMC_D15
    PD9   ------> FMC_D14
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE14   ------> FMC_D11
    PD8   ------> FMC_D13
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_13
                          | GPIO_PIN_7 | GPIO_PIN_14;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2 | GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_12
                          | GPIO_PIN_15 | GPIO_PIN_11;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = GPIO_PIN_5;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    (void)(hsdram);
    HAL_FMC_MspInit();
}

static void HAL_FMC_MspDeInit(void)
{
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    /** FMC GPIO Configuration
    PE1   ------> FMC_NBL1
    PE0   ------> FMC_NBL0
    PG15   ------> FMC_SDNCAS
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PG8   ------> FMC_SDCLK
    PF2   ------> FMC_A2
    PF1   ------> FMC_A1
    PF0   ------> FMC_A0
    PG5   ------> FMC_BA1
    PF3   ------> FMC_A3
    PG4   ------> FMC_BA0
    PG2   ------> FMC_A12
    PF5   ------> FMC_A5
    PF4   ------> FMC_A4
    PC2   ------> FMC_SDNE0
    PC3   ------> FMC_SDCKE0
    PE10   ------> FMC_D7
    PH5   ------> FMC_SDNWE
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PE9   ------> FMC_D6
    PE11   ------> FMC_D8
    PD15   ------> FMC_D1
    PD14   ------> FMC_D0
    PF12   ------> FMC_A6
    PF15   ------> FMC_A9
    PE12   ------> FMC_D9
    PE15   ------> FMC_D12
    PF11   ------> FMC_SDNRAS
    PG0   ------> FMC_A10
    PE8   ------> FMC_D5
    PE13   ------> FMC_D10
    PD10   ------> FMC_D15
    PD9   ------> FMC_D14
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE14   ------> FMC_D11
    PD8   ------> FMC_D13
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_13
                               | GPIO_PIN_7 | GPIO_PIN_14);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2 | GPIO_PIN_0 | GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_12
                               | GPIO_PIN_15 | GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2 | GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_5);
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram)
{
    (void)(hsdram);
    HAL_FMC_MspDeInit();
}
#endif /* HAL_SDRAM_MODULE_ENABLED */
