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

#define GENERIC_HAL_MAX_DELAY   30u     /**< Max delay (in micro seconds) for function that uses ST HAL timer */

/**
 * @def     HalDelay(delay)
 * @brief   This function does using active wait using the HAL main timer tick. 
 * @note    Redefinition of HAL_Delay().
 * @warning The use of this function can lead to missed deadline if used in a thread.
 */
#define HalDelay(delay)         HAL_Delay(delay)

/**
 * @def     HalGetTick()
 * @brief   This function returns the HAL main timer tick. 
 * @note    Redefinition of HAL_GetTick().
 * @warning The use of this function can lead to missed deadline if used in a thread.
 */
#define HalGetTick()            HAL_GetTick()

// Section placement macros
#define IN_TIM_TEXT_SECTION     __attribute__((section(".text_tim")))   /**< TIM functions goes in the .text_tim */
#define IN_TIM_DATA_SECTION     __attribute__((section(".data_tim")))   /**< TIM data goes in the .data_tim */

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef HAL_InitTick(uint32_t TimPriority);
extern void HAL_SuspendTick(void);
extern void HAL_ResumeTick(void);
extern halStatus_t InitMonitoringTimer(void);
extern void StartMonitoringTimer(void);
extern uint64_t GetMonitoringTick(void);

#endif /* GENERIC_HAL_TIM_H */