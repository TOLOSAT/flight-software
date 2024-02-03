/**
 * @file    io_instances.c
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking instance for IO's
 * @date    15/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef IO_INSTANCES_H
#define IO_INSTANCES_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "tolosat_hal.h"
#include "tolosat_fs.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_RX;
extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_TX;
extern iicInst_t iic_avionic_inst;
extern uartInst_t uart_tmtc_inst;
extern uartInst_t uart_print_inst;
extern uartInst_t uart_pl_inst;
extern spiInst_t spi_sdcard_inst;
extern gpioInst_t led_inst;
extern gpioInst_t user_button_inst;
extern gpioInst_t sd_card_cs;
extern fsInst_t sd_fs_inst;

#endif /* IO_INSTANCES_H */