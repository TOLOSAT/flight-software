/**
 * @file    cmsdk_timer.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CMSDK TIMER functions
 * @date    09/06/2024
 *
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

#ifndef CMSDK_TIMER_H
#define CMSDK_TIMER_H

/******************************* Include Files *******************************/

#include "cmsdk_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief  Timer mode
 */
typedef enum
{
    TIMER_ONESHOT = 0u,
    TIMER_PERIODIC = 1
} TIM_ModeTypeDef;

/**
 * @struct  TIM_HandleTypeDef
 * @brief   Struct type definition of a timer instance
 */
typedef struct
{
    CMSDK_TIMER_TypeDef *instance;
    HAL_LockTypeDef lock;
    TIM_ModeTypeDef mode;
    uint32_t reload;
    void (*callback)(void);
} TIM_HandleTypeDef;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

HAL_StatusTypeDef cmsdk_TimerInit(TIM_HandleTypeDef *tim);
HAL_StatusTypeDef cmsdk_TimerStart(TIM_HandleTypeDef *tim);
HAL_StatusTypeDef cmsdk_Timerstop(TIM_HandleTypeDef *tim);
HAL_StatusTypeDef cmsdk_TimerIrqHandler(TIM_HandleTypeDef *tim);

#endif /* CMSDK_TIMER_H */