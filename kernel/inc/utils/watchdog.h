/**
 * @file    watchdog.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for watchdog functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup watchdog Watchdog
 * @brief Provides watchdog handling.
 * @{
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitWatchdog(void);
extern void WatchdogMain(void);

#endif /* WATCHDOG_H */

/** 
 * @}
 * @}
 * @}
 */