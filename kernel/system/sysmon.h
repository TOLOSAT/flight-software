/**
 * @file    sysmon.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system monitoring handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup sysmon System Monitoring
 * @brief Provides system monitoring handling interface.
 * @{
 */

#ifndef SYSMON_H
#define SYSMON_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern systemUsage_t g_system_usage;
extern taskUsage_t g_task_usages[CONFIG_MAX_NB_TASKS];

/*************************** Functions Declarations **************************/

extern void InitSYSMON(void);
extern void SYSMONMain(void);

#endif /* SYSMON_H */

/**
 * @}
 * @}
 * @}
 */