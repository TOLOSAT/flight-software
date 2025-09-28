/**
 * @file    syswdg.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system watchdog handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup syswdg System Watchdog
 * @brief Provides system watchdog handling interface.
 * @{
 */

#ifndef SYSWDG_H
#define SYSWDG_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitSYSWDG(void);
extern void SYSWDGMain(void);

#endif /* SYSWDG_H */

/**
 * @}
 * @}
 * @}
 */