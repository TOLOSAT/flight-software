/**
 * @file    monitoring.c
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "monitoring.h"
#include "core.h"
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
 * @retval  #CORE_ERROR if cannot init timer for monitoring
 * @retval  #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION InitMonitoring(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    halStatus_t test_hal = InitMonitoringTimer();
    if (test_hal != GEN_HAL_SUCCESSFUL)
    {
        return_value = CORE_ERROR;
    }

    return return_value;
}

/**
 * @fn      configureTimerForRunTimeStats(void)
 * @brief   Configures runtime statistics variables
 */
void IN_CORE_TEXT_SECTION configureTimerForRunTimeStats(void)
{
    StartMonitoringTimer();
}

/**
 * @fn      getRunTimeCounterValue(void)
 * @brief   Increment runtime counter
 * @return  Current timer tick
 */
unsigned long IN_CORE_TEXT_SECTION getRunTimeCounterValue(void)
{
    return GetMonitoringTick();
}
