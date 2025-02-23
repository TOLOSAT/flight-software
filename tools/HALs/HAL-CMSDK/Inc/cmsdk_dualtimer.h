/**
 * @file    cmsdk_dualtimer.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CMSDK DUALTIMER functions
 * @date    16/06/2024
 *
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

#ifndef CMSDK_DUALTIMER_H
#define CMSDK_DUALTIMER_H

/******************************* Include Files *******************************/

#include "cmsdk_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief  Dualtimer timer selection
 */
typedef enum
{
    DUALTIMER_TIMER_NONE = 0u, /**< @brief No timer selected */
    DUALTIMER_TIMER_1    = 1u, /**< @brief Timer 1 selected */
    DUALTIMER_TIMER_2    = 2u, /**< @brief Timer 2 selected */
    DUALTIMER_TIMER_BOTH = 3u, /**< @brief Both timers selected */
} DUALTIM_TimerSelTypeDef;

/**
 * @brief  Dualtimer mode
 */
typedef enum
{
    DUALTIMER_DISABLED    = 0u, /**< @brief Timer disabled */
    DUALTIMER_ONESHOT     = 1u, /**< @brief Timer in oneshot mode */
    DUALTIMER_PERIODIC    = 2u, /**< @brief Timer in periodic mode */
    DUALTIMER_FREERUNNING = 3u, /**< @brief Timer in freerun mode */
} DUALTIM_ModeTypeDef;

/**
 * @brief  Dualtimer mode
 */
typedef enum
{
    DUALTIMER_PRESCALER_1   = 0u, /**< @brief Timer prescaler value is 1 */
    DUALTIMER_PRESCALER_16  = 1u, /**< @brief Timer prescaler value is 16 */
    DUALTIMER_PRESCALER_256 = 2u, /**< @brief Timer prescaler value is 256 */
} DUALTIM_PreScalerTypeDef;

/**
 * @brief  Dualtimer Size
 */
typedef enum
{
    DUALTIMER_16_BITS = 0u, /**< @brief Timer on 16 bits */
    DUALTIMER_32_BITS = 1u, /**< @brief Timer on 32 bits */
} DUALTIM_SizeTypeDef;

/**
 * @struct  DUALTIM_HandleTypeDef
 * @brief   Struct type definition of a dualtimer instance
 */
typedef struct
{
    CMSDK_DUALTIMER_BOTH_TypeDef *instance;        /**< @brief Dual timer instance */
    DUALTIM_ModeTypeDef mode_1;                    /**< @brief Timer 1 mode */
    DUALTIM_PreScalerTypeDef prescaler_1;          /**< @brief Timer 1 prescaler */
    DUALTIM_SizeTypeDef size_1;                    /**< @brief Timer 1 size */
    uint32_t reload_1;                             /**< @brief Timer 1 reload value */
    DUALTIM_ModeTypeDef mode_2;                    /**< @brief Timer 2 mode */
    DUALTIM_PreScalerTypeDef prescaler_2;          /**< @brief Timer 2 prescaler */
    DUALTIM_SizeTypeDef size_2;                    /**< @brief Timer 2 size */
    uint32_t reload_2;                             /**< @brief Timer 2 reload value */
    void (*callback)(DUALTIM_TimerSelTypeDef sel); /**< @brief Dual Timer IRQ callback */
} DUALTIM_HandleTypeDef;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef cmsdk_DualTimerInit(DUALTIM_HandleTypeDef *dualtim);
extern HAL_StatusTypeDef cmsdk_DualTimerStart(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel);
extern HAL_StatusTypeDef cmsdk_DualTimerStop(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel);
extern void cmsdk_DualTimerIrqHandler(DUALTIM_HandleTypeDef *dualtim);

#endif /* CMSDK_DUALTIMER_H */