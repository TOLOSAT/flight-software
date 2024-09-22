/**
 * @file    application_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for application types
 * @date    14/07/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef APPLICATION_TYPES_H
#define APPLICATION_TYPES_H

/******************************* Include Files *******************************/

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

/***************************** Types Definitions *****************************/

/** 
 * @enum    appStatus_t
 * @brief   Application functions specific returns 
 */
typedef enum
{
    APP_SUCCESSFUL     = 0u,    /**< Function succeed */
    APP_ERROR          = 1u,    /**< Function failed */
    APP_INVALID_PARAM  = 2u,    /**< Function parameter is not valid */
    APP_TIMEOUT        = 3u,    /**< Function returned a timeout */
} appStatus_t;

#endif /* APPLICATION_TYPES_H */