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

/* LED STATUS CONSTANTS */
#define LED_STATUS_PORT         CMSDK_GPIO0
#define LED_STATUS_PIN          GPIO_PIN_0
#define LED_STATUS_MODE         GPIO_MODE_OUTPUT
#define LED_STATUS_PULL         GPIO_NOPULL
#define LED_STATUS_SPEED        GPIO_SPEED_FREQ_LOW

/* LED ERROR CONSTANTS */
#define LED_ERROR_PORT          CMSDK_GPIO0
#define LED_ERROR_PIN           GPIO_PIN_1
#define LED_ERROR_MODE          GPIO_MODE_OUTPUT
#define LED_ERROR_PULL          GPIO_NOPULL
#define LED_ERROR_SPEED         GPIO_SPEED_FREQ_LOW

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF          CMSDK_UART0
#define UART_PRINT_RX_IRQ_NO    UART0RX_IRQn
#define UART_PRINT_TX_IRQ_NO    UART0TX_IRQn
// For compatibility
#define UART_PRINT_IRQ_NO       UART0RX_IRQn

/*************************** Functions Declarations **************************/

void BSPLateInit(void);

#endif /* BSP_H */
