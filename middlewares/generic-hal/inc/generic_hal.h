/**
 * @file    generic_hal.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all GENERIC HAL headers
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GENERIC_HAL_H
#define GENERIC_HAL_H

/******************************* Include Files *******************************/

#if defined(QEMU)
#include "generic_hal_types.h"
#include "generic_hal_init.h"
#include "generic_hal_gpio.h"
#include "generic_hal_uart.h"
#include "generic_hal_tim.h"
#include "generic_hal_rtc.h"
#include "generic_hal_ecc.h"
#else
#include "generic_hal_types.h"
#include "generic_hal_init.h"
#include "generic_hal_gpio.h"
#include "generic_hal_uart.h"
#include "generic_hal_tim.h"
#include "generic_hal_i2c.h"
#include "generic_hal_spi.h"
#include "generic_hal_ow.h"
#include "generic_hal_rtc.h"
#include "generic_hal_ecc.h"
#endif

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

#endif /* GENERIC_HAL_H */