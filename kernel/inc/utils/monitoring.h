/**
 * @file    monitoring.h
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
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

extern returnCode_t InitMonitoring(void);
extern returnCode_t UpdateSystemUsage(void);
extern void SystemMonitoringMain(void);

#endif /* MONITORING_H */

/** 
 * @}
 * @}
 * @}
 */