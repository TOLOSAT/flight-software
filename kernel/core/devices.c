/**
 * @file    devices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining devices
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/devices.h"
#include "core/buffers.h"
#include "core/fs.h"
#include "drv/peripherals.h"
#include "system/sysdevices.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_devices_table
 * @brief   Devices descriptor table
 */
deviceDesc_t g_devices_table[CONFIG_MAX_NB_DEVICES] = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn          DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
 * @brief       Function that creates a device for a given peripheral
 * @param[out]  device      Device numero that has been allocated during the creation
 * @param[in]   type        Device type : either buffer, file or peripheral
 * @param[in]   resource   Buffer, file or peripheral to which to link
 * @retval      #RET_INVALID_PARAM if device is a null pointer or peripheral does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (device != NULL)
    {
        deviceNo_t new_device = 0u;

        // Look for an available device descriptor
        return_value = RET_NOT_AVAILABLE;
        while ((new_device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (return_value == RET_NOT_AVAILABLE))
        {
            // Check if descriptor free
            if (g_devices_table[new_device].status == DESC_FREE)
            {
                // Allocate new device
                g_devices_table[new_device].type     = type;
                g_devices_table[new_device].resource = resource;
                g_devices_table[new_device].status   = DESC_USED;
                *device                              = new_device;
                return_value                         = RET_SUCCESSFUL;
            }
            else
            {
                // Continue to look for a free device
                new_device++;
            }
        }

        // If no more device are available.
        if (return_value == RET_NOT_AVAILABLE)
        {
            KernelPanic();
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
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
            case DEVICE_TYPE_BUFFER :
                return_value = BufferWrite(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_FILE :
                return_value = FsWrite(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_PERIPHERAL :
                return_value = PeripheralWrite(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_SYSTEM :
                return_value = SystemDeviceWrite(g_devices_table[device].resource, data, length);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
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
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
            case DEVICE_TYPE_BUFFER :
                return_value = BufferRead(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_FILE :
                return_value = FsRead(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_PERIPHERAL :
                return_value = PeripheralRead(g_devices_table[device].resource, data, length);
                break;
            case DEVICE_TYPE_SYSTEM :
                return_value = SystemDeviceRead(g_devices_table[device].resource, data, length);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
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
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((device < (deviceNo_t)CONFIG_MAX_NB_DEVICES) && (g_devices_table[device].status != DESC_FREE))
    {
        switch (g_devices_table[device].type)
        {
            case DEVICE_TYPE_BUFFER :
                return_value = BufferIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            case DEVICE_TYPE_FILE :
                return_value = FsIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            case DEVICE_TYPE_PERIPHERAL :
                return_value = PeripheralIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            case DEVICE_TYPE_SYSTEM :
                return_value = SystemDeviceIoctl(g_devices_table[device].resource, cmd, data, data_size);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Reset device
    g_devices_table[device].resource = 0u;
    g_devices_table[device].status   = DESC_FREE;

    return return_value;
}