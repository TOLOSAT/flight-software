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
#define HAL_TIMER_REF           TIM4
#define HAL_TIMER_IRQ_NO        TIM4_IRQn
#define MONITORING_TIMER_REF    TIM3
#define MONITORING_TIMER_IRQ_NO TIM3_IRQn

/* LED STATUS CONSTANTS */
#define LED_STATUS_PORT         GPIOI
#define LED_STATUS_PIN          GPIO_PIN_8
#define LED_STATUS_MODE         GPIO_MODE_OUTPUT_PP
#define LED_STATUS_PULL         GPIO_NOPULL
#define LED_STATUS_SPEED        GPIO_SPEED_FREQ_LOW

/* LED ERROR CONSTANTS */
#define LED_ERROR_PORT          GPIOC
#define LED_ERROR_PIN           GPIO_PIN_15
#define LED_ERROR_MODE          GPIO_MODE_OUTPUT_PP
#define LED_ERROR_PULL          GPIO_NOPULL
#define LED_ERROR_SPEED         GPIO_SPEED_FREQ_LOW

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF          USART1
#define UART_PRINT_IRQ_NO       USART1_IRQn
#define UART_PRINT_CLK_SRC      RCC_USART16CLKSOURCE_D2PCLK2
#define UART_PRINT_TX_PORT      GPIOA
#define UART_PRINT_TX_PIN       GPIO_PIN_9
#define UART_PRINT_TX_MODE      GPIO_MODE_AF_PP
#define UART_PRINT_TX_PULL      GPIO_NOPULL
#define UART_PRINT_TX_SPEED     GPIO_SPEED_FREQ_LOW
#define UART_PRINT_TX_ALT       GPIO_AF7_USART1
#define UART_PRINT_RX_PIN       GPIO_PIN_10
#define UART_PRINT_RX_PORT      GPIOA
#define UART_PRINT_RX_MODE      GPIO_MODE_AF_PP
#define UART_PRINT_RX_PULL      GPIO_NOPULL
#define UART_PRINT_RX_SPEED     GPIO_SPEED_FREQ_LOW
#define UART_PRINT_RX_ALT       GPIO_AF7_USART

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF            IWDG1

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
