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

coreStatus_t IN_CORE_TEXT_SECTION DeviceClose(deviceNo_t device)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(device);

    return return_value;
}