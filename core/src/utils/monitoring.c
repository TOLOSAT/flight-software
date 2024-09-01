/**
 * @file    monitoring.c
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"
#include "utils/monitoring.h"
#include "drv/drv_tim.h"

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
    return InitMonitoringTimer();
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
