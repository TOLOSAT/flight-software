/**
 * @file    bsp.h
 * @author  Merlin Kooshmanian
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */
#ifndef BSP_H
#define BSP_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "stm32f4xx_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_REF           TIM4
#define HAL_TIMER_IRQ_NO        TIM4_IRQn
#define MONITORING_TIMER_REF    TIM3
#define MONITORING_TIMER_IRQ_NO TIM3_IRQn

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF            IWDG

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
