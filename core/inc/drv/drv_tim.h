/**
 * @file    drv_tim.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for timers and ticks for HAL
 * @date    07/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
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

extern void HalDelay(uint32_t delay);
extern uint32_t HalGetTick(void);

extern coreStatus_t InitMonitoringTimer(void);
extern void StartMonitoringTimer(void);
extern uint64_t GetMonitoringTick(void);

#endif /* DRV_TIM_H */