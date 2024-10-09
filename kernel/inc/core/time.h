/**
 * @file    time.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Time Management
 *
 * @copyright Copyright (c) TOLOSAT 2024
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

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t GetTime(time_t *time);
extern returnCode_t SetTime(time_t time);

#endif /* TIME_H */

/** 
 * @}
 * @}
 * @}
 */