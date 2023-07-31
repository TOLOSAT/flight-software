/**
 * @file    tolosat_hal_tick.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL Tick for HAL
 * @date    07/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TOLOSAT_HAL_TICK_H
#define TOLOSAT_HAL_TICK_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);

#endif /* TOLOSAT_HAL_TICK_H */