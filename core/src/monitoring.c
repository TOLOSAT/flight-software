/**
 * @file    monitoring.c
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "os.h"
#include "monitoring.h"
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);

/*************************** Variables Definitions ***************************/

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
    halStatus_t test_hal = InitMonitoringTimer();
    if (test_hal != GEN_HAL_SUCCESSFUL)
    {
        return_value = MONITORING_ERROR;
    }

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
 * @return  Current timer tick
 */
unsigned long getRunTimeCounterValue(void)
{
    return GetMonitoringTick();
}
