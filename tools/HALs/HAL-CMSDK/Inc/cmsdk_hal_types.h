/**
 * @file    hal_cmsdk_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for HAL CMSDK types
 * @date    09/06/2024
 */

#ifndef HAL_CMSDK_TYPES_H
#define HAL_CMSDK_TYPES_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include <stddef.h>
#include "CMSDK_CM7.h"

/***************************** Types Definitions *****************************/

/**
 * @brief  HAL Lock structures definition
 */
typedef enum
{
    HAL_UNLOCKED = 0u,
    HAL_LOCKED = 1u,
} HAL_LockTypeDef;

/**
 * @brief  HAL Status structures definition
 */
typedef enum
{
    HAL_OK = 0u,
    HAL_ERROR = 1u,
    HAL_BUSY = 2u,
    HAL_TIMEOUT = 3u,
} HAL_StatusTypeDef;

#endif /* HAL_CMSDK_TYPES_H */