/**
 * @file    qemu_bsp.h
 * @author  Merlin Kooshmanian
 * @date    15/06/2024
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef QEMU_BSP_H
#define QEMU_BSP_H

/******************************* Include Files *******************************/

#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

/* USER LED CONSTANTS */
#define USER_LED_PIN                            GPIO_PIN_0
#define USER_LED_GPIO_PORT                      CMSDK_GPIO0

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                         GPIO_PIN_4
#define USER_BUTTON_GPIO_PORT                   CMSDK_GPIO0
#define USER_BUTTON_EXTI_IRQ_NO                 EXTI4_IRQn
#define USER_BUTTON_IRQ_HANDLER                 EXTI4_IRQHandler

/* UART TMTC CONSTANTS */
#define UART_TMTC                               CMSDK_UART1

/* UART PRINT CONSTANTS */
#define UART_PRINT                              CMSDK_UART0

/* UART PL CONSTANTS */
#define UART_PL                                 CMSDK_UART2

/* ONE WIRE CONSTANTS */
#define ONEWIRE_PIN                             GPIO_PIN_1
#define ONEWIRE_GPIO_PORT                       CMSDK_GPIO0

/* SPECIFIC PROCEDURES */

#endif /* QEMU_BSP_H */
