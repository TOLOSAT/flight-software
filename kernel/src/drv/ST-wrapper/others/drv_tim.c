/**
 * @file    drv_tim.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for timer and ticks for HAL
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/others/drv_tim.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define MONITORING_TIMER_IRQ_PRIO 3UL /**< Priority for monitoring timer interrupt */

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef HAL_InitTick(uint32_t TimPriority);
extern void HAL_SuspendTick(void);
extern void HAL_ResumeTick(void);

static void HalTickHandler(void *param);
static void MonitoringTickHandler(void *param);

/*************************** Variables Definitions ***************************/

/**
 * @var     hal_tick_timer
 * @brief   Tick timer instance used for HAL delay and timing
 */
static timerInst_t hal_tick_timer;

/**
 * @var     monitoring_timer
 * @brief   Monitoring timer instance used for FreeRTOS monitoring
 */
static timerInst_t monitoring_timer;

/**
 * @var     monitoring_tick
 * @brief   Tick for freertos monitoring
 */
static volatile uint64_t monitoring_tick;

/*************************** Functions Definitions ***************************/

/********************** HAL Timer Functions Definitions **********************/

/**
 * @brief  This function configures the HAL timer.
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
        /* Configure the HAL Tick IRQ */
        returnCode_t test_irq = RequestIRQ(HAL_TIMER_IRQ_NO, TickPriority, &HalTickHandler, NULL);
        if (test_irq == RET_SUCCESSFUL)
        {
            /* Start the TIM time Base generation in interrupt mode */
            status = HAL_TIM_Base_Start_IT(&hal_tick_timer);
            if (status == HAL_OK)
            {
                if (TickPriority < (1UL << __NVIC_PRIO_BITS))
                {
                    uwTickPrio = TickPriority;
                }
                else
                {
                    status = HAL_ERROR;
                }
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
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling timer hal update interrupt.
 */
void HAL_ResumeTick(void)
{
    /* Enable TIM HAL Update interrupt */
    __HAL_TIM_ENABLE_IT(&hal_tick_timer, TIM_IT_UPDATE);
}

/******************* Monitoring Timer Functions Definitions ******************/

/**
 * @brief Monitoring Timer Initialization Function
 */
returnCode_t InitMonitoringTimer(void)
{
    returnCode_t return_value                 = RET_SUCCESSFUL;
    TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
    TIM_MasterConfigTypeDef sMasterConfig     = { 0 };

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
    uint32_t monitoring_timer_prescaler = (uint32_t)((APB1_timers_clock / 1000000U) - 1U);

    // Set timer
    monitoring_tick                         = 0u;
    monitoring_timer.Instance               = MONITORING_TIMER_REF;
    monitoring_timer.Init.Prescaler         = monitoring_timer_prescaler;
    monitoring_timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
    monitoring_timer.Init.Period            = 10u; // 10 us
    monitoring_timer.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    monitoring_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&monitoring_timer) == HAL_OK)
    {
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&monitoring_timer, &sClockSourceConfig) == HAL_OK)
        {
            sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
            sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
            if (HAL_TIMEx_MasterConfigSynchronization(&monitoring_timer, &sMasterConfig) == HAL_OK)
            {
                // Setup Interrupt
                return_value = RequestIRQ(MONITORING_TIMER_IRQ_NO, MONITORING_TIMER_IRQ_PRIO, &MonitoringTickHandler, NULL);
            }
            else
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }

    return return_value;
}

/**
 * @brief This function start Monitoring Timer
 */
void StartMonitoringTimer(void)
{
    HAL_TIM_Base_Start_IT(&monitoring_timer);
}

/**
 * @brief This function get the current value of the monitoring tick
 */
uint64_t GetMonitoringTick(void)
{
    return monitoring_tick;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @brief This function is the monitoring tick timer interrupt handler
 */
static void MonitoringTickHandler(void *param)
{
    // Unused Parameter
    (void)(param);

    // Interrupt Core
    HAL_TIM_IRQHandler(&monitoring_timer);
    monitoring_tick++;
}

/**
 * @brief This function is the HAL tick timer interrupt handler
 */
static void HalTickHandler(void *param)
{
    // Unused Parameter
    (void)(param);

    // Interrupt Core
    HAL_TIM_IRQHandler(&hal_tick_timer);
    HAL_IncTick();
}
