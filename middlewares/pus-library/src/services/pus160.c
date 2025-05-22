/**
 * @file    pus160.c
 * @author  Théo Bessel
 * @brief   Source file for PUS 160 functions (System management)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus160.h"
#include "system/context.h"

/***************************** Macros Definitions ****************************/

#define PUS_S160SS34_DATA_SIZE 1u /**< TM(160,34) data size */
#define PUS_S160SS36_DATA_SIZE 2u /**< TM(160,36) data size */

/*************************** Functions Declarations **************************/

static returnCode_t BuildS160SS18(pusTM_t *tm);
static returnCode_t BuildS160SS20(pusTM_t *tm);
static returnCode_t BuildS160SS22(pusTM_t *tm);
static returnCode_t BuildS160SS34(pusTM_t *tm, uint8_t idle_time);
static returnCode_t BuildS160SS36(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage);
static returnCode_t BuildS160SS38(pusTM_t *tm, taskUsage_t *tasks_info);

/*************************** Variables Definitions ***************************/

/**
 * @var     pus160_dev_reboot
 * @brief   Device for rebooting the system
 */
static deviceNo_t pus160_dev_reboot = 0u;

/**
 * @var pus160_dev_context
 * @brief Device for reading system context
 */
static deviceNo_t pus160_dev_context = 0u;

/**
 * @var     pus160_dev_system_usage
 * @brief   Device for reading system usage
 */
static deviceNo_t pus160_dev_system_usage = 0u;

/**
 * @var     temp_system_usage
 * @brief   Temporary system usage status
 */
static systemUsage_t temp_system_usage = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitS160(void)
 * @brief       Function that initialises PUS 160
 * @retval      #RET_ERROR if cannot bind the pus160_dev_reboot to the reboot
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t InitS160(void)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Start S160 by opening a device for rebooting the system
    return_value = DeviceOpen(&pus160_dev_reboot, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_REBOOT);

    if (return_value == RET_SUCCESSFUL)
    {
        return_value = DeviceOpen(&pus160_dev_context, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_CONTEXT);
    }

    if (return_value == RET_SUCCESSFUL)
    {
        // Start S160 by opening a device for system usage virtual device
        return_value = DeviceOpen(&pus160_dev_system_usage, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_USAGE);
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that achieve a reboot to a chosen software
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for the sent TM
 */
returnCode_t ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    uint8_t software_id       = 0u;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    LOG("[TM/TC] Rebooting to a nominal sw ...\n");

    // Read software ID from TC
    if (tc->spp_header.packet_data_length == (sizeof(software_id) + CRC_TRAILER_SIZE + TC_HEADER_SIZE - 1))
    {
        (void)memcpy((uint8_t *)&software_id, tc->data, sizeof(software_id));
        LOG_DECIMAL("Software ID: %d\n", software_id);
    }
    else
    {
        *error_code  = PUS_EXECUTION_FAILED;
        return_value = RET_INVALID_PARAM;
    }

    if (return_value == RET_SUCCESSFUL)
    {
        return_value = DeviceIoctl(pus160_dev_reboot, 1u, &software_id, sizeof(software_id));
    }

    (void)DeviceClose(pus160_dev_reboot);

    return return_value;
}

/**
 * @fn          ExecuteS160SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that achieve a reboot to safe mode
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for the sent TM
 */
returnCode_t ExecuteS160SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    LOG("[TM/TC] Rebooting ...\n");

    return_value = DeviceIoctl(pus160_dev_reboot, 0u, NULL, 0u);

    (void)DeviceClose(pus160_dev_reboot);

    return return_value;
}

/**
 * @fn          ExecuteS160SS17(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that requests the system context
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S160SS18 TM
 */
