/**
 * @file    sysled.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system LED handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup sysled Sysled
 * @brief Provides System LED handling.
 * @{
 */

#ifndef SYSLED_H
#define SYSLED_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitSysLED(void);
extern void LEDStatToggle(void);
extern void LEDErrorOn(void);

#endif /* SYSLED_H */

/** 
 * @}
 * @}
 * @}
 */