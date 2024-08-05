/**
 * @file    time.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TIME_H
#define TIME_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "core_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t GetTime(time_t *time);
extern coreStatus_t SetTime(time_t *time);

#endif /* TIME_H */
