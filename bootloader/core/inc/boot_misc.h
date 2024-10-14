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

#include "boot_types.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF                   0x00u       /**< Disk0 reference */

/***************************** Types Definitions *****************************/

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

#endif /* BOOT_MISC_H */
