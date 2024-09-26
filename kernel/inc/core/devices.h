/**
 * @file    devices.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining devices
 * @date    19/08/2024
 * 
 * @copyright Copyright (c) TOLOSAT 2024
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

#define MAX_NUMBER_DEVICES  64u /**< Maximum number of devices that can be allocated */

#define IOCTL_LOCK_DEVICE       (-1u)    /**< Generic IOTCL command to lock the device linked file or peripheral */
#define IOCTL_UNLOCK_DEVICE     (-2u)    /**< Generic IOTCL command to unlock the device linked file or peripheral */
#define IOCTL_SET_EXTRA_INFO    (-3u)    /**< Generic IOTCL command to set device extra info */

/***************************** Types Definitions *****************************/

/** 
 * @enum    deviceDescStatus_t
 * @brief   Device descriptor status typedef enum
 */
typedef enum
{
    DEVICE_DESC_FREE = 0u,  /**< Device descriptor is free */
    DEVICE_DESC_USED = 1u,  /**< Device descriptor is already used */
} deviceDescStatus_t;

/** 
 * @enum    deviceType_t
 * @brief   Device type typedef enum
 */
typedef enum
{
    DEVICE_TYPE_BUFFER = 0u,        /**< Device is linked to a buffer */
    DEVICE_TYPE_FILE = 1u,          /**< Device is linked to a file */
    DEVICE_TYPE_PERIPHERAL = 2u,    /**< Device is linked to a peripheral */
} deviceType_t;

/** 
 * @struct  deviceDesc_t
 * @brief   Struct type of a device descriptors
 */
typedef struct
{
    deviceDescStatus_t status;  /**< @brief Device descriptor status (used for the device table) */
    deviceType_t type;          /**< @brief Device type (either buffer, file, peripheral) */
    uint32_t ressource;         /**< @brief The resource reference (buffer, file, or peripheral) */
    uint32_t extra_info;        /**< @brief Extra information (used when there are several physical devices on the same peripheral) */
} deviceDesc_t;

/*************************** Variables Declarations **************************/

extern deviceDesc_t g_devices_table[MAX_NUMBER_DEVICES];

/*************************** Functions Declarations **************************/

extern kernelStatus_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info);
extern kernelStatus_t DeviceWrite(deviceNo_t device, data_t data, length_t length);
extern kernelStatus_t DeviceRead(deviceNo_t device, data_t data, length_t length);
extern kernelStatus_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);
extern kernelStatus_t DeviceClose(deviceNo_t device);

#endif /* DEVICES_H */

/** 
 * @}
 * @}
 * @}
 */