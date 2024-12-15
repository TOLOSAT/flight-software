/**
 * @file    sysusage.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system usage handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup sysusage System Usage
 * @brief Provides system usage handling interface.
 * @{
 */

#ifndef SYSUSAGE_H
#define SYSUSAGE_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern systemUsage_t g_system_usage;

/*************************** Functions Declarations **************************/

extern returnCode_t InitMonitoring(void);
extern returnCode_t UpdateSystemUsage(void);
extern void SystemMonitoringMain(void);

#endif /* SYSUSAGE_H */

/**
 * @}
 * @}
 * @}
 */