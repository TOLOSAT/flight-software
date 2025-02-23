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

HAL_StatusTypeDef cmsdk_InitHal(void);
HAL_StatusTypeDef HAL_InitTick(void);
void cmsdk_HalDelay(uint32_t delay);
uint32_t cmsdk_HalGetTick(void);
void cmsdk_HalIncTick(void);

#endif /* HAL_CMSDK_H */