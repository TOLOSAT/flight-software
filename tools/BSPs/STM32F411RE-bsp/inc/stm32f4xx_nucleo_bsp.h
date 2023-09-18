/**
 * @file    stm32f4xx_nucleo_bsp.h
 * @author  Merlin Kooshmanian
 * @date    16/04/2023
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef STM32F4xx_NUCLEO_BSP_H
#define STM32F4xx_NUCLEO_BSP_H

/******************************* Include Files *******************************/

#include "stm32f4xx_hal.h"

/***************************** Macros Definitions ****************************/

#define LED2_PIN                                        GPIO_PIN_5
#define LED2_GPIO_PORT                                  GPIOA
#define LED2_GPIO_CLK_ENABLE()                          __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                         __HAL_RCC_GPIOA_CLK_DISABLE()  

#define USER_BUTTON_PIN                                 GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                           GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()   
#define USER_BUTTON_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()  
#define USER_BUTTON_EXTI_LINE                           GPIO_PIN_13
#define USER_BUTTON_EXTI_IRQn                           EXTI15_10_IRQn
#define USER_BUTTON_IRQ_HANDLER                         EXTI15_10_IRQHandler
#define USER_BUTTON_IRQ_CALLBACK                        HAL_GPIO_EXTI_Callback


#define NUCLEO_SPIx                                     SPI1
#define NUCLEO_SPIx_CLK_ENABLE()                        __HAL_RCC_SPI1_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_AF                              GPIO_AF5_SPI1
#define NUCLEO_SPIx_SCK_GPIO_PORT                       GPIOA
#define NUCLEO_SPIx_SCK_PIN                             GPIO_PIN_5
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_MOSI_AF                        GPIO_AF5_SPI1
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT                 GPIOA
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()        __HAL_RCC_GPIOA_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                            GPIO_PIN_6
#define NUCLEO_SPIx_MOSI_PIN                            GPIO_PIN_7

#define NUCLEO_ADCx                                     ADC1
#define NUCLEO_ADCx_CLK_ENABLE()                        __HAL_RCC_ADC1_CLK_ENABLE()
#define NUCLEO_ADCx_CLK_DISABLE()                       __HAL_RCC_ADC1_CLK_DISABLE()
#define NUCLEO_ADCx_CHANNEL                             ADC_CHANNEL_8
#define NUCLEO_ADCx_GPIO_PORT                           GPIOB
#define NUCLEO_ADCx_GPIO_PIN                            GPIO_PIN_0
#define NUCLEO_ADCx_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define NUCLEO_ADCx_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOB_CLK_DISABLE()

#define TAPAS_UART_TMTC                                 USART2
#define TAPAS_UART_TMTC_DMA_RX                          hdma_usart2_rx
#define TAPAS_UART_TMTC_DMA_TX                          hdma_usart2_tx
#define TAPAS_UART_TMTC_IRQ_HANDLER                     USART2_IRQHandler
#define TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER              DMA1_Stream5_IRQHandler
#define TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER              DMA1_Stream6_IRQHandler

#define TAPAS_UART_PRINT                                USART1
#define TAPAS_UART_PRINT_IRQ_HANDLER                    USART1_IRQHandler

#define TAPAS_UART_PL                                   USART6
#define TAPAS_UART_PL_IRQ_HANDLER                       USART6_IRQHandler

#define TAPAS_I2C_AVIONIC                               I2C1
#define TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER               I2C1_EV_IRQHandler

#define TAPAS_SPI_SDCARD                                SPI2
#define TAPAS_SPI_SDCARD_IRQ_HANDLER                    SPI2_IRQHandler

#define TAPAS_SDCARD_PIN                                GPIO_PIN_12
#define TAPAS_SDCARD_GPIO_PORT                          GPIOB
#define TAPAS_SDCARD_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define TAPAS_SDCARD_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

#endif /* STM32F4xx_NUCLEO_BSP_H */
