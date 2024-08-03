/**
 * @file    boot_misc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL used by boot
 * @date    25/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
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
    hal_tick_timer.Instance = TIM4;

    /* Initialize TIMx peripheral as follow:

    + Period = [(TIM4CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    hal_tick_timer.Init.Period = (1000000U / 1000U) - 1U;
    hal_tick_timer.Init.Prescaler = uwPrescalerValue;
    hal_tick_timer.Init.ClockDivision = 0;
    hal_tick_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    hal_tick_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&hal_tick_timer);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&hal_tick_timer);
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

/*************************** IRQ Handler Definition **************************/

/**
 * @brief Generic IRQ Handler
 * 
 * We keep the Generic IRQ Handler for futur  
 * improvements even if it seems overkill atm.
 */
void Generic_IRQHandler(void)
{
    // First get the IPSR that indicates which interrupts has been triggered
    int32_t ipsr = __get_IPSR();
    IRQn_Type irq_no = (IRQn_Type)(ipsr - 16);
    
    // Check if the interrupt is the timer interrupt
    if (irq_no == TIM4_IRQn) 
    {
        TIM4_IRQHandler();
    }
    else
    {
        // Error : shouldn't be here
        __disable_irq();
        while (1)
        {
            // Wait until the watchdog kills us
        }
    }
}
