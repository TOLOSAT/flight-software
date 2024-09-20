/**
 * @file    monitoring.h
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup monitoring Monitoring
 * @brief Provides software monitoring handling interface.
 * @{
 */

#ifndef MONITORING_H
#define MONITORING_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern kernelStatus_t InitMonitoring(void);
extern kernelStatus_t GetSystemUsage(monitoringSystemUsage_t *system_usage);

#endif /* MONITORING_H */

/** 
 * @}
 * @}
 * @}
 */