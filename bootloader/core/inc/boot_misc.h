/**
 * @file    boot_misc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT boot types and defines
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BOOT_MISC_H
#define BOOT_MISC_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include <stddef.h>
#include "ff_gen_drv.h"

#if defined(ART_PI)
#include "art_pi_bsp.h"
#elif defined(NUCLEO_H745ZI)
#include "nucleo_h745zi_bsp.h"
#else
#error "Board is not supported"
#endif

/***************************** Macros Definitions ****************************/

#define DISK0_REF                   0x00u       /**< Disk0 reference */

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/** 
 * @enum    coreStatus_t
 * @brief   Boot functions specific returns 
 */
typedef enum
{
    CORE_SUCCESSFUL     = 0u,    /**< Function succeed */
    CORE_ERROR          = 1u,    /**< Function failed */
    CORE_INVALID_PARAM  = 2u,    /**< Function parameter is not valid */
    CORE_TIMEOUT        = 3u,    /**< Function returned a timeout */
    CORE_BUSY           = 4u,    /**< Function's ressource is busy */
} coreStatus_t;

#endif /* BOOT_MISC_H */
