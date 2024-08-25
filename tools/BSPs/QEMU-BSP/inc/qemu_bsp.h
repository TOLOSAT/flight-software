/**
 * @file    qemu_bsp.h
 * @author  Merlin Kooshmanian
 * @date    15/06/2024
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */
#ifndef QEMU_BSP_H
#define QEMU_BSP_H

/******************************* Include Files *******************************/

#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

/* USER LED CONSTANTS */
#define USER_LED_PIN                            GPIO_PIN_0
#define USER_LED_PORT                           CMSDK_GPIO0

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                         GPIO_PIN_4
#define USER_BUTTON_PORT                        CMSDK_GPIO0
#define USER_BUTTON_EXTI_IRQ_NO                 GPIO0_4_IRQn
#define USER_BUTTON_IRQ_HANDLER                 GPIO0_4_Handler

/* UART TMTC CONSTANTS */
#define UART_TMTC_REF                           CMSDK_UART1
#define UART_TMTC_RX_IRQ_NO                     UART1RX_IRQn
#define UART_TMTC_TX_IRQ_NO                     UART1TX_IRQn
// For compatibility : to be change
#define UART_TMTC_IRQ_NO                        UART1TX_IRQn

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX                        0
#define UART_TMTC_DMA_TX                        0
#define UART_TMTC_DMA_RX_IRQ_NO                 0
#define UART_TMTC_DMA_TX_IRQ_NO                 0

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF                          CMSDK_UART0
#define UART_PRINT_RX_IRQ_NO                    UART0RX_IRQn
#define UART_PRINT_TX_IRQ_NO                    UART0TX_IRQn
// For compatibility : to be change
#define UART_PRINT_IRQ_NO                       UART0TX_IRQn

/* UART PL CONSTANTS */
#define UART_PL_REF                             CMSDK_UART2
#define UART_PL_RX_IRQ_NO                       UART2RX_IRQn
#define UART_PL_TX_IRQ_NO                       UART2TX_IRQn
// For compatibility : to be change
#define UART_PL_IRQ_NO                          UART2TX_IRQn

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC_REF                         0
#define I2C_AVIONIC_IRQ_NO                      0

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD_REF                         0
#define SPI_SD_CARD_IRQ_NO                      0

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                             GPIO_PIN_2
#define SD_PORT                                 CMSDK_GPIO0

/* ONE WIRE CONSTANTS */
#define ONEWIRE_PIN                             GPIO_PIN_1
#define ONEWIRE_PORT                            CMSDK_GPIO0

/* SPECIFIC PROCEDURES */

/*************************** Functions Declarations **************************/

uint32_t BSPLateInit(void);

#endif /* QEMU_BSP_H */
