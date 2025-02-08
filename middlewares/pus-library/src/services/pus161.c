/**
 * @file    pus161.c
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Source file for PUS 161 functions (MISO)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus161.h"

/***************************** Macros Definitions ****************************/

#define PUS_S161SS2_DATA_SIZE 1u /**< TM(161,2) data size */
#define PUS_S161SS4_DATA_SIZE 2u /**< TM(161,4) data size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     pus161_dev_system_usage
 * @brief   Device for reading system usage
 */
static deviceNo_t pus161_dev_system_usage = 0u;

/**
 * @var     temp_system_usage
 * @brief   Temporary system usage status
 */
static systemUsage_t temp_system_usage = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitS161(void)
 * @brief       Function that initialises PUS 161
 * @retval      #RET_ERROR if cannot bind the pus161_dev_system_usage to the system usage
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t InitS161(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    return_value = DeviceOpen(&pus161_dev_system_usage, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_USAGE);

    return return_value;
}

/**
 * @fn          ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS2 TM (idle time report) when requested by a S161SS1
 * @param[in]   tc S161SS1 TC that requests this TM
 * @param[out]  tm S161SS2 TM that we will send
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Read system usage
        return_value = DeviceRead(pus161_dev_system_usage, (data_t)&temp_system_usage, sizeof(systemUsage_t));
        if (return_value == RET_SUCCESSFUL)
        {
            // Build S161SS2 TM
            return_value = BuildS161SS2(tm, temp_system_usage.idle_time);
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
 * @fn          BuildS161SS2(pusTM_t *tm, uint8_t idle_time)
 * @brief       Function that send S161SS2 TM (idle time report)
 * @param[out]  tm          TM to be sent
 * @param[in]   idle_time   Idle time
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS161SS2(pusTM_t *tm, uint8_t idle_time)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (tm != NULL)
    {
        // Build TM
        return_value = BuildTM(tm, 161u, 2u, (pusData_t *)&idle_time, PUS_S161SS2_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS4 TM (stack usage report) when requested by a S161SS3
 * @param[in]   tc S161SS3 TC that requests this TM
 * @param[out]  tm S161SS4 TM that we will send
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Read system usage
        return_value = DeviceRead(pus161_dev_system_usage, (data_t)&temp_system_usage, sizeof(systemUsage_t));
        if (return_value == RET_SUCCESSFUL)
        {
            // Build S161SS4 TM
            return_value = BuildS161SS4(tm, temp_system_usage.highest_stack_consumer, temp_system_usage.max_stack_usage);
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
 * @fn          BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
 * @brief       Function that send S161SS4 TM (stack usage report)
 * @param[out]  tm TM to be sent
 * @param[in]   highest_stack_consumer Task that is the highest stack consummer (in percent of its own stack)
 * @param[in]   max_stack_usage Stack usage for that stack
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
{
    // Variable Initialisation
    returnCode_t return_value             = RET_SUCCESSFUL;
    pusData_t data[PUS_S161SS4_DATA_SIZE] = { 0 };

    // Function Core
    if (tm != NULL)
    {
        // Get highest stack consummer
        data[0] = highest_stack_consumer;

        // Get stack usage
        data[1] = max_stack_usage;

        // Build TM
        return_value = BuildTM(tm, 161u, 4u, (pusData_t *)&data, PUS_S161SS4_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS6 TM (system usage report) when requested by a S161SS5
 * @param[in]   tc S161SS5 TC that requests this TM
 * @param[out]  tm S161SS6 TM that we will send
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Read system usage
        return_value = DeviceRead(pus161_dev_system_usage, (data_t)&temp_system_usage, sizeof(systemUsage_t));
        if (return_value == RET_SUCCESSFUL)
        {
            // Build S161SS4 TM
            return_value = BuildS161SS6(tm, temp_system_usage.task_usage);
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
 * @fn          BuildS161SS6(pusTM_t *tm, taskUsage_t *tasks_info)
 * @brief       Function that send S161SS6 TM (system usage report)
 * @param[out]  tm TM to be sent
 * @param[in]   tasks_info Pointer towards tasks monitoring information
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS161SS6(pusTM_t *tm, taskUsage_t *tasks_info)
{
    // Variable Initialisation
    returnCode_t return_value        = RET_SUCCESSFUL;
    pusData_t data[TM_MAX_DATA_SIZE] = { 0 };

    // Function Core
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
            return_value = BuildTM(tm, 161u, 6u, (pusData_t *)&data, report_size);
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
