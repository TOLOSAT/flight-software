/**
 * @file    mpu.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for MPU functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup mpu MPU
 * @brief Memory Protection Unit interface.
 * @{
 */

#ifndef MPU_H
#define MPU_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitMPU(void);

#endif /* MPU_H */

/**
 * @}
 * @}
 * @}
 */