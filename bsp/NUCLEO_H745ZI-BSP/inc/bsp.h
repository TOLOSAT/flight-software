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
#define HAL_TIMER_REF            TIM4
#define HAL_TIMER_IRQ_NO         TIM4_IRQn
#define MONITORING_TIMER_REF     TIM3
#define MONITORING_TIMER_IRQ_NO  TIM3_IRQn

/* LED STATUS CONSTANTS */
#define LED_STATUS_PORT          GPIOB
#define LED_STATUS_PIN           GPIO_PIN_0
#define LED_STATUS_MODE          GPIO_MODE_OUTPUT_PP
#define LED_STATUS_PULL          GPIO_NOPULL
#define LED_STATUS_SPEED         GPIO_SPEED_FREQ_LOW

/* LED ERROR CONSTANTS */
#define LED_ERROR_PORT           GPIOB
#define LED_ERROR_PIN            GPIO_PIN_14
#define LED_ERROR_MODE           GPIO_MODE_OUTPUT_PP
#define LED_ERROR_PULL           GPIO_NOPULL
#define LED_ERROR_SPEED          GPIO_SPEED_FREQ_LOW

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PORT         GPIOC
#define USER_BUTTON_PIN          GPIO_PIN_13
#define USER_BUTTON_MODE         GPIO_MODE_IT_FALLING
#define USER_BUTTON_PULL         GPIO_NOPULL
#define USER_BUTTON_SPEED        GPIO_SPEED_FREQ_LOW
#define USER_BUTTON_EXTI_IRQ_NO  EXTI15_10_IRQn
#define USER_BUTTON_EXTI_LINE    EXTI_LINE_13

/* UART TMTC CONSTANTS */
#define UART_TMTC_REF            USART3
#define UART_TMTC_IRQ_NO         USART3_IRQn
#define UART_TMTC_CLK_SRC        RCC_USART234578CLKSOURCE_D2PCLK1
#define UART_TMTC_TX_PORT        GPIOD
#define UART_TMTC_TX_PIN         GPIO_PIN_8
#define UART_TMTC_TX_MODE        GPIO_MODE_AF_PP
#define UART_TMTC_TX_PULL        GPIO_NOPULL
#define UART_TMTC_TX_SPEED       GPIO_SPEED_FREQ_LOW
#define UART_TMTC_TX_ALT         GPIO_AF7_USART3
#define UART_TMTC_RX_PORT        GPIOD
#define UART_TMTC_RX_PIN         GPIO_PIN_9
#define UART_TMTC_RX_MODE        GPIO_MODE_AF_PP
#define UART_TMTC_RX_PULL        GPIO_NOPULL
#define UART_TMTC_RX_SPEED       GPIO_SPEED_FREQ_LOW
#define UART_TMTC_RX_ALT         GPIO_AF7_USART3

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX_REF     DMA1_Stream0
#define UART_TMTC_DMA_TX_REF     DMA1_Stream1
#define UART_TMTC_DMA_RX_IRQ_NO  DMA1_Stream0_IRQn
#define UART_TMTC_DMA_TX_IRQ_NO  DMA1_Stream1_IRQn
#define UART_TMTC_DMA_RX_CHANNEL DMA_REQUEST_USART3_RX
#define UART_TMTC_DMA_TX_CHANNEL DMA_REQUEST_USART3_TX

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF           USART2
#define UART_PRINT_IRQ_NO        USART2_IRQn
#define UART_PRINT_CLK_SRC       RCC_USART234578CLKSOURCE_D2PCLK1
#define UART_PRINT_TX_PORT       GPIOD
#define UART_PRINT_TX_PIN        GPIO_PIN_5
#define UART_PRINT_TX_MODE       GPIO_MODE_AF_PP
#define UART_PRINT_TX_PULL       GPIO_NOPULL
#define UART_PRINT_TX_SPEED      GPIO_SPEED_FREQ_LOW
#define UART_PRINT_TX_ALT        GPIO_AF7_USART2
#define UART_PRINT_RX_PORT       GPIOD
#define UART_PRINT_RX_PIN        GPIO_PIN_6
#define UART_PRINT_RX_MODE       GPIO_MODE_AF_PP
#define UART_PRINT_RX_PULL       GPIO_NOPULL
#define UART_PRINT_RX_SPEED      GPIO_SPEED_FREQ_LOW
#define UART_PRINT_RX_ALT        GPIO_AF7_USART2

