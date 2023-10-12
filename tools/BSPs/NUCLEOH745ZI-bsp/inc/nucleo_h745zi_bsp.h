/**
 * @file    nucleoh745zi_bsp.h
 * @author  Merlin Kooshmanian
 * @date    26/05/2023
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef NUCLEOH745ZI_BSP_H
#define NUCLEOH745ZI_BSP_H

/******************************* Include Files *******************************/

#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

/* LED 1 CONSTANTS */
#define LED1_PIN                                        GPIO_PIN_0
#define LED1_GPIO_PORT                                  GPIOB

/* LED 2 CONSTANTS */
#define LED2_PIN                                        GPIO_PIN_1
#define LED2_GPIO_PORT                                  GPIOE

/* LED 3 CONSTANTS */
#define LED3_PIN                                        GPIO_PIN_14
#define LED3_GPIO_PORT                                  GPIOB

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                                 GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                           GPIOC
#define USER_BUTTON_EXTI_IRQn                           EXTI15_10_IRQn
#define USER_BUTTON_IRQ_HANDLER                         EXTI15_10_IRQHandler
#define USER_BUTTON_IRQ_CALLBACK                        HAL_GPIO_EXTI_Callback
#define USER_BUTTON_EXTI_LINE                           EXTI_LINE_13

/* UART TMTC CONSTANTS */
#define TAPAS_UART_TMTC                                 USART3
#define TAPAS_UART_TMTC_IRQ_HANDLER                     USART3_IRQHandler
#define TAPAS_UART_TMTC_IRQ_NO                          USART3_IRQn
#define TAPAS_UART_TMTC_CLK_ENABLE()                    __HAL_RCC_USART3_CLK_ENABLE()
#define TAPAS_UART_TMTC_CLK_DISABLE()                   __HAL_RCC_USART3_CLK_DISABLE()
#define TAPAS_UART_TMTC_TX_PIN                          GPIO_PIN_8
#define TAPAS_UART_TMTC_TX_GPIO_PORT                    GPIOD
#define TAPAS_UART_TMTC_RX_PIN                          GPIO_PIN_9
#define TAPAS_UART_TMTC_RX_GPIO_PORT                    GPIOD

/* UART TMTC DMA CONSTANTS */
#define TAPAS_UART_TMTC_DMA_RX                          hdma_usart3_rx
#define TAPAS_UART_TMTC_DMA_TX                          hdma_usart3_tx
#define TAPAS_UART_TMTC_DMA_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define TAPAS_UART_TMTC_DMA_DISABLE()                   __HAL_RCC_DMA1_CLK_DISABLE()
#define TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER              DMA1_Stream0_IRQHandler
#define TAPAS_UART_TMTC_DMA_RX_IRQ_NO                   DMA1_Stream0_IRQn
#define TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER              DMA1_Stream1_IRQHandler
#define TAPAS_UART_TMTC_DMA_TX_IRQ_NO                   DMA1_Stream1_IRQn

/* UART PRINT CONSTANTS */
#define TAPAS_UART_PRINT                                USART2
#define TAPAS_UART_PRINT_IRQ_HANDLER                    USART2_IRQHandler
#define TAPAS_UART_PRINT_IRQ_NO                         USART2_IRQn
#define TAPAS_UART_PRINT_CLK_ENABLE()                   __HAL_RCC_USART2_CLK_ENABLE()
#define TAPAS_UART_PRINT_CLK_DISABLE()                  __HAL_RCC_USART2_CLK_DISABLE()
#define TAPAS_UART_PRINT_TX_PIN                         GPIO_PIN_5
#define TAPAS_UART_PRINT_TX_GPIO_PORT                   GPIOD
#define TAPAS_UART_PRINT_RX_PIN                         GPIO_PIN_6
#define TAPAS_UART_PRINT_RX_GPIO_PORT                   GPIOD

/* UART PL CONSTANTS */
#define TAPAS_UART_PL                                   USART6
#define TAPAS_UART_PL_IRQ_HANDLER                       USART6_IRQHandler
#define TAPAS_UART_PL_IRQ_NO                            USART6_IRQn
#define TAPAS_UART_PL_CLK_ENABLE()                      __HAL_RCC_USART6_CLK_ENABLE()
#define TAPAS_UART_PL_CLK_DISABLE()                     __HAL_RCC_USART6_CLK_DISABLE()
#define TAPAS_UART_PL_TX_PIN                            GPIO_PIN_6
#define TAPAS_UART_PL_TX_GPIO_PORT                      GPIOC
#define TAPAS_UART_PL_RX_PIN                            GPIO_PIN_7
#define TAPAS_UART_PL_RX_GPIO_PORT                      GPIOC

/* I2C AVIONIC CONSTANTS */
#define TAPAS_I2C_AVIONIC                               I2C1
#define TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER               I2C1_EV_IRQHandler
#define TAPAS_I2C_AVIONIC_EVT_IRQ_NO                    I2C1_EV_IRQn
#define TAPAS_I2C_AVIONIC_SCL_PIN                       GPIO_PIN_8
#define TAPAS_I2C_AVIONIC_SCL_GPIO_PORT                 GPIOB
#define TAPAS_I2C_AVIONIC_SDA_PIN                       GPIO_PIN_9
#define TAPAS_I2C_AVIONIC_SDA_GPIO_PORT                 GPIOB

/* SPI SD CARD CONSTANTS */
#define TAPAS_SPI_SDCARD                                SPI1
#define TAPAS_SPI_SDCARD_IRQ_HANDLER                    SPI1_IRQHandler
#define TAPAS_SPI_SDCARD_IRQ_NO                         SPI1_IRQn
#define TAPAS_SPI_SDCARD_SCK_PIN                        GPIO_PIN_3
#define TAPAS_SPI_SDCARD_SCK_GPIO_PORT                  GPIOB
#define TAPAS_SPI_SDCARD_MISO_PIN                       GPIO_PIN_4
#define TAPAS_SPI_SDCARD_MISO_GPIO_PORT                 GPIOB
#define TAPAS_SPI_SDCARD_MOSI_PIN                       GPIO_PIN_5
#define TAPAS_SPI_SDCARD_MOSI_GPIO_PORT                 GPIOB
#define TAPAS_SPI_SDCARD_CS_PIN                         GPIO_PIN_4
#define TAPAS_SPI_SDCARD_CS_GPIO_PORT                   GPIOA

#endif /* NUCLEOH745ZI_BSP_H */
