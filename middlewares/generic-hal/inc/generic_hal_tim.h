/**
 * @file    generic_hal_tim.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL timers and ticks for HAL
 * @date    07/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GENERIC_HAL_TIM_H
#define GENERIC_HAL_TIM_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

#define GENERIC_HAL_MAX_DELAY   30u /**< Max delay (in micro seconds) for function that uses HAL timer */

// Section placement macros
#define IN_TIM_TEXT_SECTION     __attribute__((section(".text_tim")))   /**< TIM functions goes in the .text_tim */
#define IN_TIM_DATA_SECTION     __attribute__((section(".data_tim")))   /**< TIM data goes in the .data_tim */

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

#endif /* GENERIC_HAL_TIM_H */