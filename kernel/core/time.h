/**
 * @file    time.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Time Management
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup time Time
 * @brief Time handling interface.
 * @{
 */

#ifndef TIME_H
#define TIME_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern tick_t GetTick(void);
extern void Sleep(tick_t tick);
extern void SleepPeriodic(void);
extern time_t GetTime(void);
extern returnCode_t SetTime(time_t time);

#endif /* TIME_H */

/**
 * @}
 * @}
 * @}
 */