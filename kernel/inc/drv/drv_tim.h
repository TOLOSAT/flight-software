/**
 * @file    drv_tim.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for timers and ticks for HAL
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_tim Timers Driver
 * @brief Abstraction layer for internal timers.
 * @{
 */

#ifndef DRV_TIM_H
#define DRV_TIM_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitMonitoringTimer(void);
extern void StartMonitoringTimer(void);
extern uint64_t GetMonitoringTick(void);

#endif /* DRV_TIM_H */

/**
 * @}
 * @}
 * @}
 */