/**
 * @file    io_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking instance for IO's
 * @date    15/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef IO_CONF_H
#define IO_CONF_H

/******************************* Include Files *******************************/

#include <stdint.h>
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

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_RX;
extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_TX;
extern iicInst_t iic_avionic_inst;
extern uartInst_t uart_tmtc_inst;
extern uartInst_t uart_print_inst;
extern uartInst_t uart_pl_inst;
extern gpioInst_t led_inst;
extern gpioInst_t user_button_inst;

#endif /* IO_CONF_H */