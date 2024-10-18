/**
 * @file    devices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining devices
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
 * @retval      #RET_INVALID_PARAM if device is a null pointer or peripheral does not exist
 * @retval      #RET_NOT_AVAILABLE if no more device can be allocated (solution : increase CONFIG_MAX_NB_DEVICES)
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (device != NULL)
    {
        // Look for an available device descriptor
        deviceNo_t new_device = 0u;
        return_value = RET_NOT_AVAILABLE;
        while ((new_device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (return_value == RET_NOT_AVAILABLE))
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
                return_value = RET_SUCCESSFUL;
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
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief       Function that writes data to a device
 * @param[in]   device  Device numero
 * @param[in]   data    Data that will be sent to the device
 * @param[in]   length  Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #RET_TIMEOUT if writing the device timeouted before sending all data (data sent may be incomplete)
 * @retval      #RET_NOT_AVAILABLE if writing the device is still occuring (data are not fully sent yet)
 * @retval      #RET_ERROR if device writing encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        returnCode_t test_lock = RET_SUCCESSFUL;
        switch (g_devices_table[device].type)
        {
        case DEVICE_TYPE_BUFFER:
            return_value = BufferWrite(g_devices_table[device].ressource, data, length);
            break;
        case DEVICE_TYPE_FILE:
            // First lock file
            test_lock = FsLock(g_devices_table[device].ressource);
            if (test_lock == RET_SUCCESSFUL)
            {
                // Then write
                return_value = FsWrite(g_devices_table[device].ressource, data, length);

                // Unlock whatever happened
                test_lock = FsUnlock(g_devices_table[device].ressource);
                if (test_lock != RET_SUCCESSFUL)
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
            break;
        case DEVICE_TYPE_PERIPHERAL:
            // First lock peripheral
            test_lock = PeripheralLock(g_devices_table[device].ressource);
            if (test_lock == RET_SUCCESSFUL)
            {
                // Then write
                return_value = PeripheralWrite(g_devices_table[device].ressource, data, length, g_devices_table[device].extra_info);

                // Unlock whatever happened
                test_lock = PeripheralUnlock(g_devices_table[device].ressource);
                if (test_lock != RET_SUCCESSFUL)
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
            break;
        default:
            return_value = RET_INVALID_PARAM;
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
 * @retval      #RET_INVALID_PARAM if data is a null pointer or device is not valid
 * @retval      #RET_TIMEOUT if reading the device timeouted before receiving any data
 * @retval      #RET_NOT_AVAILABLE if reading the device is still occuring (data are not available yet)
 * @retval      #RET_ERROR if device reading encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        returnCode_t test_lock = RET_SUCCESSFUL;
        switch (g_devices_table[device].type)
        {
        case DEVICE_TYPE_BUFFER:
            return_value = BufferRead(g_devices_table[device].ressource, data, length);
            break;
        case DEVICE_TYPE_FILE:
            // First lock file
            test_lock = FsLock(g_devices_table[device].ressource);
            if (test_lock == RET_SUCCESSFUL)
            {
                // Then read
                return_value = FsRead(g_devices_table[device].ressource, data, length);

                // Unlock whatever happened
                test_lock = FsUnlock(g_devices_table[device].ressource);
                if (test_lock != RET_SUCCESSFUL)
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
            break;
        case DEVICE_TYPE_PERIPHERAL:
            // First lock peripheral
            test_lock = PeripheralLock(g_devices_table[device].ressource);
            if (test_lock == RET_SUCCESSFUL)
            {
                // Then read
                return_value = PeripheralRead(g_devices_table[device].ressource, data, length, g_devices_table[device].extra_info);

                // Unlock whatever happened
                test_lock = PeripheralUnlock(g_devices_table[device].ressource);
                if (test_lock != RET_SUCCESSFUL)
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
            break;
        default:
            return_value = RET_INVALID_PARAM;
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
 * @retval          #RET_INVALID_PARAM if device is not valid
 * @retval          #RET_TIMEOUT if ioctl the device timeouted before receiving any data
 * @retval          #RET_NOT_AVAILABLE if ioctl the device is still occuring
 * @retval          #RET_ERROR if device IOCTL encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
        case DEVICE_TYPE_BUFFER:
            return_value = BufferIoctl(g_devices_table[device].ressource, cmd, data, data_size);
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
                return_value = RET_INVALID_PARAM;
            }
            else
            {
                // Lock peripheral
                returnCode_t test_lock = FsLock(g_devices_table[device].ressource);
                if (test_lock == RET_SUCCESSFUL)
                {
                    // Then IOCTL
                    return_value = FsIoctl(g_devices_table[device].ressource, cmd, data, data_size);

                    // Unlock whatever happened
                    test_lock = FsUnlock(g_devices_table[device].ressource);
                    if (test_lock != RET_SUCCESSFUL)
                    {
                        return_value = RET_ERROR;
                    }
                }
                else
                {
                    return_value = RET_ERROR;
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
                returnCode_t test_lock = PeripheralLock(g_devices_table[device].ressource);
                if (test_lock == RET_SUCCESSFUL)
                {
                    // Then IOCTL
                    return_value = PeripheralIoctl(g_devices_table[device].ressource, cmd, data, data_size);

                    // Unlock whatever happened
                    test_lock = PeripheralUnlock(g_devices_table[device].ressource);
                    if (test_lock != RET_SUCCESSFUL)
                    {
                        return_value = RET_ERROR;
                    }
                }
                else
                {
                    return_value = RET_ERROR;
                }
            }
            break;
        default:
            return_value = RET_INVALID_PARAM;
            break;
        }
    }

    return return_value;
}

/**
 * @fn          DeviceClose(deviceNo_t device)
 * @brief       Function that will remove the device
 * @param[in]   device  Device numero
 * @retval      #RET_SUCCESSFUL always
 */
returnCode_t DeviceClose(deviceNo_t device)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    g_devices_table[device].ressource = 0u;
    g_devices_table[device].extra_info = 0u;
    g_devices_table[device].status = DEVICE_DESC_FREE;

    return return_value;
}