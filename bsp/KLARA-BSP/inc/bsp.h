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
#define LED_STATUS_PIN              GPIO_PIN_1
#define LED_STATUS_PORT             GPIOB

/* LED ERROR CONSTANTS */
#define LED_ERROR_PIN               GPIO_PIN_0
#define LED_ERROR_PORT              GPIOB

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN             GPIO_PIN_2
#define USER_BUTTON_PORT            GPIOB
#define USER_BUTTON_EXTI_IRQ_NO     EXTI2_IRQn
#define USER_BUTTON_IRQ_HANDLER     EXTI2_IRQHandler
#define USER_BUTTON_EXTI_LINE       EXTI_LINE_2

/* UART TMTC CONSTANTS */
#define UART_TMTC_REF               UART4
#define UART_TMTC_IRQ_HANDLER       UART4_IRQHandler
#define UART_TMTC_IRQ_NO            UART4_IRQn
#define UART_TMTC_CLK_SRC           RCC_USART234578CLKSOURCE_D2PCLK1
#define UART_TMTC_TX_PORT           GPIOA
#define UART_TMTC_TX_PIN            GPIO_PIN_0
#define UART_TMTC_TX_MODE           GPIO_MODE_AF_PP
#define UART_TMTC_TX_PULL           GPIO_NOPULL
#define UART_TMTC_TX_SPEED          GPIO_SPEED_FREQ_LOW
#define UART_TMTC_TX_ALT            GPIO_AF8_UART4
#define UART_TMTC_RX_PORT           GPIOA
#define UART_TMTC_RX_PIN            GPIO_PIN_1
#define UART_TMTC_RX_MODE           GPIO_MODE_AF_PP
#define UART_TMTC_RX_PULL           GPIO_NOPULL
#define UART_TMTC_RX_SPEED          GPIO_SPEED_FREQ_LOW
#define UART_TMTC_RX_ALT            GPIO_AF8_UART4

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX_REF        DMA1_Stream0
#define UART_TMTC_DMA_TX_REF        DMA1_Stream1
#define UART_TMTC_DMA_RX_IRQ_NO     DMA1_Stream0_IRQn
#define UART_TMTC_DMA_TX_IRQ_NO     DMA1_Stream1_IRQn
#define UART_TMTC_DMA_RX_CHANNEL    DMA_REQUEST_UART4_RX
#define UART_TMTC_DMA_TX_CHANNEL    DMA_REQUEST_UART4_TX

/* UART PL CONSTANTS */
#define UART_PL_REF                 UART7
#define UART_PL_IRQ_HANDLER         UART7_IRQHandler
#define UART_PL_IRQ_NO              UART7_IRQn
#define UART_PL_CLK_SRC             RCC_USART234578CLKSOURCE_D2PCLK1
#define UART_PL_TX_PORT             GPIOF
#define UART_PL_TX_PIN              GPIO_PIN_7
#define UART_PL_TX_MODE             GPIO_MODE_AF_PP
#define UART_PL_TX_PULL             GPIO_NOPULL
#define UART_PL_TX_SPEED            GPIO_SPEED_FREQ_LOW
#define UART_PL_TX_ALT              GPIO_AF7_UART7
#define UART_PL_RX_PORT             GPIOA
#define UART_PL_RX_PIN              GPIO_PIN_8
#define UART_PL_RX_MODE             GPIO_MODE_AF_PP
#define UART_PL_RX_PULL             GPIO_NOPULL
#define UART_PL_RX_SPEED            GPIO_SPEED_FREQ_LOW
#define UART_PL_RX_ALT              GPIO_AF11_UART7

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC_REF             I2C1
#define I2C_AVIONIC_EVT_IRQ_HANDLER I2C1_EV_IRQHandler
#define I2C_AVIONIC_IRQ_NO          I2C1_EV_IRQn
#define I2C_AVIONIC_SCL_PIN         GPIO_PIN_6
#define I2C_AVIONIC_SCL_PORT        GPIOB
#define I2C_AVIONIC_SDA_PIN         GPIO_PIN_7
#define I2C_AVIONIC_SDA_PORT        GPIOB

/* ONE WIRE CONSTANTS */
#define ONEWIRE_TIMER_REF           TIM5
#define ONEWIRE_TIMER_IRQ_NO        TIM5_IRQn
#define ONEWIRE_PIN                 GPIO_PIN_2
#define ONEWIRE_PORT                GPIOA

/* NAND CONSTANTS */
#define NAND_PAGE_SIZE              4096U
#define NAND_SPARE_AREA_SIZE        256U
#define NAND_BLOCK_SIZE_IN_PAGES    64U
#define NAND_PLANE_SIZE_IN_BLOCKS   2048U
#define NAND_BLOCK_COUNT            4096
#define NAND_PLANE_COUNT            2U
#define NAND_TCLR_SETUP_TIME        2U
#define NAND_TAR_SETUP_TIME         2U
#define NAND_TIMING_SETUP_TIME      0U
#define NAND_TIMING_WAIT_TIME       1U
#define NAND_TIMING_HOLD_TIME       1U
#define NAND_TIMING_HIZ_TIME        4U
#define NAND_ECC_COMPUTATION        FMC_NAND_ECC_DISABLE
#define NAND_ECC_PAGE_SIZE          FMC_NAND_ECC_PAGE_SIZE_4096BYTE
#define NAND_MEMORY_BUS_WIDTH       FMC_NAND_MEM_BUS_WIDTH_8
#define NAND_WAIT_FEATURE           FMC_NAND_WAIT_FEATURE_ENABLE
#define NAND_BANK                   FMC_NAND_BANK3
#define NAND_EXTRA_COMMAND          DISABLE
#define NAND_WP_PIN                 GPIO_PIN_10
#define NAND_WP_PORT                GPIOD
#define NAND_WP_CLK_ENABLE()        __HAL_RCC_GPIOD_CLK_ENABLE()

/* WATCHDOG CONSTANTS */
#define WATCHDOG_REF                IWDG1

/***************************** Types Definitions *****************************/

/*************************** Functions Declarations **************************/

returnCode_t SystemClock_Config(void);
void BSPLateInit(void);

#endif /* BSP_H */
