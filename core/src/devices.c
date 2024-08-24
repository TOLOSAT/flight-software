/**
 * @file    devices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining devices
 * @date    19/08/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          DeviceOpen(deviceNo_t *device, peripheralNo_t peripheral, uint32_t extra_info)
 * @brief       Function that creates a device for a given peripheral
 * @param[out]  device      Device numero that has been allocated during the creation
 * @param[in]   peripheral  Peripheral to which to link
 * @param[in]   extra_info  Extra information (used when there are several physical devices on the same peripheral)
 * @retval      TODO
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceOpen(deviceNo_t *device, peripheralNo_t peripheral, uint32_t extra_info)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(device);
    (void)(peripheral);
    (void)(extra_info);

    return return_value;
}

/**
 * @fn          DeviceWrite(deviceNo_t device, deviceData_t *data, deviceSize_t size)
 * @brief       Function that writes data to a device
 * @param[in]   device  Device numero
 * @param[in]   data    Data that will be sent to the device
 * @param[in]   size    Size of the data
 * @retval      TODO
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceWrite(deviceNo_t device, deviceData_t *data, deviceSize_t size)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(device);
    (void)(data);
    (void)(size);

    return return_value;
}

/**
 * @fn          DeviceRead(deviceNo_t device, deviceData_t *data, deviceSize_t size)
 * @brief       Function that reads data to a device
 * @param[in]   device  Device numero
 * @param[out]  data    Data that will be received to the device
 * @param[in]   size    Size of the data
 * @retval      TODO
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceRead(deviceNo_t device, deviceData_t *data, deviceSize_t size)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(device);
    (void)(data);
    (void)(size);

    return return_value;
}

/**
 * @fn              DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the device
 * @param[in]       device      Device numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data size (if any)
 * @retval          TODO
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(device);
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    return return_value;
}

/**
 * @fn          DeviceClose(deviceNo_t device)
 * @brief       Function that will remove the device  
 * @param[in]   device  Device numero
 * @retval      TODO
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceClose(deviceNo_t device)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(device);

    return return_value;
}