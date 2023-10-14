/**
 * @file    tolosat_hal.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all TOLOSAT HAL headers
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TOLOSAT_HAL_H
#define TOLOSAT_HAL_H

/******************************* Include Files *******************************/

#if defined(NUCLEO_H745ZI)
#include "nucleo_h745zi_bsp.h"
#elif defined(NUCLEO_F411RE)
#include "nucleo_f411re_bsp.h"
#elif defined(DISCOVERY_F407VG)
#include "discovery_f407vg_bsp.h"
#elif defined(NUCLEO_F103RB)
#include "nucleo_f103rb_bsp.h"
#else
#error "Board is not supported"
#endif

#include "tolosat_hal_types.h"
#include "tolosat_hal_init.h"
#include "tolosat_hal_gpio.h"
#include "tolosat_hal_uart.h"
#include "tolosat_hal_iic.h"
#include "tolosat_hal_spi.h"
#include "tolosat_hal_rtc.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

#endif /* TOLOSAT_HAL_H */