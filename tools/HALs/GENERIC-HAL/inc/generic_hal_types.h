/**
 * @file    generic_hal_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for HAL
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef generic_hal_TYPES_H
#define generic_hal_TYPES_H

/******************************* Include Files *******************************/

#include <stdint.h>

#if defined(NUCLEO_H745ZI)
#include "stm32h7xx_hal.h"
#elif defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#include "stm32f4xx_hal.h"
#elif defined(NUCLEO_F103RB)
#include "stm32f1xx_hal.h"
#else 
#error "Board is not supported"
#endif

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

/** 
 * @struct  halIoCtlCmd_t
 * @brief   Generic struct type for IO control command
 */
typedef struct  {
    uint32_t cmd;       /**< @brief IO control command reference */
    uint32_t data_size; /**< @brief IO control data size (can be null) */
    void *data;         /**< @brief IO control data (inout) pointer (can be null) */
} halIoCtlCmd_t;

#endif /* generic_hal_TYPES_H */