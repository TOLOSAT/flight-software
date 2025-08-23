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

/* LED STATUS CONSTANTS */
#define LED_STATUS_PORT           GPIOB
#define LED_STATUS_PIN            GPIO_PIN_1
#define LED_STATUS_MODE           GPIO_MODE_OUTPUT_PP
#define LED_STATUS_PULL           GPIO_NOPULL
#define LED_STATUS_SPEED          GPIO_SPEED_FREQ_LOW

/* LED ERROR CONSTANTS */
#define LED_ERROR_PORT            GPIOB
#define LED_ERROR_PIN             GPIO_PIN_0
#define LED_ERROR_MODE            GPIO_MODE_OUTPUT_PP
#define LED_ERROR_PULL            GPIO_NOPULL
#define LED_ERROR_SPEED           GPIO_SPEED_FREQ_LOW

/* NAND CONSTANTS */
#define NAND_PAGE_SIZE            4096U
#define NAND_SPARE_AREA_SIZE      256U
#define NAND_BLOCK_SIZE_IN_PAGES  64U
#define NAND_PLANE_SIZE_IN_BLOCKS 2048U
#define NAND_BLOCK_COUNT          4096
#define NAND_PLANE_COUNT          2U
#define NAND_TCLR_SETUP_TIME      2U
#define NAND_TAR_SETUP_TIME       2U
#define NAND_TIMING_SETUP_TIME    0U
#define NAND_TIMING_WAIT_TIME     1U
#define NAND_TIMING_HOLD_TIME     1U
#define NAND_TIMING_HIZ_TIME      4U
#define NAND_ECC_COMPUTATION      FMC_NAND_ECC_DISABLE
#define NAND_ECC_PAGE_SIZE        FMC_NAND_ECC_PAGE_SIZE_4096BYTE
#define NAND_MEMORY_BUS_WIDTH     FMC_NAND_MEM_BUS_WIDTH_8
#define NAND_WAIT_FEATURE         FMC_NAND_WAIT_FEATURE_ENABLE
#define NAND_BANK                 FMC_NAND_BANK3
#define NAND_EXTRA_COMMAND        DISABLE
#define NAND_WP_PIN               GPIO_PIN_10
#define NAND_WP_PORT              GPIOD
#define NAND_WP_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF              IWDG1

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
