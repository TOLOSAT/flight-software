/**
 * @file    sysdevices.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for system devices handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "system/context.h"
#include "system/sysdevices.h"
#include "system/sysinfo.h"
#include "system/sysmon.h"
#include "bsp.h"

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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused for the moment
    (void)(data);
    (void)(length);

    // Check parameter(s)
    if ((data != NULL) && (length != 0u))
    {
        switch (sysdev)
        {
            case SYSDEV_SYSTEM_INFO :
                // System information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_USAGE :
                // System information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_TASK_USAGES :
                // Task information is read only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_REBOOT :
                // System reboot is execution only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_CONTEXT :
                // Check size
                if (length == sizeof(context_t))
                {
                    // Write system context
                    context_t system_context = { 0 };
                    (void)memcpy(&system_context, (void *)data, length);
                    return_value = WriteContext(&system_context);
                }

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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((data != NULL) && (length != 0u))
    {
        switch (sysdev)
        {
            case SYSDEV_SYSTEM_INFO :
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
            case SYSDEV_SYSTEM_USAGE :
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
            case SYSDEV_TASK_USAGES :
                // Check size
                if ((length != 0u) && (length % sizeof(taskUsage_t) == 0u) && (length <= (sizeof(taskUsage_t) * (length_t)CONFIG_MAX_NB_TASKS)))
                {
                    (void)memcpy((void *)data, &g_task_usages, length);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case SYSDEV_SYSTEM_REBOOT :
                // System reboot is execution only
                return_value = RET_NOT_AVAILABLE;
                break;
            case SYSDEV_SYSTEM_CONTEXT :
                // Check size
                if ((0u < length) && (length <= sizeof(context_t)))
                {
                    // Read system context
                    context_t system_context = { 0 };
                    return_value             = ReadContext(&system_context);

                    (void)memcpy((void *)data, &system_context, length);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused for the moment
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Check parameter(s)
    switch (sysdev)
    {
        case SYSDEV_SYSTEM_INFO :
            // System information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_SYSTEM_USAGE :
            // System information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_TASK_USAGES :
            // Task information is read only
            return_value = RET_NOT_AVAILABLE;
            break;
        case SYSDEV_SYSTEM_REBOOT :
            NVIC_SystemReset();
            break;
        case SYSDEV_SYSTEM_CONTEXT :
            // Erase the context memory
            return_value = EraseContext();
            break;
        default :
            return_value = RET_INVALID_PARAM;
            break;
    }

    return return_value;
}
