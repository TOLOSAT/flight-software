/**
 * @file    boot_misc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL used by boot
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "boot_misc.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Generic_IRQHandler(void);

extern HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
extern void HAL_SuspendTick(void);
extern void HAL_ResumeTick(void);
extern void TIM4_IRQHandler(void);

extern void OTG_FS_EP1_OUT_IRQHandler(void);
extern void OTG_FS_EP1_IN_IRQHandler(void);
extern void OTG_FS_IRQHandler(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     hal_tick_timer
 * @brief   Tick timer instance used for HAL delay and timing
 */
static timerInst_t hal_tick_timer;

/*************************** Functions Definitions ***************************/

/**
 * @brief  This function configures the TIM4 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    // Get clock configuration
    RCC_ClkInitTypeDef clkconfig = { 0 };
    uint32_t pFLatency           = 0u;
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    // Get APB1 prescaler, because ABP1 timers clock is either :
    // - Equal to APB1 peripheral clock if the prescaler equals 1
    // - Equal to 2 x APB1 peripheral clock if the prescaler is greater than 1
    uint32_t APB1_prescaler    = clkconfig.APB1CLKDivider;
    uint32_t APB1_timers_clock = 0u;
    if (APB1_prescaler == RCC_HCLK_DIV1)
    {
        // APB1 timers clock equals APB1 peripheral clock
        APB1_timers_clock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        // APB1 timers clock equals 2 x APB1 peripheral clock
        APB1_timers_clock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    // Compute the prescaler value to have timer counter clock equal to 1MHz (1us period)
    uint32_t hal_tick_timer_prescaler = (uint32_t)((APB1_timers_clock / 1000000U) - 1U);

    // Initialize HAL tick timer
    hal_tick_timer.Instance               = HAL_TIMER_REF;
    hal_tick_timer.Init.Period            = 999u; // 1000 us - 1, i.e. interrupt occurs every 1ms
    hal_tick_timer.Init.Prescaler         = hal_tick_timer_prescaler;
    hal_tick_timer.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    hal_tick_timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    hal_tick_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    // Then start HAL Timer
    HAL_StatusTypeDef status = HAL_TIM_Base_Init(&hal_tick_timer);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&hal_tick_timer);
        if (status == HAL_OK)
        {
            /* Enable the TIM4 global Interrupt */
            HAL_NVIC_EnableIRQ(HAL_TIMER_IRQ_NO);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority(HAL_TIMER_IRQ_NO, TickPriority, 0U);
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
 * @note   Disable the tick increment by disabling timer hal update interrupt.
 */
void HAL_SuspendTick(void)
{
    /* Disable timer HAL update Interrupt */
    __HAL_TIM_DISABLE_IT(&hal_tick_timer, TIM_IT_UPDATE);
    HAL_NVIC_DisableIRQ(TIM4_IRQn);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling timer hal update interrupt.
 */
void HAL_ResumeTick(void)
{
    /* Enable TIM HAL Update interrupt */
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
    __HAL_TIM_ENABLE_IT(&hal_tick_timer, TIM_IT_UPDATE);
}

/**
 * @brief This function handles TIM4 trigger and commutation interrupts and TIM4 global interrupt.
 */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&hal_tick_timer);
    HAL_IncTick();
}
