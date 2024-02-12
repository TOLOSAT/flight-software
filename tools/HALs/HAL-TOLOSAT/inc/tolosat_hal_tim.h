/**
 * @file    tolosat_hal_tim.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL timers and ticks for HAL
 * @date    07/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TOLOSAT_HAL_TIM_H
#define TOLOSAT_HAL_TIM_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

HAL_StatusTypeDef HAL_InitTick(uint32_t TimPriority);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);

#endif /* TOLOSAT_HAL_TIM_H */