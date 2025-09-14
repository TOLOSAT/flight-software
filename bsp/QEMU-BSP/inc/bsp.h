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
#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_REF           CMSDK_DUALTIMER
#define HAL_TIMER_IRQ_NO        DUALTIMER_IRQn
#define MONITORING_TIMER_REF    CMSDK_TIMER0
#define MONITORING_TIMER_IRQ_NO TIMER0_IRQn

/*************************** Functions Declarations **************************/

void BSPLateInit(void);

#endif /* BSP_H */