/* UART PL CONSTANTS */
#define UART_PL_REF              USART6
#define UART_PL_IRQ_NO           USART6_IRQn
#define UART_PL_CLK_SRC          RCC_USART16CLKSOURCE_D2PCLK2
#define UART_PL_TX_PORT          GPIOC
#define UART_PL_TX_PIN           GPIO_PIN_6
#define UART_PL_TX_MODE          GPIO_MODE_AF_PP
#define UART_PL_TX_PULL          GPIO_NOPULL
#define UART_PL_TX_SPEED         GPIO_SPEED_FREQ_LOW
#define UART_PL_TX_ALT           GPIO_AF7_USART6
#define UART_PL_RX_PORT          GPIOC
#define UART_PL_RX_PIN           GPIO_PIN_7
#define UART_PL_RX_MODE          GPIO_MODE_AF_PP
#define UART_PL_RX_PULL          GPIO_NOPULL
#define UART_PL_RX_SPEED         GPIO_SPEED_FREQ_LOW
#define UART_PL_RX_ALT           GPIO_AF7_USART6

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC_REF          I2C1
#define I2C_AVIONIC_IRQ_NO       I2C1_EV_IRQn
#define I2C_AVIONIC_CLK_SRC      RCC_I2C123CLKSOURCE_D2PCLK1
#define I2C_AVIONIC_SCL_PORT     GPIOB
#define I2C_AVIONIC_SCL_PIN      GPIO_PIN_8
#define I2C_AVIONIC_SCL_MODE     GPIO_MODE_AF_OD
#define I2C_AVIONIC_SCL_PULL     GPIO_NOPULL
#define I2C_AVIONIC_SCL_SPEED    GPIO_SPEED_FREQ_LOW
#define I2C_AVIONIC_SCL_ALT      GPIO_AF4_I2C1
#define I2C_AVIONIC_SDA_PORT     GPIOB
#define I2C_AVIONIC_SDA_PIN      GPIO_PIN_9
#define I2C_AVIONIC_SDA_MODE     GPIO_MODE_AF_OD
#define I2C_AVIONIC_SDA_PULL     GPIO_NOPULL
#define I2C_AVIONIC_SDA_SPEED    GPIO_SPEED_FREQ_LOW
#define I2C_AVIONIC_SDA_ALT      GPIO_AF4_I2C1

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD_REF          SPI1
#define SPI_SD_CARD_IRQ_NO       SPI1_IRQn
#define SPI_SD_CARD_PRESCALER    SPI_BAUDRATEPRESCALER_8
#define SPI_SD_CARD_CLK_SRC      RCC_SPI123CLKSOURCE_PLL
#define SPI_SD_CARD_SCK_PORT     GPIOB
#define SPI_SD_CARD_SCK_PIN      GPIO_PIN_3
#define SPI_SD_CARD_SCK_MODE     GPIO_MODE_AF_PP
#define SPI_SD_CARD_SCK_PULL     GPIO_NOPULL
#define SPI_SD_CARD_SCK_SPEED    GPIO_SPEED_FREQ_VERY_HIGH
#define SPI_SD_CARD_SCK_ALT      GPIO_AF5_SPI1
#define SPI_SD_CARD_MISO_PORT    GPIOB
#define SPI_SD_CARD_MISO_PIN     GPIO_PIN_4
#define SPI_SD_CARD_MISO_MODE    GPIO_MODE_AF_PP
#define SPI_SD_CARD_MISO_PULL    GPIO_NOPULL
#define SPI_SD_CARD_MISO_SPEED   GPIO_SPEED_FREQ_VERY_HIGH
#define SPI_SD_CARD_MISO_ALT     GPIO_AF5_SPI1
#define SPI_SD_CARD_MOSI_PORT    GPIOB
#define SPI_SD_CARD_MOSI_PIN     GPIO_PIN_5
#define SPI_SD_CARD_MOSI_MODE    GPIO_MODE_AF_PP
#define SPI_SD_CARD_MOSI_PULL    GPIO_NOPULL
#define SPI_SD_CARD_MOSI_SPEED   GPIO_SPEED_FREQ_VERY_HIGH
#define SPI_SD_CARD_MOSI_ALT     GPIO_AF5_SPI1

/* SD CARD GPIO CONSTANTS */
#define SPI_SD_CARD_CS_PORT      GPIOA
#define SPI_SD_CARD_CS_PIN       GPIO_PIN_4
#define SPI_SD_CARD_CS_MODE      GPIO_MODE_OUTPUT_PP
#define SPI_SD_CARD_CS_PULL      GPIO_NOPULL
#define SPI_SD_CARD_CS_SPEED     GPIO_SPEED_FREQ_LOW

/* ONE WIRE CONSTANTS */
#define ONEWIRE_TIMER_REF        TIM5
#define ONEWIRE_TIMER_IRQ_NO     TIM5_IRQn
#define ONEWIRE_PORT             GPIOA
#define ONEWIRE_PIN              GPIO_PIN_5

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF             IWDG1

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
