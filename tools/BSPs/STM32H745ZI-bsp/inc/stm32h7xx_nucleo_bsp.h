/**
 * @file    stm32h7xx_nucleo_bsp.h
 * @author  Merlin Kooshmanian
 * @date    26/05/2023
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef STM32H7xx_NUCLEO_BSP_H
#define STM32H7xx_NUCLEO_BSP_H

/******************************* Include Files *******************************/

#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

#define LED1_PIN                                        GPIO_PIN_0
#define LED1_GPIO_PORT                                  GPIOB
#define LED1_GPIO_CLK_ENABLE()                          __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()                         __HAL_RCC_GPIOB_CLK_DISABLE()

#define LED2_PIN                                        GPIO_PIN_1
#define LED2_GPIO_PORT                                  GPIOE
#define LED2_GPIO_CLK_ENABLE()                          __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                         __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED3_PIN                                        GPIO_PIN_14
#define LED3_GPIO_PORT                                  GPIOB
#define LED3_GPIO_CLK_ENABLE()                          __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()                         __HAL_RCC_GPIOB_CLK_DISABLE()

#define USER_BUTTON_PIN                                 GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                           GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQn                           EXTI15_10_IRQn
#define USER_BUTTON_IRQ_HANDLER                         EXTI15_10_IRQHandler
#define USER_BUTTON_IRQ_CALLBACK                        HAL_GPIO_EXTI_Callback
#define USER_BUTTON_EXTI_LINE                           EXTI_LINE_13

#define COM1_UART                                       USART3
#define COM1_CLK_ENABLE()                               __HAL_RCC_USART3_CLK_ENABLE()
#define COM1_CLK_DISABLE()                              __HAL_RCC_USART3_CLK_DISABLE()
#define COM1_TX_PIN                                     GPIO_PIN_8
#define COM1_TX_GPIO_PORT                               GPIOD
#define COM1_TX_GPIO_CLK_ENABLE()                       __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_TX_GPIO_CLK_DISABLE()                      __HAL_RCC_GPIOD_CLK_DISABLE()
#define COM1_TX_AF                                      GPIO_AF7_USART3
#define COM1_RX_PIN                                     GPIO_PIN_9
#define COM1_RX_GPIO_PORT                               GPIOD
#define COM1_RX_GPIO_CLK_ENABLE()                       __HAL_RCC_GPIOD_CLK_ENABLE()
#define COM1_RX_GPIO_CLK_DISABLE()                      __HAL_RCC_GPIOD_CLK_DISABLE()
#define COM1_RX_AF                                      GPIO_AF7_USART3
#define COM_POLL_TIMEOUT                                1000

#define TAPAS_UART_TMTC                                 USART2
#define TAPAS_UART_TMTC_DMA_RX                          hdma_usart2_rx
#define TAPAS_UART_TMTC_DMA_TX                          hdma_usart2_tx
#define TAPAS_UART_TMTC_IRQ_HANDLER                     USART2_IRQHandler
#define TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER              DMA1_Stream0_IRQHandler
#define TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER              DMA1_Stream1_IRQHandler

#define TAPAS_UART_PRINT                                USART3
#define TAPAS_UART_PRINT_IRQ_HANDLER                    USART3_IRQHandler

#define TAPAS_UART_PL                                   USART6
#define TAPAS_UART_PL_IRQ_HANDLER                       USART6_IRQHandler

#define TAPAS_I2C_AVIONIC                               I2C1
#define TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER               I2C1_EV_IRQHandler

#define TAPAS_SPI_SDCARD                                SPI1
#define TAPAS_SPI_SDCARD_IRQ_HANDLER                    SPI1_IRQHandler

#define TAPAS_SDCARD_PIN                                GPIO_PIN_4
#define TAPAS_SDCARD_GPIO_PORT                          GPIOA
#define TAPAS_SDCARD_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define TAPAS_SDCARD_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOA_CLK_DISABLE()

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

#endif /* STM32H7xx_NUCLEO_BSP_H */
