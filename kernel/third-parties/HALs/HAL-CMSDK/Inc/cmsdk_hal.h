/**
 * @file    cmsdk_hal.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for HAL CMSDK
 * @date    09/06/2024
 */

#ifndef HAL_CMSDK_H
#define HAL_CMSDK_H

/******************************* Include Files *******************************/

#include "cmsdk_hal_types.h"
#include "cmsdk_dualtimer.h"
#include "cmsdk_timer.h"
#include "cmsdk_uart.h"
#include "cmsdk_gpio.h"

/*************************** Variables Declarations **************************/

/**
 * @var     SystemCoreClock
 * @brief   System clock frequency value
 */
extern uint32_t SystemCoreClock;

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef cmsdk_InitHal(void);
extern HAL_StatusTypeDef HAL_InitTick(void);
extern void HAL_Delay(uint32_t delay);
extern uint32_t HAL_GetTick(void);
extern void HAL_IncTick(void);

#endif /* HAL_CMSDK_H */