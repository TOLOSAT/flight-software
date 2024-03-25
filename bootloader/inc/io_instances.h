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

#include "generic_hal.h"
#include "tolosat_fs.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern spiInst_t spi_avionic_inst;
extern gpioInst_t led_inst;
extern gpioInst_t sd_card_gpio;
extern fsInst_t sd_fs_inst;

#endif /* IO_INSTANCES_H */