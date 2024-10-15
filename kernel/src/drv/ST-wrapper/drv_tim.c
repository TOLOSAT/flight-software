/**
 * @file    drv_tim.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for timer and ticks for HAL
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_tim.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

#define MONITORING_TIMER_IRQ_PRIO   3UL    /**< Priority for monitoring timer interrupt */

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
        /* Configure the HAL Tick IRQ */
        returnCode_t test_irq = RequestIRQ(TIM4_IRQn, TickPriority, &HalTickHandler, NULL);
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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    // Enable TIM3 clock
    __HAL_RCC_TIM3_CLK_ENABLE();

    // Function Core
    monitoring_tick = 0u;
    monitoring_timer.Instance = TIM3;
    monitoring_timer.Init.Prescaler = 0;
    monitoring_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    monitoring_timer.Init.Period = 1000;
    monitoring_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    monitoring_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&monitoring_timer) == HAL_OK)
    {
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        if (HAL_TIM_ConfigClockSource(&monitoring_timer, &sClockSourceConfig) == HAL_OK)
        {
            sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
            sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
            if (HAL_TIMEx_MasterConfigSynchronization(&monitoring_timer, &sMasterConfig) == HAL_OK)
            {
                // Setup Interrupt
                return_value = RequestIRQ(TIM3_IRQn, MONITORING_TIMER_IRQ_PRIO, &MonitoringTickHandler, NULL);
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_ERROR;
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
