/**
 * @file    sysdevices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for system devices handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "system/sysdevices.h"
#include "system/sysinfo.h"
#include "system/sysmon.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          SystemDeviceWrite(systemDeviceNo_t sysdev, data_t data, length_t length)
 * @brief       Function that writes information onto a system device
 * @param[in]   sysdev      System device numero
 * @param[in]   data        Data that will be write onto the system device
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or length is zero
 * @retval      #RET_INVALID_PARAM if the system device does not exist
 * @retval      #RET_NOT_AVAILABLE if cannot write into the system device (because read-only)
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SystemDeviceWrite(systemDeviceNo_t sysdev, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused for the moment
    (void)(data);
    (void)(length);

    // Function Core
    if ((data != NULL) && (length != 0u))
    {
        switch (sysdev)
        {
            case SYSDEV_SYSTEM_INFO:
                // System information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_USAGE:
                // System information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            default:
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
 * @fn          SystemDeviceRead(systemDeviceNo_t sysdev, data_t data, length_t length)
 * @brief       Function that reads information from a system device
 * @param[in]   sysdev      System device numero
 * @param[in]   data        Data that will be read from the system device
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or length is zero
 * @retval      #RET_INVALID_PARAM if the system device does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SystemDeviceRead(systemDeviceNo_t sysdev, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (length != 0u))
    {
        switch (sysdev)
        {
            case SYSDEV_SYSTEM_INFO:
                // Check size
                if (length == sizeof(sytemInfo_t))
                {
                    (void)memcpy((void *)data, &g_system_info, sizeof(sytemInfo_t));
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case SYSDEV_SYSTEM_USAGE:
                // Check size
                if (length == sizeof(systemUsage_t))
                {
                    (void)memcpy((void *)data, &g_system_usage, sizeof(systemUsage_t));
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            default:
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
 * @fn              SystemDeviceIoctl(systemDeviceNo_t sysdev, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over a system device
 * @param[in]       sysdev      System device numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data length (if any)
 * @retval          #RET_INVALID_PARAM if the system device does not exist
 * @retval          #RET_NOT_AVAILABLE if cannot ioctl the system device (because read-only)
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t SystemDeviceIoctl(systemDeviceNo_t sysdev, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused for the moment
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Function Core
    switch (sysdev)
    {
        case SYSDEV_SYSTEM_INFO:
            // System information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_SYSTEM_USAGE:
            // System information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        default:
            return_value = RET_INVALID_PARAM;
            break;
    }

    return return_value;
}
