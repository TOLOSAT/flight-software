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
#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_REF             TIM4
#define HAL_TIMER_IRQ_NO          TIM4_IRQn
#define MONITORING_TIMER_REF      TIM3
#define MONITORING_TIMER_IRQ_NO   TIM3_IRQn

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF              IWDG1

/* SDIO CARD CONSTANTS */
#define SD_CARD_SDIO_PERIPH       SDMMC1
#define SD_CARD_SDIO_CLK_EDGE     SDMMC_CLOCK_EDGE_RISING
#define SD_CARD_SDIO_PWR_SAVE     SDMMC_CLOCK_POWER_SAVE_DISABLE
#define SD_CARD_SDIO_BUS_WIDTH    SDMMC_BUS_WIDE_4B
#define SD_CARD_SDIO_HW_FLOW_CTRL SDMMC_HARDWARE_FLOW_CONTROL_DISABLE
#define SD_CARD_SDIO_PRESCALER    8u

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
