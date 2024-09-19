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

#include "core_types.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

#define MAX_NUMBER_DEVICES  64u /**< Maximum number of devices that can be allocated */

#define IOCTL_LOCK_PERIPHERAL       (-1u)    /**< Generic IOTCL command to lock the peripheral */
#define IOCTL_UNLOCK_PERIPHERAL     (-2u)    /**< Generic IOTCL command to unlock the peripheral */
#define IOCTL_SET_EXTRA_INFO        (-3u)    /**< Generic IOTCL command to set device extra info */

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

/** @brief Device Reference number type */
typedef uint32_t deviceNo_t;

/** @brief Device data type definition */
typedef uint8_t deviceData_t;

/** @brief Device data size type definition */
typedef uint32_t deviceSize_t;

/** 
 * @struct  deviceDesc_t
 * @brief   Struct type of a device descriptors
 */
typedef struct
{
    deviceDescStatus_t status;  /**< @brief Device descriptor status (used for the device table) */
    peripheralNo_t peripheral;  /**< @brief Peripheral linked to the device */
    uint32_t extra_info;        /**< @brief Extra information (used when there are several physical devices on the same peripheral) */
} deviceDesc_t;

/*************************** Variables Declarations **************************/

extern deviceDesc_t g_devices_table[MAX_NUMBER_DEVICES];

/*************************** Functions Declarations **************************/

extern coreStatus_t DeviceOpen(deviceNo_t *device, peripheralNo_t peripheral, uint32_t extra_info);
extern coreStatus_t DeviceWrite(deviceNo_t device, deviceData_t *data, deviceSize_t size);
extern coreStatus_t DeviceRead(deviceNo_t device, deviceData_t *data, deviceSize_t size);
extern coreStatus_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);
extern coreStatus_t DeviceClose(deviceNo_t device);

#endif /* DEVICES_H */

/** 
 * @}
 * @}
 * @}
 */