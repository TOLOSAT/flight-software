/**
 * @file    drv_WDG.h
 * @author  Matteo Planchet
 * @author  Mathis Steinberger
 * @brief   Header file for WDG functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_WDG WDG Driver
 * @brief Abstraction layer for controlling watchdog.
 * @{
 */

#ifndef DRV_WDG_H
#define DRV_WDG_H

/******************************* Include Files *******************************/

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

returnCode_t InitWatchDog(uint32_t timeout_ms);
void PetWatchDog(void);

#endif /* DRV_WDG_H */

/**
 * @}
 * @}
 * @}
 */
