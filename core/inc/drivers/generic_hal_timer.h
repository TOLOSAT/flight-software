/**
 * @file    generic_hal_timer.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL timers and ticks for HAL
 * @date    07/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GENERIC_HAL_TIMER_H
#define GENERIC_HAL_TIMER_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

#define GENERIC_HAL_MAX_DELAY   30u /**< Max delay (in milliseconds) for function that uses HAL timer */

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void HalDelay(uint32_t delay);
extern uint32_t HalGetTick(void);

extern halStatus_t InitMonitoringTimer(void);
extern void StartMonitoringTimer(void);
extern uint64_t GetMonitoringTick(void);

#endif /* GENERIC_HAL_TIMER_H */