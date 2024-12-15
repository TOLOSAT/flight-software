/**
 * @file    sysleds.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system LEDs handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup sysleds System LED
 * @brief Provides system LED handling.
 * @{
 */

#ifndef SYSLEDS_H
#define SYSLEDS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitSysLEDs(void);
extern void LEDStatToggle(void);
extern void LEDErrorOn(void);

#endif /* SYSLEDS_H */

/**
 * @}
 * @}
 * @}
 */