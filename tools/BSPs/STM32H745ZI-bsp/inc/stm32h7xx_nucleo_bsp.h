/**
 * @file stm32h7xx_nucleo_bsp.h
 * @author Merlin Kooshmanian
 * @brief 
 * @version 0.1
 * @date 26/05/2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef STM32H7xx_NUCLEO_BSP_H
#define STM32H7xx_NUCLEO_BSP_H

/***************************** Include Files *********************************/

#include "stm32h7xx_hal.h"

/************************** Constant Definitions *****************************/

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

#define BUTTON_USER_PIN                                 GPIO_PIN_13
#define BUTTON_USER_GPIO_PORT                           GPIOC
#define BUTTON_USER_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUTTON_USER_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUTTON_USER_EXTI_IRQn                           EXTI15_10_IRQn
#define BUTTON_USER_EXTI_LINE                           EXTI_LINE_13

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

#define TOLOSAT_UART_TMTC                               USART2
#define TOLOSAT_UART_PRINT                              USART3
#define TOLOSAT_UART_PL                                 USART6
#define TOLOSAT_I2C_AVIONIC                             I2C1

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

#endif /* STM32H7xx_NUCLEO_BSP_H */
