/**
 * @file    devices.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining devices
 * @date    19/08/2024
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup devices Devices Management
 * Functions that allows to manage devices in TAPAS
 * @{
 */

#ifndef DEVICES_H
#define DEVICES_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "peripherals.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Device Reference number type */
typedef uint32_t deviceNo_t;

/** @brief FS data type definition */
typedef uint8_t deviceData_t;

/** @brief Size type definition */
typedef uint32_t deviceSize_t;

/*************************** Variables Declarations **************************/

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
 */