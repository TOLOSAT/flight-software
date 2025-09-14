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
#define HAL_TIMER_REF               TIM4
#define HAL_TIMER_IRQ_NO            TIM4_IRQn

/* LED STATUS CONSTANTS */
#define LED_STATUS_PIN              GPIO_PIN_8
#define LED_STATUS_PORT             GPIOI
#define LED_STATUS_CLK_ENABLE()     __HAL_RCC_GPIOI_CLK_ENABLE()
#define LED_STATUS_CLK_DISABLE()    __HAL_RCC_GPIOI_CLK_DISABLE()

/* LED ERROR CONSTANTS */
#define LED_ERROR_PIN               GPIO_PIN_15
#define LED_ERROR_PORT              GPIOC
#define LED_ERROR_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_ERROR_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN             GPIO_PIN_4
#define USER_BUTTON_PORT            GPIOH
#define USER_BUTTON_CLK_ENABLE()    __HAL_RCC_GPIOH_CLK_ENABLE()
#define USER_BUTTON_CLK_DISABLE()   __HAL_RCC_GPIOH_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQ_NO     EXTI4_IRQn
#define USER_BUTTON_IRQ_HANDLER     EXTI4_IRQHandler
#define USER_BUTTON_EXTI_LINE       EXTI_LINE_4

/* UART PRINT CONSTANTS */
#define UART_PRINT_REF              USART1
#define UART_PRINT_IRQ_HANDLER      USART1_IRQHandler
#define UART_PRINT_IRQ_NO           USART1_IRQn
#define UART_PRINT_CLK_ENABLE()     __HAL_RCC_USART1_CLK_ENABLE()
#define UART_PRINT_CLK_DISABLE()    __HAL_RCC_USART1_CLK_DISABLE()
#define UART_PRINT_TX_PIN           GPIO_PIN_9
#define UART_PRINT_TX_PORT          GPIOA
#define UART_PRINT_RX_PIN           GPIO_PIN_10
#define UART_PRINT_RX_PORT          GPIOA

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                 GPIO_PIN_4
#define SD_PORT                     GPIOE

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
