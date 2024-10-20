/**
 * @file    boot_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for boot
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup boot Boot
 * @{
 */

#ifndef BOOT_TYPES_H
#define BOOT_TYPES_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "autoconf.h"
#include <ff.h>
#include <ffconf.h>
#include <ff_gen_drv.h>
#include "bsp.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

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

#endif /* BOOT_TYPES_H */

/**
 * @}
 */