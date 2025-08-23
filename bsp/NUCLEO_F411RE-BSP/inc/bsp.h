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
#include "stm32f4xx_hal.h"

/***************************** Macros Definitions ****************************/

/* INTERNAL TIMERS */
#define HAL_TIMER_REF            TIM4
#define HAL_TIMER_IRQ_NO         TIM4_IRQn
#define MONITORING_TIMER_REF     TIM3
#define MONITORING_TIMER_IRQ_NO  TIM3_IRQn

/* LED STATUS CONSTANTS */
#define LED_STATUS_PORT          GPIOA
#define LED_STATUS_PIN           GPIO_PIN_5
#define LED_STATUS_MODE          GPIO_MODE_OUTPUT_PP
#define LED_STATUS_PULL          GPIO_NOPULL
#define LED_STATUS_SPEED         GPIO_SPEED_FREQ_LOW

/* LED ERROR CONSTANTS */
#define LED_ERROR_PORT           GPIOA
#define LED_ERROR_PIN            GPIO_PIN_1
#define LED_ERROR_MODE           GPIO_MODE_OUTPUT_PP
#define LED_ERROR_PULL           GPIO_NOPULL
#define LED_ERROR_SPEED          GPIO_SPEED_FREQ_LOW

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF           USART1
#define UART_PRINT_IRQ_NO        USART1_IRQn
#define UART_PRINT_TX_PORT       GPIOA
#define UART_PRINT_TX_PIN        GPIO_PIN_15
#define UART_PRINT_TX_MODE       GPIO_MODE_AF_PP
#define UART_PRINT_TX_PULL       GPIO_NOPULL
#define UART_PRINT_TX_SPEED      GPIO_SPEED_FREQ_LOW
#define UART_PRINT_TX_ALT        GPIO_AF7_USART1
#define UART_PRINT_RX_PORT       GPIOB
#define UART_PRINT_RX_PIN        GPIO_PIN_7
#define UART_PRINT_RX_MODE       GPIO_MODE_AF_PP
#define UART_PRINT_RX_PULL       GPIO_NOPULL
#define UART_PRINT_RX_SPEED      GPIO_SPEED_FREQ_LOW
#define UART_PRINT_RX_ALT        GPIO_AF7_USART1

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD_REF          SPI2
#define SPI_SD_CARD_IRQ_NO       SPI2_IRQn
#define SPI_SD_CARD_PRESCALER    SPI_BAUDRATEPRESCALER_8
#define SPI_SD_CARD_SCK_PORT     GPIOB
#define SPI_SD_CARD_SCK_PIN      GPIO_PIN_13
#define SPI_SD_CARD_SCK_MODE     GPIO_MODE_AF_PP
#define SPI_SD_CARD_SCK_PULL     GPIO_NOPULL
#define SPI_SD_CARD_SCK_SPEED    GPIO_SPEED_FREQ_VERY_HIGH
#define SPI_SD_CARD_SCK_ALT      GPIO_AF5_SPI2
#define SPI_SD_CARD_MISO_PORT    GPIOB
#define SPI_SD_CARD_MISO_PIN     GPIO_PIN_14
#define SPI_SD_CARD_MISO_MODE    GPIO_MODE_AF_PP
#define SPI_SD_CARD_MISO_PULL    GPIO_NOPULL
#define SPI_SD_CARD_MISO_SPEED   GPIO_SPEED_FREQ_VERY_HIGH
#define SPI_SD_CARD_MISO_ALT     GPIO_AF5_SPI2
#define SPI_SD_CARD_MOSI_PORT    GPIOB
#define SPI_SD_CARD_MOSI_PIN     GPIO_PIN_15
#define SPI_SD_CARD_MOSI_MODE    GPIO_MODE_AF_PP
#define SPI_SD_CARD_MOSI_PULL    GPIO_NOPULL
#define SPI_SD_CARD_MOSI_SPEED   GPIO_SPEED_FREQ_VERY_HIGH
#define SPI_SD_CARD_MOSI_ALT     GPIO_AF5_SPI2
#define SPI_SD_CARD_CS_PORT      GPIOB
#define SPI_SD_CARD_CS_PIN       GPIO_PIN_12
#define SPI_SD_CARD_CS_MODE      GPIO_MODE_OUTPUT_PP
#define SPI_SD_CARD_CS_PULL      GPIO_NOPULL
#define SPI_SD_CARD_CS_SPEED     GPIO_SPEED_FREQ_LOW

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF             IWDG

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
