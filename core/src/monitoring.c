/**
 * @file    monitoring.c
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <FreeRTOS.h>

#include "monitoring.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);

static monitoringStatus_t InitMonitoringTimer(void);
static void StartMonitoringTimer(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     ulHighFrequencyTimerTicks
 * @brief   High Frequency tick used for runtime calculation
 */
static volatile unsigned long ulHighFrequencyTimerTicks;

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitMonitoring(void)
 * @brief   Enables TAPAS monitoring
 * @retval  #MONITORING_ERROR if cannot init timer for monitoring
 * @retval  #MONITORING_SUCCESSFUL else
 */
monitoringStatus_t InitMonitoring(void)
{
    // Variable Initialisation
    monitoringStatus_t return_value = MONITORING_SUCCESSFUL;

    // Function Core
    ulHighFrequencyTimerTicks = 0;
    return_value = InitMonitoringTimer();

    return return_value;
}

/**
 * @fn      configureTimerForRunTimeStats(void)
 * @brief   Configures runtime statistics variables
 */
void configureTimerForRunTimeStats(void)
{
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

#if defined(NUCLEO_H745ZI) || defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG) || defined(NUCLEO_F103RB)

/* STM32 HAL Specific variable or function declaration */
extern void TIM3_IRQHandler(void);
static TIM_HandleTypeDef monitoring_timer;

/**
 * @brief Monitoring Timer Initialization Function
 */
static monitoringStatus_t InitMonitoringTimer(void)
{
    // Variable Initialisation
    monitoringStatus_t return_value = MONITORING_SUCCESSFUL;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    // Function Core
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
            if (HAL_TIMEx_MasterConfigSynchronization(&monitoring_timer, &sMasterConfig) != HAL_OK)
            {
                return_value = MONITORING_ERROR;
            }
        }
        else
        {
            return_value = MONITORING_ERROR;
        }
    }
    else
    {
        return_value = MONITORING_ERROR;
    }

    return return_value;
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