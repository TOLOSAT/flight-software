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

/* LED STATUS CONSTANTS */
#define LED_STATUS_PIN              GPIO_PIN_0
#define LED_STATUS_PORT             GPIOB

/* LED ERROR CONSTANTS */
#define LED_ERROR_PIN               GPIO_PIN_14
#define LED_ERROR_PORT              GPIOB

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN             GPIO_PIN_13
#define USER_BUTTON_PORT            GPIOC
#define USER_BUTTON_EXTI_IRQ_NO     EXTI15_10_IRQn
#define USER_BUTTON_IRQ_HANDLER     EXTI15_10_IRQHandler
#define USER_BUTTON_EXTI_LINE       EXTI_LINE_13

/* UART TMTC CONSTANTS */
#define UART_TMTC_REF               USART3
#define UART_TMTC_IRQ_HANDLER       USART3_IRQHandler
#define UART_TMTC_IRQ_NO            USART3_IRQn
#define UART_TMTC_CLK_ENABLE()      __HAL_RCC_USART3_CLK_ENABLE()
#define UART_TMTC_CLK_DISABLE()     __HAL_RCC_USART3_CLK_DISABLE()
#define UART_TMTC_TX_PIN            GPIO_PIN_8
#define UART_TMTC_TX_PORT           GPIOD
#define UART_TMTC_RX_PIN            GPIO_PIN_9
#define UART_TMTC_RX_PORT           GPIOD

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX_REF        DMA1_Stream0
#define UART_TMTC_DMA_TX_REF        DMA1_Stream1
#define UART_TMTC_DMA_CLK_ENABLE()  __HAL_RCC_DMA1_CLK_ENABLE()
#define UART_TMTC_DMA_DISABLE()     __HAL_RCC_DMA1_CLK_DISABLE()
#define UART_TMTC_DMA_RX_IRQ_NO     DMA1_Stream0_IRQn
#define UART_TMTC_DMA_TX_IRQ_NO     DMA1_Stream1_IRQn
#define UART_TMTC_DMA_RX_CHANNEL    DMA_REQUEST_USART3_RX
#define UART_TMTC_DMA_TX_CHANNEL    DMA_REQUEST_USART3_TX

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF              USART2
#define UART_PRINT_IRQ_HANDLER      USART2_IRQHandler
#define UART_PRINT_IRQ_NO           USART2_IRQn
#define UART_PRINT_CLK_ENABLE()     __HAL_RCC_USART2_CLK_ENABLE()
#define UART_PRINT_CLK_DISABLE()    __HAL_RCC_USART2_CLK_DISABLE()
#define UART_PRINT_TX_PIN           GPIO_PIN_5
#define UART_PRINT_TX_PORT          GPIOD
#define UART_PRINT_RX_PIN           GPIO_PIN_6
#define UART_PRINT_RX_PORT          GPIOD

/* UART PL CONSTANTS */
#define UART_PL_REF                 USART6
#define UART_PL_IRQ_HANDLER         USART6_IRQHandler
#define UART_PL_IRQ_NO              USART6_IRQn
#define UART_PL_CLK_ENABLE()        __HAL_RCC_USART6_CLK_ENABLE()
#define UART_PL_CLK_DISABLE()       __HAL_RCC_USART6_CLK_DISABLE()
#define UART_PL_TX_PIN              GPIO_PIN_6
#define UART_PL_TX_PORT             GPIOC
#define UART_PL_RX_PIN              GPIO_PIN_7
#define UART_PL_RX_PORT             GPIOC

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC_REF             I2C1
#define I2C_AVIONIC_EVT_IRQ_HANDLER I2C1_EV_IRQHandler
#define I2C_AVIONIC_IRQ_NO          I2C1_EV_IRQn
#define I2C_AVIONIC_SCL_PIN         GPIO_PIN_8
#define I2C_AVIONIC_SCL_PORT        GPIOB
#define I2C_AVIONIC_SDA_PIN         GPIO_PIN_9
#define I2C_AVIONIC_SDA_PORT        GPIOB

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD_REF             SPI1
#define SPI_SD_CARD_IRQ_HANDLER     SPI1_IRQHandler
#define SPI_SD_CARD_IRQ_NO          SPI1_IRQn
#define SPI_SD_CARD_SCK_PIN         GPIO_PIN_3
#define SPI_SD_CARD_SCK_PORT        GPIOB
#define SPI_SD_CARD_MISO_PIN        GPIO_PIN_4
#define SPI_SD_CARD_MISO_PORT       GPIOB
#define SPI_SD_CARD_MOSI_PIN        GPIO_PIN_5
#define SPI_SD_CARD_MOSI_PORT       GPIOB

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                 GPIO_PIN_4
#define SD_PORT                     GPIOA

/* ONE WIRE CONSTANTS */
#define ONEWIRE_PIN                 GPIO_PIN_5
#define ONEWIRE_PORT                GPIOA

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF                IWDG1

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
