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

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

#if defined(HAL_RTC_MODULE_ENABLED)
/**
 * @brief RTC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
    if (hrtc->Instance == RTC)
    {
        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            ErrorHandler();
        }

        /* Peripheral clock enable */
        __HAL_RCC_RTC_ENABLE();
    }
}

/**
 * @brief RTC MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
    if (hrtc->Instance == RTC)
    {
        /* Peripheral clock disable */
        __HAL_RCC_RTC_DISABLE();
    }
}
#endif /* HAL_RTC_MODULE_ENABLED */

#if defined(HAL_TIM_MODULE_ENABLED)
/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM3)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    else if (htim_base->Instance == TIM4)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM4_CLK_ENABLE();
    }
    else if (htim_base->Instance == TIM5)
    {
        /* Peripheral clock enable */
        __HAL_RCC_TIM5_CLK_ENABLE();
    }
}

/**
 * @brief TIM_Base MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_base: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();
    }
    else if (htim_base->Instance == TIM4)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM4_CLK_DISABLE();
    }
    else if (htim_base->Instance == TIM5)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM5_CLK_DISABLE();
    }
}
#endif /* HAL_TIM_MODULE_ENABLED */
