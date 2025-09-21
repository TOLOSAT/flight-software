/**
 * @file    sysdevices.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system devices handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup sysdevices System Devices
 * @brief Provides system devices handling.
 * @{
 */

#ifndef SYSDEVICE_H
#define SYSDEVICE_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief System device reference number type */
typedef uint32_t systemDeviceNo_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t SystemDeviceWrite(systemDeviceNo_t sysdev, data_t data, length_t length);
extern returnCode_t SystemDeviceRead(systemDeviceNo_t sysdev, data_t data, length_t length);
extern returnCode_t SystemDeviceIoctl(systemDeviceNo_t sysdev, uint32_t cmd, void *data, uint32_t data_size);

#endif /* SYSDEVICE_H */

/**
 * @}
 * @}
 * @}
 */