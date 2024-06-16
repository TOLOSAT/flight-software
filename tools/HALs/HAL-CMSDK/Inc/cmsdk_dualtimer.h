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
    DUALTIMER_TIMER_NONE = 0u,
    DUALTIMER_TIMER_1 = 1u,
    DUALTIMER_TIMER_2 = 2u,
    DUALTIMER_TIMER_BOTH = 3u,
} DUALTIM_TimerSelTypeDef;

/**
 * @brief  Dualtimer mode
 */
typedef enum
{
    DUALTIMER_DISABLED = 0u,
    DUALTIMER_ONESHOT = 1u,
    DUALTIMER_PERIODIC = 2u,
    DUALTIMER_FREERUNNING= 3u,
} DUALTIM_ModeTypeDef;

/**
 * @brief  Dualtimer mode
 */
typedef enum
{
    DUALTIMER_PRESCALER_1 = 0u,
    DUALTIMER_PRESCALER_16 = 1u,
    DUALTIMER_PRESCALER_256 = 2u,
} DUALTIM_PreScalerTypeDef;

/**
 * @brief  Dualtimer Size
 */
typedef enum
{
    DUALTIMER_16_BITS = 0u,
    DUALTIMER_32_BITS = 1u,
} DUALTIM_SizeTypeDef;

/**
 * @struct  DUALTIM_HandleTypeDef
 * @brief   Struct type definition of a dualtimer instance
 */
typedef struct
{
    CMSDK_DUALTIMER_BOTH_TypeDef *instance;
    DUALTIM_ModeTypeDef mode_1;
    DUALTIM_PreScalerTypeDef prescaler_1;
    DUALTIM_SizeTypeDef size_1;
    uint32_t reload_1;
    DUALTIM_ModeTypeDef mode_2;
    DUALTIM_PreScalerTypeDef prescaler_2;
    DUALTIM_SizeTypeDef size_2;
    uint32_t reload_2;
    void (*callback)(DUALTIM_TimerSelTypeDef sel);
} DUALTIM_HandleTypeDef;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

HAL_StatusTypeDef cmsdk_DualTimerInit(DUALTIM_HandleTypeDef *dualtim);
HAL_StatusTypeDef cmsdk_DualTimerStart(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel);
HAL_StatusTypeDef cmsdk_DualTimerStop(DUALTIM_HandleTypeDef *dualtim, DUALTIM_TimerSelTypeDef sel);
HAL_StatusTypeDef cmsdk_DualTimerIrqHandler(DUALTIM_HandleTypeDef *dualtim);

#endif /* CMSDK_DUALTIMER_H */