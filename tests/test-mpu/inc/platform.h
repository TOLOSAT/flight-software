/**
 * @file    platform.h
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
#include "fs.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern gpioInst_t led_inst;
extern gpioInst_t user_button_inst;
extern uartInst_t uart_print_inst;

/*************************** Functions Declarations **************************/

extern uint32_t PlatformInit(void);

#endif /* IO_INSTANCES_H */