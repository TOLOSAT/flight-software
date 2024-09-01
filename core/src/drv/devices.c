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
    if ((data != NULL) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        // First get peripheral and type
        peripheralNo_t peripheral = g_devices_table[device].peripheral;
        peripheralType_t type = g_peripherals_desc_table[peripheral].type;

        // Then use the correct driver to write
        switch (type)
        {
        case PERIPHERALS_GPIO:
            if (size == sizeof(deviceData_t))
            {
                return_value = GpioWrite((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, *data);
            }
            else
            {
                return_value = CORE_ERROR;
            }
            break;
        case PERIPHERALS_UART:
            return_value = UartWrite((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        case PERIPHERALS_I2C:
            return_value = I2cWrite((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, g_devices_table[device].extra_info, data, size);
            break;
        case PERIPHERALS_SPI:
            return_value = SpiWrite((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        case PERIPHERALS_OW:
            return_value = OwWrite((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        default:
            return_value = CORE_ERROR;
            break;
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
    if ((data != NULL) && (g_devices_table[device].status != DEVICE_DESC_FREE))
    {
        // First get peripheral and type
        peripheralNo_t peripheral = g_devices_table[device].peripheral;
        peripheralType_t type = g_peripherals_desc_table[peripheral].type;

        // Then use the correct driver to read
        switch (type)
        {
        case PERIPHERALS_GPIO:
            if (size == sizeof(deviceData_t))
            {
                return_value = GpioRead((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, data);
            }
            else
            {
                return_value = CORE_ERROR;
            }
            break;
        case PERIPHERALS_UART:
            return_value = UartRead((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        case PERIPHERALS_I2C:
            return_value = I2cRead((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, g_devices_table[device].extra_info, data, size);
            break;
        case PERIPHERALS_SPI:
            return_value = SpiRead((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, NULL, size); // To do : improve
            break;
        case PERIPHERALS_OW:
            return_value = OwRead((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        default:
            return_value = CORE_ERROR;
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
 * @param[in]       data_size   Data size (if any)
 * @retval          TODO
 */
coreStatus_t IN_CORE_TEXT_SECTION DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
// Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (g_devices_table[device].status != DEVICE_DESC_FREE)
    {
        // First get peripheral and type
        peripheralNo_t peripheral = g_devices_table[device].peripheral;
        peripheralType_t type = g_peripherals_desc_table[peripheral].type;

        // Then use the correct driver to write
        switch (type)
        {
        case PERIPHERALS_GPIO:
            return_value = GpioIoctl((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
            break;
        case PERIPHERALS_UART:
            return_value = UartIoctl((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
            break;
        case PERIPHERALS_I2C:
            return_value = I2cIoctl((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
            break;
        case PERIPHERALS_SPI:
            return_value = SpiIoctl((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
            break;
        case PERIPHERALS_OW:
            return_value = OwIoctl((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
            break;
        default:
            return_value = CORE_ERROR;
            break;
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