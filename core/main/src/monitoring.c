/**
 * @file monitoring.c
 * @author Merlin Kooshmanian
 * @brief Monitoring functions
 * @date 31/05/2023
 *
 * Last Update : 31/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <cmsis_os2.h>
#include <FreeRTOS.h>

#include "monitoring.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

static void InitMonitoringTimer(void);
static void StartMonitoringTimer(void);
extern void Error_Handler(void);

/************************** Variable Definitions *****************************/

/**
 * @var     ulHighFrequencyTimerTicks
 * @brief   High Frequency tick used for runtime calculation
 */
volatile unsigned long ulHighFrequencyTimerTicks;

/************************* Functions Definitions *****************************/

/**
 * @fn      InitMonitoring(void)
 * @brief   Enables TAPAS monitoring
 */
void InitMonitoring(void)
{
    InitMonitoringTimer();
}

/**
 * @fn      configureTimerForRunTimeStats(void)
 * @brief   Configures runtime statistics variables
 */
void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0;
    StartMonitoringTimer();
}

/**
 * @fn      getRunTimeCounterValue(void)
 * @brief   Increment runtime counter
 */
unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTicks;
}

#if defined(STM32F411xE) || defined(STM32F103xB) || defined(STM32H745xx)
/**
 * @var     monitoring_timer
 * @brief   Timer instance used for the high frequency tick
 */
TIM_HandleTypeDef monitoring_timer;

/**
 * @brief Monitoring Timer Initialization Function
 */
static void InitMonitoringTimer(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    monitoring_timer.Instance = TIM3;
    monitoring_timer.Init.Prescaler = 0;
    monitoring_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    monitoring_timer.Init.Period = 1000;
    monitoring_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    monitoring_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&monitoring_timer) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&monitoring_timer, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&monitoring_timer, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief This function start Monitoring Timer
 */
static void StartMonitoringTimer(void)
{
    HAL_TIM_Base_Start_IT(&monitoring_timer);
}

/**
 * @brief This function handles TIM3 global interrupt.
 */
void TIM3_IRQHandler(void)
{
    // Needed for freertos stats
    ulHighFrequencyTimerTicks++;
    HAL_TIM_IRQHandler(&monitoring_timer);
}
#else 
#error "Board is not supported"
#endif