returnCode_t ExecuteS160SS17(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    (void)(tc);

    returnCode_t return_value = RET_SUCCESSFUL;
    *error_code               = PUS_EXECUTION_NO_ERROR;

    return_value = BuildS160SS18(tm);

    if (return_value != RET_SUCCESSFUL)
    {
        *error_code = PUS_EXECUTION_FAILED;
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS19(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that requests the reduced system context (without debug info)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S160SS20 TM
 */
returnCode_t ExecuteS160SS19(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    (void)(tc);

    returnCode_t return_value = RET_SUCCESSFUL;
    *error_code               = PUS_EXECUTION_NO_ERROR;

    return_value = BuildS160SS20(tm);

    if (return_value != RET_SUCCESSFUL)
    {
        *error_code = PUS_EXECUTION_FAILED;
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS21(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that requests the error context (only debug info)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S160SS22 TM
 */
returnCode_t ExecuteS160SS21(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    (void)(tc);

    returnCode_t return_value = RET_SUCCESSFUL;
    *error_code               = PUS_EXECUTION_NO_ERROR;

    return_value = BuildS160SS22(tm);

    if (return_value != RET_SUCCESSFUL)
    {
        *error_code = PUS_EXECUTION_FAILED;
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS33(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S160SS34 TM (idle time report) when requested by a S160SS33
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S160SS34 TM
 */
returnCode_t ExecuteS160SS33(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);

    // Check parameter(s)
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Read system usage
        return_value = DeviceRead(pus160_dev_system_usage, (data_t)&temp_system_usage, sizeof(systemUsage_t));
        if (return_value == RET_SUCCESSFUL)
        {
            // Build S161SS2 TM
            return_value = BuildS160SS34(tm, temp_system_usage.idle_time);
            if (return_value != RET_SUCCESSFUL)
            {
                *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
            }
        }
        else
        {
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS35(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S160SS36 TM (stack usage report) when requested by a S160SS35
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S160SS36 TM
 */
returnCode_t ExecuteS160SS35(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);

    // Check parameter(s)
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Read system usage
        return_value = DeviceRead(pus160_dev_system_usage, (data_t)&temp_system_usage, sizeof(systemUsage_t));
        if (return_value == RET_SUCCESSFUL)
        {
            // Build S161SS4 TM
            return_value = BuildS160SS36(tm, temp_system_usage.highest_stack_consumer, temp_system_usage.max_stack_usage);
            if (return_value != RET_SUCCESSFUL)
            {
                *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
            }
        }
        else
        {
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS37(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S160SS38 TM (system usage report) when requested by a S160SS37
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S160SS38 TM
 */
returnCode_t ExecuteS160SS37(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);

    // Check parameter(s)
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Read system usage
        return_value = DeviceRead(pus160_dev_system_usage, (data_t)&temp_system_usage, sizeof(systemUsage_t));
        if (return_value == RET_SUCCESSFUL)
        {
            // Build S161SS4 TM
            return_value = BuildS160SS38(tm, temp_system_usage.task_usage);
            if (return_value != RET_SUCCESSFUL)
            {
                *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
            }
        }
        else
        {
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS160SS18(pusTM_t *tm)
 * @brief       Function that sends the memory context of the system
 * @param[out]  tm TM that will be sent
 */
static returnCode_t BuildS160SS18(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    context_t context         = { 0 };

    return_value = DeviceRead(pus160_dev_context, (data_t)&context, sizeof(context_t));

    if ((tm != NULL) && (return_value == RET_SUCCESSFUL))
    {
        return_value = BuildTM(tm, 160u, 18u, (pusData_t *)&context, sizeof(context_t));
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    (void)DeviceClose(pus160_dev_context);

    return return_value;
}

/**
 * @fn          BuildS160SS20(pusTM_t *tm)
 * @brief       Function that sends the reduced memory context of the system
 * @param[out]  tm TM that will be sent
 */
static returnCode_t BuildS160SS20(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    context_t context         = { 0 };

    length_t reduced_context_length = sizeof(context.version) + sizeof(context.state) + sizeof(context.boot) + sizeof(context.critical_error);

    return_value = DeviceRead(pus160_dev_context, (data_t)&context, reduced_context_length);

    if ((tm != NULL) && (return_value == RET_SUCCESSFUL))
    {
        return_value = BuildTM(tm, 160u, 18u, (pusData_t *)&context, reduced_context_length);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    (void)DeviceClose(pus160_dev_context);

    return return_value;
}

/**
 * @fn          BuildS160SS22(pusTM_t *tm)
 * @brief       Function that sends the error context of the system
 * @param[out]  tm TM that will be sent
 */
static returnCode_t BuildS160SS22(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    context_t context         = { 0 };

    length_t error_context_length = sizeof(context.cfsr) + sizeof(context.hfsr) + sizeof(context.registers) + sizeof(context.call_stack);

    return_value = DeviceRead(pus160_dev_context, (data_t)&context, error_context_length);

    if ((tm != NULL) && (return_value == RET_SUCCESSFUL))
    {
        return_value = BuildTM(tm, 160u, 18u, (pusData_t *)&context, error_context_length);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    (void)DeviceClose(pus160_dev_context);

    return return_value;
}

/**
 * @fn          BuildS160SS34(pusTM_t *tm, uint8_t idle_time)
 * @brief       Function that send S161SS2 TM (idle time report)
 * @param[out]  tm TM that will be sent
 * @param[in]   idle_time   Idle time
 */
static returnCode_t BuildS160SS34(pusTM_t *tm, uint8_t idle_time)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (tm != NULL)
    {
        // Build TM
        return_value = BuildTM(tm, 160u, 34u, (pusData_t *)&idle_time, PUS_S160SS34_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS160SS36(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
 * @brief       Function that send S161SS4 TM (stack usage report)
 * @param[out]  tm TM that will be sent
 * @param[in]   highest_stack_consumer Task that is the highest stack consummer (in percent of its own stack)
 * @param[in]   max_stack_usage Stack usage for that stack
 */
static returnCode_t BuildS160SS36(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
{
    returnCode_t return_value              = RET_SUCCESSFUL;
    pusData_t data[PUS_S160SS36_DATA_SIZE] = { 0 };

    // Check parameter(s)
    if (tm != NULL)
    {
        // Get highest stack consummer
        data[0] = highest_stack_consumer;

        // Get stack usage
        data[1] = max_stack_usage;

        // Build TM
        return_value = BuildTM(tm, 160u, 36u, (pusData_t *)&data, PUS_S160SS36_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS160SS38(pusTM_t *tm, taskUsage_t *tasks_info)
 * @brief       Function that sends S160SS38 TM (system usage report)
 * @param[out]  tm TM that will be sent
 * @param[in]   tasks_info Pointer towards tasks monitoring information
 */
static returnCode_t BuildS160SS38(pusTM_t *tm, taskUsage_t *tasks_info)
{
    returnCode_t return_value        = RET_SUCCESSFUL;
    pusData_t data[TM_MAX_DATA_SIZE] = { 0 };

    // Check parameter(s)
    if ((tm != NULL) && (tasks_info != NULL))
    {
        // Check if the size of the report can be contained in TM data
        uint32_t report_size = NB_TASKS * sizeof(taskUsage_t);
        if (report_size <= TM_MAX_DATA_SIZE)
        {
            // Copy report in data
            for (uint32_t i = 0u; i < report_size; i++)
            {
                (void)memcpy((void *)&data[i * sizeof(taskUsage_t)], (void *)&tasks_info[i], sizeof(taskUsage_t));
            }

            // Build TM
            return_value = BuildTM(tm, 160u, 38u, (pusData_t *)&data, report_size);
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}