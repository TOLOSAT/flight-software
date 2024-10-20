/**
 * @file    drv_common.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for drivers common functions (e.g. HAL init)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 */

#ifndef DRV_COMMON_H
#define DRV_COMMON_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitHal(void);

#endif /* DRV_COMMON_H */

/**
 * @}
 * @}
 */