/**
 * @file io_conf.c
 * @author Merlin Kooshmanian
 * @brief Source file stocking instance for IO's
 * @date 15/07/2023
 *
 * Last Update : 15/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/io_conf.h"
#include "tolosat_hal.h"
#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#elif defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#elif defined(STM32H745xx)
#include "stm32h7xx_nucleo_bsp.h"
#else
#error "Board is not supported"
#endif

/*************************** Variables Definitions ***************************/

extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_RX;
extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_TX;
extern iicInst_t iic_avionic_inst;
extern uartInst_t uart_tmtc_inst;
extern uartInst_t uart_print_inst;
extern uartInst_t uart_pl_inst;
extern gpioInst_t led_inst;
extern gpioInst_t user_button_inst;
extern rtcInst_t rtc_inst;

/***************************** Configuration *********************************/

/**
 * @var     TAPAS_UART_TMTC_DMA_RX
 * @brief   DMA USART1 RX instance declaration
 */
DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_RX = {0};

/**
 * @var     TAPAS_UART_TMTC_DMA_TX
 * @brief   DMA USART1 TX instance declaration
 */
DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_TX = {0};

/**
 * @var     iic_avionic_inst
 * @brief   I2C avionic instance declaration
 */
iicInst_t iic_avionic_inst = {
    .iic_ref = TAPAS_I2C_AVIONIC,
    .drive_type = IIC_IT_MASTER_DRIVE,
};

/**
 * @var     uart_tmtc_inst
 * @brief   UART tmtc instance declaration
 */
uartInst_t uart_tmtc_inst = {
    .uart_ref = TAPAS_UART_TMTC,
    .drive_type = UART_DMA_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     uart_print_inst
 * @brief   UART print instance declaration
 */
uartInst_t uart_print_inst = {
    .uart_ref = TAPAS_UART_PRINT,
    .drive_type = UART_POLLING_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     uart_pl_inst
 * @brief   UART payload instance declaration
 */
uartInst_t uart_pl_inst = {
    .uart_ref = TAPAS_UART_PL,
    .drive_type = UART_INTERRUPT_DRIVE,
    .baud_rate = 115200,
};

/**
 * @var     led_inst
 * @brief   GPIO led instance declaration
 */
gpioInst_t led_inst = {
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     user_button_inst
 * @brief   GPIO button instance declaration
 */
gpioInst_t user_button_inst = {
    .mode = GPIO_MODE_IT_FALLING,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
};

/**
 * @var     rtc_inst
 * @brief   RTC instance declaration
 */
rtcInst_t rtc_inst = {0};