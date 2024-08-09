/**
 * @file    nucleo_f411re_bsp.h
 * @author  Merlin Kooshmanian
 * @date    16/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */
#ifndef NUCLEO_F411RE_BSP_H
#define NUCLEO_F411RE_BSP_H

/******************************* Include Files *******************************/

#include "stm32f4xx_hal.h"

/***************************** Macros Definitions ****************************/

/* LED 2 CONSTANTS */
#define LED2_PIN                                GPIO_PIN_5
#define LED2_PORT                          GPIOA

/* USER LED CONSTANTS */
#define USER_LED_PIN                            LED2_PIN
#define USER_LED_PORT                      LED2_PORT

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                         GPIO_PIN_13
#define USER_BUTTON_PORT                   GPIOC
#define USER_BUTTON_EXTI_IRQ_NO                 EXTI15_10_IRQn
#define USER_BUTTON_IRQ_HANDLER                 EXTI15_10_IRQHandler
#define USER_BUTTON_EXTI_LINE                   EXTI_LINE_13

/* UART TMTC CONSTANTS */
#define UART_TMTC                               USART2
#define UART_TMTC_IRQ_HANDLER                   USART2_IRQHandler
#define UART_TMTC_IRQ_NO                        USART2_IRQn
#define UART_TMTC_CLK_ENABLE()                  __HAL_RCC_USART2_CLK_ENABLE()
#define UART_TMTC_CLK_DISABLE()                 __HAL_RCC_USART2_CLK_DISABLE()
#define UART_TMTC_TX_PIN                        GPIO_PIN_2
#define UART_TMTC_TX_PORT                  GPIOA
#define UART_TMTC_RX_PIN                        GPIO_PIN_3
#define UART_TMTC_RX_PORT                  GPIOA

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX                        DMA1_Stream5
#define UART_TMTC_DMA_TX                        DMA1_Stream6
#define UART_TMTC_DMA_CLK_ENABLE()              __HAL_RCC_DMA1_CLK_ENABLE()
#define UART_TMTC_DMA_DISABLE()                 __HAL_RCC_DMA1_CLK_DISABLE()
#define UART_TMTC_DMA_RX_IRQ_NO                 DMA1_Stream5_IRQn
#define UART_TMTC_DMA_TX_IRQ_NO                 DMA1_Stream6_IRQn

/* UART PRINT CONSTANTS */
#define UART_PRINT                              USART1
#define UART_PRINT_IRQ_HANDLER                  USART1_IRQHandler
#define UART_PRINT_IRQ_NO                       USART1_IRQn
#define UART_PRINT_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define UART_PRINT_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()
#define UART_PRINT_TX_PIN                       GPIO_PIN_15
#define UART_PRINT_TX_PORT                 GPIOA
#define UART_PRINT_RX_PIN                       GPIO_PIN_7
#define UART_PRINT_RX_PORT                 GPIOB

/* UART PL CONSTANTS */
#define UART_PL                                 USART6
#define UART_PL_IRQ_HANDLER                     USART6_IRQHandler
#define UART_PL_IRQ_NO                          USART6_IRQn
#define UART_PL_CLK_ENABLE()                    __HAL_RCC_USART6_CLK_ENABLE()
#define UART_PL_CLK_DISABLE()                   __HAL_RCC_USART6_CLK_DISABLE()
#define UART_PL_TX_PIN                          GPIO_PIN_6
#define UART_PL_TX_PORT                    GPIOC
#define UART_PL_RX_PIN                          GPIO_PIN_7
#define UART_PL_RX_PORT                    GPIOC

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC                             I2C1
#define I2C_AVIONIC_EVT_IRQ_HANDLER             I2C1_EV_IRQHandler
#define I2C_AVIONIC_IRQ_NO                      I2C1_EV_IRQn
#define I2C_AVIONIC_SCL_PIN                     GPIO_PIN_8
#define I2C_AVIONIC_SCL_PORT               GPIOB
#define I2C_AVIONIC_SDA_PIN                     GPIO_PIN_9
#define I2C_AVIONIC_SDA_PORT               GPIOB

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD                             SPI2
#define SPI_SD_CARD_IRQ_HANDLER                 SPI2_IRQHandler
#define SPI_SD_CARD_IRQ_NO                      SPI2_IRQn
#define SPI_SD_CARD_SCK_PIN                     GPIO_PIN_13
#define SPI_SD_CARD_SCK_PORT               GPIOB
#define SPI_SD_CARD_MISO_PIN                    GPIO_PIN_14
#define SPI_SD_CARD_MISO_PORT              GPIOB
#define SPI_SD_CARD_MOSI_PIN                    GPIO_PIN_15
#define SPI_SD_CARD_MOSI_PORT              GPIOB

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                             GPIO_PIN_12
#define SD_PORT                            GPIOB

/* ONE WIRE CONSTANTS */
#define ONEWIRE_PIN                             GPIO_PIN_0
#define ONEWIRE_PORT                       GPIOA

/***************************** Types Definitions *****************************/

/** 
 * @enum    bspStatus_t
 * @brief   BSP functions specific returns 
 */
typedef enum
{
    BSP_SUCCESSFUL = 0u,    /**< Function succeed */
    BSP_ERROR = 1u,         /**< Function failed */
} bspStatus_t;

/*************************** Functions Declarations **************************/

bspStatus_t SystemClock_Config(void);
bspStatus_t BSPLateInit(void);

#endif /* NUCLEO_F411RE_BSP_H */
