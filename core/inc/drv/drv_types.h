/**
 * @file    drv_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for HAL
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 */

#ifndef DRV_TYPES_H
#define DRV_TYPES_H

/******************************* Include Files *******************************/

#include "core_types.h"

#if defined(CONFIG_BOARD_ART_PI)
#include "art_pi_bsp.h"
#elif defined(CONFIG_BOARD_QEMU)
#include "qemu_bsp.h"
#elif defined(CONFIG_BOARD_NUCLEO_H745ZI)
#include "nucleo_h745zi_bsp.h"
#elif defined(CONFIG_BOARD_NUCLEO_F411RE)
#include "nucleo_f411re_bsp.h"
#elif defined(CONFIG_BOARD_DISCOVERY_F407VG)
#include "discovery_f407vg_bsp.h"
#else
#error "Board is not supported"
#endif

/***************************** Macros Definitions ****************************/

#define DRV_MAX_DELAY   30u /**< Max delay (in milliseconds) for function that uses HAL timer */

/***************************** Types Definitions *****************************/

#endif /* DRV_TYPES_H */

/**
 * @}
 * @}
 */