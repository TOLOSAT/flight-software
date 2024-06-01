/**
 * @file    core_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for core
 * @date    06/05/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>

/***************************** Types Definitions *****************************/

/** 
 * @enum    coreStatus_t
 * @brief   Core functions specific returns 
 */
typedef enum
{
    CORE_SUCCESSFUL     = 0u,    /**< Function succeed */
    CORE_ERROR          = 1u,    /**< Function failed */
    CORE_INVALID_PARAM  = 2u,    /**< Function parameter is not valid */
    CORE_TIMEOUT        = 3u,    /**< Function returned a timeout */
} coreStatus_t;

#endif /* CORE_TYPES_H */