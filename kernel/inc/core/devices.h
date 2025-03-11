/**
 * @file    devices.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining devices
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup devices Devices Management
 * @brief Abstraction layer for managing devices using their corresponding peripheral.
 * @{
 */

#ifndef DEVICES_H
#define DEVICES_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/buffers.h"
#include "fs/fs.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    deviceDescStatus_t
 * @brief   Device descriptor status typedef enum
 */
typedef enum
{
    DEVICE_DESC_FREE = 0u, /**< Device descriptor is free */
    DEVICE_DESC_USED = 1u, /**< Device descriptor is already used */
} deviceDescStatus_t;

/**
 * @struct  deviceDesc_t
 * @brief   Struct type of a device descriptors
 */
typedef struct
{
    deviceDescStatus_t status; /**< @brief Device descriptor status (used for the device table) */
    deviceType_t type;         /**< @brief Device type (either buffer, file, peripheral) */
    uint32_t resource;         /**< @brief The resource reference (buffer, file, or peripheral) */
} deviceDesc_t;

/*************************** Variables Declarations **************************/

extern deviceDesc_t g_devices_table[CONFIG_MAX_NB_DEVICES];

/*************************** Functions Declarations **************************/

extern returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource);
extern returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length);
extern returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length);
extern returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t DeviceClose(deviceNo_t device);

#endif /* DEVICES_H */

/**
 * @}
 * @}
 * @}
 */