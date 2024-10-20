/**
 * @file    bsp.h
 * @author  Merlin Kooshmanian
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */
#ifndef BSP_H
#define BSP_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "stm32f4xx_hal.h"

/***************************** Macros Definitions ****************************/

/* LED STATUS CONSTANTS */
#define LED_STATUS_PIN                          GPIO_PIN_15
#define LED_STATUS_PORT                         GPIOD

/* LED ERROR CONSTANTS */
#define LED_ERROR_PIN                           GPIO_PIN_14
#define LED_ERROR_PORT                          GPIOD

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                         GPIO_PIN_0
#define USER_BUTTON_PORT                        GPIOA
#define USER_BUTTON_EXTI_IRQ_NO                 EXTI0_IRQn
#define USER_BUTTON_IRQ_HANDLER                 EXTI0_IRQHandler
#define USER_BUTTON_EXTI_LINE                   EXTI_LINE_0

/* UART TMTC CONSTANTS */
#define UART_TMTC_REF                           USART2
#define UART_TMTC_IRQ_HANDLER                   USART2_IRQHandler
#define UART_TMTC_IRQ_NO                        USART2_IRQn
#define UART_TMTC_CLK_ENABLE()                  __HAL_RCC_USART2_CLK_ENABLE()
#define UART_TMTC_CLK_DISABLE()                 __HAL_RCC_USART2_CLK_DISABLE()
#define UART_TMTC_TX_PIN                        GPIO_PIN_2
#define UART_TMTC_TX_PORT                       GPIOA
#define UART_TMTC_RX_PIN                        GPIO_PIN_3
#define UART_TMTC_RX_PORT                       GPIOA

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX_REF                    DMA1_Stream5
#define UART_TMTC_DMA_TX_REF                    DMA1_Stream6
#define UART_TMTC_DMA_CLK_ENABLE()              __HAL_RCC_DMA1_CLK_ENABLE()
#define UART_TMTC_DMA_DISABLE()                 __HAL_RCC_DMA1_CLK_DISABLE()
#define UART_TMTC_DMA_RX_IRQ_NO                 DMA1_Stream5_IRQn
#define UART_TMTC_DMA_TX_IRQ_NO                 DMA1_Stream6_IRQn
#define UART_TMTC_DMA_RX_CHANNEL                DMA_CHANNEL_4
#define UART_TMTC_DMA_TX_CHANNEL                DMA_CHANNEL_4

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF                          USART3
#define UART_PRINT_IRQ_HANDLER                  USART3_IRQHandler
#define UART_PRINT_IRQ_NO                       USART3_IRQn
#define UART_PRINT_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
#define UART_PRINT_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()
#define UART_PRINT_TX_PIN                       GPIO_PIN_8
#define UART_PRINT_TX_PORT                      GPIOD
#define UART_PRINT_RX_PIN                       GPIO_PIN_11
#define UART_PRINT_RX_PORT                      GPIOB

/* UART PL CONSTANTS */
#define UART_PL_REF                             USART6
#define UART_PL_IRQ_HANDLER                     USART6_IRQHandler
#define UART_PL_IRQ_NO                          USART6_IRQn
#define UART_PL_CLK_ENABLE()                    __HAL_RCC_USART6_CLK_ENABLE()
#define UART_PL_CLK_DISABLE()                   __HAL_RCC_USART6_CLK_DISABLE()
#define UART_PL_TX_PIN                          GPIO_PIN_6
#define UART_PL_TX_PORT                         GPIOC
#define UART_PL_RX_PIN                          GPIO_PIN_7
#define UART_PL_RX_PORT                         GPIOC

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC_REF                         I2C1
#define I2C_AVIONIC_EVT_IRQ_HANDLER             I2C1_EV_IRQHandler
#define I2C_AVIONIC_IRQ_NO                      I2C1_EV_IRQn
#define I2C_AVIONIC_SCL_PIN                     GPIO_PIN_8
#define I2C_AVIONIC_SCL_PORT                    GPIOB
#define I2C_AVIONIC_SDA_PIN                     GPIO_PIN_9
#define I2C_AVIONIC_SDA_PORT                    GPIOB

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD_REF                         SPI2
#define SPI_SD_CARD_IRQ_HANDLER                 SPI2_IRQHandler
#define SPI_SD_CARD_IRQ_NO                      SPI2_IRQn
#define SPI_SD_CARD_SCK_PIN                     GPIO_PIN_13
#define SPI_SD_CARD_SCK_PORT                    GPIOB
#define SPI_SD_CARD_MISO_PIN                    GPIO_PIN_14
#define SPI_SD_CARD_MISO_PORT                   GPIOB
#define SPI_SD_CARD_MOSI_PIN                    GPIO_PIN_15
#define SPI_SD_CARD_MOSI_PORT                   GPIOB

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                             GPIO_PIN_12
#define SD_PORT                                 GPIOB

/* ONE WIRE CONSTANTS */
#define ONEWIRE_PIN                             GPIO_PIN_1
#define ONEWIRE_PORT                            GPIOC

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
returnCode_t BSPLateInit(void);

#endif /* BSP_H */
