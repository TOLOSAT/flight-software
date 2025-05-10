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
#define MONITORING_TIMER_REF        TIM3
#define MONITORING_TIMER_IRQ_NO     TIM3_IRQn

/* LED STATUS CONSTANTS */
#define LED_STATUS_PIN              GPIO_PIN_8
#define LED_STATUS_PORT             GPIOI
#define LED_STATUS_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()
#define LED_STATUS_CLK_DISABLE()     __HAL_RCC_GPIOI_CLK_DISABLE()

/* LED ERROR CONSTANTS */
#define LED_ERROR_PIN               GPIO_PIN_15
#define LED_ERROR_PORT              GPIOC
#define LED_ERROR_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_ERROR_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN             GPIO_PIN_4
#define USER_BUTTON_PORT            GPIOH
#define USER_BUTTON_CLK_ENABLE()      __HAL_RCC_GPIOH_CLK_ENABLE()
#define USER_BUTTON_CLK_DISABLE()     __HAL_RCC_GPIOH_CLK_DISABLE()
#define USER_BUTTON_EXTI_IRQ_NO     EXTI4_IRQn
#define USER_BUTTON_IRQ_HANDLER     EXTI4_IRQHandler
#define USER_BUTTON_EXTI_LINE       EXTI_LINE_4

/* UART TMTC CONSTANTS */
#define UART_TMTC_REF               UART4
#define UART_TMTC_IRQ_HANDLER       UART4_IRQHandler
#define UART_TMTC_IRQ_NO            UART4_IRQn
#define UART_TMTC_CLK_ENABLE()      __HAL_RCC_UART4_CLK_ENABLE()
#define UART_TMTC_CLK_DISABLE()     __HAL_RCC_UART4_CLK_DISABLE()
#define UART_TMTC_TX_PIN            GPIO_PIN_0
#define UART_TMTC_TX_PORT           GPIOA
#define UART_TMTC_RX_PIN            GPIO_PIN_9
#define UART_TMTC_RX_PORT           GPIOI

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX_REF        DMA1_Stream0
#define UART_TMTC_DMA_TX_REF        DMA1_Stream1
#define UART_TMTC_DMA_CLK_ENABLE()  __HAL_RCC_DMA1_CLK_ENABLE()
#define UART_TMTC_DMA_DISABLE()     __HAL_RCC_DMA1_CLK_DISABLE()
#define UART_TMTC_DMA_RX_IRQ_NO     DMA1_Stream0_IRQn
#define UART_TMTC_DMA_TX_IRQ_NO     DMA1_Stream1_IRQn
#define UART_TMTC_DMA_RX_CHANNEL    DMA_REQUEST_UART4_RX
#define UART_TMTC_DMA_TX_CHANNEL    DMA_REQUEST_UART4_TX

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
#define I2C_AVIONIC_REF             I2C4
#define I2C_AVIONIC_EVT_IRQ_HANDLER I2C4_EV_IRQHandler
#define I2C_AVIONIC_IRQ_NO          I2C4_EV_IRQn
#define I2C_AVIONIC_SCL_PIN         GPIO_PIN_11
#define I2C_AVIONIC_SCL_PORT        GPIOH
#define I2C_AVIONIC_SDA_PIN         GPIO_PIN_12
#define I2C_AVIONIC_SDA_PORT        GPIOH

/* SPI SD CARD CONSTANTS */
#define SPI_SD_CARD_REF             SPI4
#define SPI_SD_CARD_IRQ_HANDLER     SPI4_IRQHandler
#define SPI_SD_CARD_IRQ_NO          SPI4_IRQn
#define SPI_SD_CARD_SCK_PIN         GPIO_PIN_2
#define SPI_SD_CARD_SCK_PORT        GPIOE
#define SPI_SD_CARD_MISO_PIN        GPIO_PIN_5
#define SPI_SD_CARD_MISO_PORT       GPIO_E
#define SPI_SD_CARD_MOSI_PIN        GPIO_PIN_6
#define SPI_SD_CARD_MOSI_PORT       GPIOE

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                 GPIO_PIN_4
#define SD_PORT                     GPIOE

/* ONE WIRE CONSTANTS */
#define ONEWIRE_TIMER_REF           TIM5
#define ONEWIRE_TIMER_IRQ_NO        TIM5_IRQn
#define ONEWIRE_TIMER_CLK_ENABLE()  __HAL_RCC_TIM5_CLK_ENABLE()
#define ONEWIRE_PIN                 GPIO_PIN_15
#define ONEWIRE_PORT                GPIOA

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF                IWDG1

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
