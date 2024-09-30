/**
 * @file    devices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining devices
 * @date    19/08/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/devices.h"
#include "core/buffers.h"
#include "fs/fs.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_devices_table
 * @brief   Devices descriptor table
 */
deviceDesc_t IN_DESC_TABLES_SECTION g_devices_table[CONFIG_MAX_NB_DEVICES] = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
 * @brief       Function that creates a device for a given peripheral
 * @param[out]  device      Device numero that has been allocated during the creation
 * @param[in]   type        Device type : either buffer, file or peripheral
 * @param[in]   ressource   Buffer, file or peripheral to which to link
 * @param[in]   extra_info  Extra information (used when there are several physical devices on the same peripheral)
 * @retval      #KERNEL_INVALID_PARAM if device is a null pointer or peripheral does not exist
 * @retval      #KERNEL_ERROR if no more device cannot be allocated (increase CONFIG_MAX_NB_DEVICES)
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (device != NULL)
    {
        // Look for an available device descriptor
        deviceNo_t new_device = 0u;
        return_value = KERNEL_ERROR;
        while ((new_device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (return_value == KERNEL_ERROR))
        {
            // Check if descriptor free
            if (g_devices_table[new_device].status == DEVICE_DESC_FREE)
            {
                // Allocate new device
                g_devices_table[new_device].type = type;
                g_devices_table[new_device].ressource = ressource;
                g_devices_table[new_device].extra_info = extra_info;
                g_devices_table[new_device].status = DEVICE_DESC_USED;
                *device = new_device;
                return_value = KERNEL_SUCCESSFUL;
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
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief       Function that writes data to a device
 * @param[in]   device  Device numero
 * @param[in]   data    Data that will be sent to the device
 * @param[in]   length  Length of the data
 * @retval      #KERNEL_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #KERNEL_ERROR if device writing encountered an error
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        kernelStatus_t test_lock = KERNEL_SUCCESSFUL;
        switch (g_devices_table[device].type)
        {
        case DEVICE_TYPE_BUFFER:
            return_value = BufferWrite(g_devices_table[device].ressource, data, length);
            break;
        case DEVICE_TYPE_FILE:
            // First lock file
            test_lock = FsLock(g_devices_table[device].ressource);
            if (test_lock == KERNEL_SUCCESSFUL)
            {
                // Then write
                return_value = FsWrite(g_devices_table[device].ressource, data, length);

                // Unlock whatever happened
                test_lock = FsUnlock(g_devices_table[device].ressource);
                if (test_lock != KERNEL_SUCCESSFUL)
                {
                    return_value = KERNEL_ERROR;
                }
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
            break;
        case DEVICE_TYPE_PERIPHERAL:
            // First lock peripheral
            test_lock = PeripheralLock(g_devices_table[device].ressource);
            if (test_lock == KERNEL_SUCCESSFUL)
            {
                // Then write
                return_value = PeripheralWrite(g_devices_table[device].ressource, data, length, g_devices_table[device].extra_info);

                // Unlock whatever happened
                test_lock = PeripheralUnlock(g_devices_table[device].ressource);
                if (test_lock != KERNEL_SUCCESSFUL)
                {
                    return_value = KERNEL_ERROR;
                }
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
            break;
        default:
            return_value = KERNEL_INVALID_PARAM;
            break;
        }
    }

    return return_value;
}

/**
 * @fn          DeviceRead(deviceNo_t device, data_t data, length_t length)
 * @brief       Function that reads data to a device
 * @param[in]   device  Device numero
 * @param[out]  data    Data that will be received to the device
 * @param[in]   length  Length of the data
 * @retval      #KERNEL_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #KERNEL_ERROR if device reading encountered an error
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        kernelStatus_t test_lock = KERNEL_SUCCESSFUL;
        switch (g_devices_table[device].type)
        {
        case DEVICE_TYPE_BUFFER:
            return_value = BufferRead(g_devices_table[device].ressource, data, length);
            break;
        case DEVICE_TYPE_FILE:
            // First lock file
            test_lock = FsLock(g_devices_table[device].ressource);
            if (test_lock == KERNEL_SUCCESSFUL)
            {
                // Then read
                return_value = FsRead(g_devices_table[device].ressource, data, length);

                // Unlock whatever happened
                test_lock = FsUnlock(g_devices_table[device].ressource);
                if (test_lock != KERNEL_SUCCESSFUL)
                {
                    return_value = KERNEL_ERROR;
                }
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
            break;
        case DEVICE_TYPE_PERIPHERAL:
            // First lock peripheral
            test_lock = PeripheralLock(g_devices_table[device].ressource);
            if (test_lock == KERNEL_SUCCESSFUL)
            {
                // Then read
                return_value = PeripheralRead(g_devices_table[device].ressource, data, length, g_devices_table[device].extra_info);

                // Unlock whatever happened
                test_lock = PeripheralUnlock(g_devices_table[device].ressource);
                if (test_lock != KERNEL_SUCCESSFUL)
                {
                    return_value = KERNEL_ERROR;
                }
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
            break;
        default:
            return_value = KERNEL_INVALID_PARAM;
            break;
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
 * @param[in]       data_size   Data length (if any)
 * @retval          #KERNEL_INVALID_PARAM if device is not valid
 * @retval          #KERNEL_ERROR if device IOCTL encountered an error
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
        case DEVICE_TYPE_BUFFER:
            return_value = KERNEL_INVALID_PARAM;
            break;
        case DEVICE_TYPE_FILE:
            // Check Generic IOTC
            if (cmd == IOCTL_LOCK_DEVICE)
            {
                // Just lock file
                return_value = FsLock(g_devices_table[device].ressource);
            }
            else if (cmd == IOCTL_UNLOCK_DEVICE)
            {
                // Just unlock file
                return_value = FsUnlock(g_devices_table[device].ressource);
            }
            else if (cmd == IOCTL_SET_EXTRA_INFO)
            {
                return_value = KERNEL_INVALID_PARAM;
            }
            else
            {
                // Lock peripheral
                kernelStatus_t test_lock = FsLock(g_devices_table[device].ressource);
                if (test_lock == KERNEL_SUCCESSFUL)
                {
                    // Then IOCTL
                    return_value = FsIoctl(g_devices_table[device].ressource, cmd, data, data_size);

                    // Unlock whatever happened
                    test_lock = FsUnlock(g_devices_table[device].ressource);
                    if (test_lock != KERNEL_SUCCESSFUL)
                    {
                        return_value = KERNEL_ERROR;
                    }
                }
                else
                {
                    return_value = KERNEL_ERROR;
                }
            }
            break;
        case DEVICE_TYPE_PERIPHERAL:
            // Check Generic IOTC
            if (cmd == IOCTL_LOCK_DEVICE)
            {
                // Just lock peripheral
                return_value = PeripheralLock(g_devices_table[device].ressource);
            }
            else if (cmd == IOCTL_UNLOCK_DEVICE)
            {
                // Just unlock peripheral
                return_value = PeripheralUnlock(g_devices_table[device].ressource);
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
                // Lock peripheral
                kernelStatus_t test_lock = PeripheralLock(g_devices_table[device].ressource);
                if (test_lock == KERNEL_SUCCESSFUL)
                {
                    // Then IOCTL
                    return_value = PeripheralIoctl(g_devices_table[device].ressource, cmd, data, data_size);

                    // Unlock whatever happened
                    test_lock = PeripheralUnlock(g_devices_table[device].ressource);
                    if (test_lock != KERNEL_SUCCESSFUL)
                    {
                        return_value = KERNEL_ERROR;
                    }
                }
                else
                {
                    return_value = KERNEL_ERROR;
                }
            }
            break;
        default:
            return_value = KERNEL_INVALID_PARAM;
            break;
        }
    }

    return return_value;
}

/**
 * @fn          DeviceClose(deviceNo_t device)
 * @brief       Function that will remove the device
 * @param[in]   device  Device numero
 * @retval      #KERNEL_SUCCESSFUL always
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DeviceClose(deviceNo_t device)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    g_devices_table[device].ressource = 0u;
    g_devices_table[device].extra_info = 0u;
    g_devices_table[device].status = DEVICE_DESC_FREE;

    return return_value;
}