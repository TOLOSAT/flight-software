/**
 * @file    platform.c
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking platform informations
 * @date    30/04/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef IO_INSTANCES_H
#define IO_INSTANCES_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include "generic_hal.h"
#include "tolosat_fs.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern iicInst_t iic_avionic_inst;
extern uartInst_t uart_tmtc_inst;
extern uartInst_t uart_print_inst;
extern uartInst_t uart_pl_inst;
extern spiInst_t spi_avionic_inst;
extern owInst_t one_wire_inst;
extern gpioInst_t led_inst;
extern gpioInst_t user_button_inst;
extern gpioInst_t sd_card_gpio;
extern fsInst_t sd_fs_inst;

/*************************** Functions Declarations **************************/

uint32_t PlatformInit(void);

#endif /* IO_INSTANCES_H */