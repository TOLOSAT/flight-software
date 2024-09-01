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

/**
 * @var     g_devices_table
 * @brief   Devices descriptor table
 */
deviceDesc_t IN_DESC_TABLES_SECTION g_devices_table[MAX_NUMBER_DEVICES] = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          DeviceOpen(deviceNo_t *device, peripheralNo_t peripheral, uint32_t extra_info)
 * @brief       Function that creates a device for a given peripheral
 * @param[out]  device      Device numero that has been allocated during the creation
 * @param[in]   peripheral  Peripheral to which to link
 * @param[in]   extra_info  Extra information (used when there are several physical devices on the same peripheral)
 * @retval      #CORE_INVALID_PARAM if device is a null pointer or peripheral does not exist
 * @retval      #CORE_ERROR if no more device cannot be allocated (increase MAX_NUMBER_DEVICES)
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceOpen(deviceNo_t *device, peripheralNo_t peripheral, uint32_t extra_info)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((device != NULL) && (peripheral < (peripheralNo_t)NB_PERIPHERALS))
    {
        // Look for an available device descriptor
        deviceNo_t new_device = 0u;
        return_value = CORE_ERROR;
        while ((new_device < MAX_NUMBER_DEVICES) && (return_value == CORE_ERROR))
        {
            // Check if descriptor free
            if (g_devices_table[new_device].status == DEVICE_DESC_FREE)
            {
                // Allocate new device
                g_devices_table[new_device].peripheral = peripheral;
                g_devices_table[new_device].extra_info = extra_info;
                g_devices_table[new_device].status = DEVICE_DESC_USED;
                *device = new_device;
                return_value = CORE_SUCCESSFUL;
            }
            else
            {
                // Continue to look for a free device
                new_device++;
            }
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          DeviceWrite(deviceNo_t device, deviceData_t *data, deviceSize_t size)
 * @brief       Function that writes data to a device
 * @param[in]   device  Device numero
 * @param[in]   data    Data that will be sent to the device
 * @param[in]   size    Size of the data
 * @retval      #CORE_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #CORE_ERROR if device writing encountered an error
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceWrite(deviceNo_t device, deviceData_t *data, deviceSize_t size)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (device < MAX_NUMBER_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        // First Lock Peripheral
        return_value = PeripheralLock(g_devices_table[device].peripheral);
        if (return_value == CORE_SUCCESSFUL)
        {
            // Then Write
            return_value = PeripheralWrite(g_devices_table[device].peripheral, data, size, g_devices_table[device].extra_info);
            if (return_value == CORE_SUCCESSFUL)
            {
                // Finally Unlock
                return_value = PeripheralUnlock(g_devices_table[device].peripheral);
            }
        }
    }

    return return_value;
}

/**
 * @fn          DeviceRead(deviceNo_t device, deviceData_t *data, deviceSize_t size)
 * @brief       Function that reads data to a device
 * @param[in]   device  Device numero
 * @param[out]  data    Data that will be received to the device
 * @param[in]   size    Size of the data
 * @retval      #CORE_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #CORE_ERROR if device reading encountered an error
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceRead(deviceNo_t device, deviceData_t *data, deviceSize_t size)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (device < MAX_NUMBER_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        // First Lock Peripheral
        return_value = PeripheralLock(g_devices_table[device].peripheral);
        if (return_value == CORE_SUCCESSFUL)
        {
            // Then Write
            return_value = PeripheralRead(g_devices_table[device].peripheral, data, size, g_devices_table[device].extra_info);
            if (return_value == CORE_SUCCESSFUL)
            {
                // Finally Unlock
                return_value = PeripheralUnlock(g_devices_table[device].peripheral);
            }
        }
    }

    return return_value;
}

/**
 * @fn              DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the device
 * @param[in]       device      Device numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data size (if any)
 * @retval          #CORE_INVALID_PARAM if device is not valid
 * @retval          #CORE_ERROR if device IOCTL encountered an error
 * @retval          #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((device < MAX_NUMBER_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        // Check Generic IOTC
        if (cmd == IOCTL_LOCK_PERIPHERAL)
        {
            // Just lock peripheral
            return_value = PeripheralLock(g_devices_table[device].peripheral);
        }
        else if (cmd == IOCTL_UNLOCK_PERIPHERAL)
        {
            // Just unlock peripheral
            return_value = PeripheralUnlock(g_devices_table[device].peripheral);
        }
        else if (cmd == IOCTL_SET_EXTRA_INFO)
        {
            if (data_size == sizeof(uint32_t))
            {
                g_devices_table[device].extra_info = *(uint32_t *)data;
            }
        }
        else
        {
            // Lock Peripheral
            return_value = PeripheralLock(g_devices_table[device].peripheral);
            if (return_value == CORE_SUCCESSFUL)
            {
                // Then IOCTL
                return_value = PeripheralIoctl(g_devices_table[device].peripheral, cmd, data, data_size);
                if (return_value == CORE_SUCCESSFUL)
                {
                    // Finally Unlock
                    return_value = PeripheralUnlock(g_devices_table[device].peripheral);
                }
            }
        }
    }

    return return_value;
}

/**
 * @fn          DeviceClose(deviceNo_t device)
 * @brief       Function that will remove the device  
 * @param[in]   device  Device numero
 * @retval      #CORE_SUCCESSFUL always
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceClose(deviceNo_t device)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    g_devices_table[device].peripheral = 0u;
    g_devices_table[device].extra_info = 0u;
    g_devices_table[device].status = DEVICE_DESC_FREE;

    return return_value;
}