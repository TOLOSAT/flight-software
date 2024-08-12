/**
 * @file    generic_hal_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for HAL
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GENERIC_HAL_TYPES_H
#define GENERIC_HAL_TYPES_H

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>

#if defined(ART_PI)
#include "art_pi_bsp.h"
#elif defined(NUCLEO_H745ZI)
#include "nucleo_h745zi_bsp.h"
#elif defined(NUCLEO_F411RE)
#include "nucleo_f411re_bsp.h"
#elif defined(DISCOVERY_F407VG)
#include "discovery_f407vg_bsp.h"
#elif defined(QEMU)
#include "qemu_bsp.h"
#else
#error "Board is not supported"
#endif

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_GENERIC_HAL_TEXT_SECTION     __attribute__((section(".text_gen_hal")))   /**< Generic HAL functions goes in the .text_gen_hal */
#define IN_GENERIC_HAL_DATA_SECTION     __attribute__((section(".data_gen_hal")))   /**< Generic HAL data goes in the .data_gen_hal */

/***************************** Types Definitions *****************************/

/** 
 * @enum    halStatus_t
 * @brief   GENERIC HAL functions specific returns 
 */
typedef enum
{
    GEN_HAL_SUCCESSFUL = 0u,    /**< Function succeed */
    GEN_HAL_ERROR = 1u,         /**< Function failed */
    GEN_HAL_INVALID_PARAM = 2u, /**< Function parameter is not valid */
    GEN_HAL_TIMEOUT = 3u,       /**< Device timed out */
    GEN_HAL_BUSY = 4u,          /**< Device is busy */
} halStatus_t;

#endif /* GENERIC_HAL_TYPES_H */