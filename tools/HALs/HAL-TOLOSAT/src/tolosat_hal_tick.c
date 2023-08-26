/**
 * @file    tolosat_hal_tick.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL Tick for HAL
 * @date    29/04/2023
 *
 * This file is base on the stm32_hal_timebase_tim_template.c
 * and was generated with ST wizards
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal_tick.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void TIM4_IRQHandler(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

/*************************** Variables Definitions ***************************/

/**
 * @var     htim4
 * @brief   HAL Timer 4 instance declaration (timer used for HAL tick)
 */
static TIM_HandleTypeDef htim4;

/*************************** Functions Definitions ***************************/

/**
 * @brief  This function configures the TIM11 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock = 0U;
    uint32_t uwAPB1Prescaler = 0U;

    uint32_t uwPrescalerValue = 0U;
    uint32_t pFLatency;
    HAL_StatusTypeDef status;

    /* Enable TIM4 clock */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    /* Compute TIM4 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
    {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM4 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM4 */
    htim4.Instance = TIM4;

    /* Initialize TIMx peripheral as follow:

    + Period = [(TIM4CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim4.Init.Period = (1000000U / 1000U) - 1U;
    htim4.Init.Prescaler = uwPrescalerValue;
    htim4.Init.ClockDivision = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&htim4);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&htim4);
        if (status == HAL_OK)
        {
            /* Enable the TIM4 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM4_IRQn);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority(TIM4_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            }
            else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disable the tick increment by disabling TIM11 update interrupt.
 */
void HAL_SuspendTick(void)
{
    /* Disable TIM11 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling TIM11 update interrupt.
 */
void HAL_ResumeTick(void)
{
    /* Enable TIM11 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
}

/**
 * @brief This function handles TIM1 trigger and commutation interrupts and TIM11 global interrupt.
 */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim4);
}

/**
 * @brief HAL Timer(s) Callback Function
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        HAL_IncTick();
    }
}