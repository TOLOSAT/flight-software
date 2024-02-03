/**
 * @file    monitoring.h
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup monitoring Monitoring
 * Functions that allows to monitor TAPAS
 * @{
 */

#ifndef MONITORING_H
#define MONITORING_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    monitoringStatus_t
 * @brief   Monitoring functions specific returns 
 */
typedef enum
{
    MONITORING_SUCCESSFUL       = 0u,    /**< Function succeed */
    MONITORING_ERROR            = 1u,    /**< Function failed */
    MONITORING_INVALID_PARAM    = 2u,    /**< Function parameter is not valid */
} monitoringStatus_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

monitoringStatus_t InitMonitoring(void);

#endif /* MONITORING_H */

/** 
 * @} 
 * @} 
 */