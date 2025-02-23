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

/**
 * @def     HAL_TIM_SET_COUNTER
 * @brief   Set timer reload value
 */
#define HAL_TIM_SET_COUNTER(timer, value) (timer).instance->RELOAD = (value)

/**
 * @def     HAL_TIM_GET_COUNTER
 * @brief   Get timer counter value
 */
#define HAL_TIM_GET_COUNTER(timer)        (timer).instance->VALUE

/***************************** Types Definitions *****************************/

/**
 * @brief  Timer mode
 */
typedef enum
{
    TIMER_ONESHOT  = 0u, /**< @brief Timer in oneshot mode */
    TIMER_PERIODIC = 1u, /**< @brief Timer in periodic mode */
} TIM_ModeTypeDef;

/**
 * @struct  TIM_HandleTypeDef
 * @brief   Struct type definition of a timer instance
 */
typedef struct
{
    CMSDK_TIMER_TypeDef *instance; /**< @brief Timer instance */
    TIM_ModeTypeDef mode;          /**< @brief Timer mode */
    uint32_t reload;               /**< @brief Timer reload value */
    void (*callback)(void);        /**< @brief Timer IRQ callback */
} TIM_HandleTypeDef;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef cmsdk_TimerInit(TIM_HandleTypeDef *tim);
extern HAL_StatusTypeDef cmsdk_TimerStart(TIM_HandleTypeDef *tim);
extern HAL_StatusTypeDef cmsdk_TimerStop(TIM_HandleTypeDef *tim);
extern HAL_StatusTypeDef cmsdk_TimerIrqHandler(TIM_HandleTypeDef *tim);

#endif /* CMSDK_TIMER_H */