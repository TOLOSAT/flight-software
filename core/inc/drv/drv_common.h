/**
 * @file    drv_common.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for drivers common functions (e.g. HAL init)
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef DRV_COMMON_H
#define DRV_COMMON_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t InitHal(void);

#endif /* DRV_COMMON_H */