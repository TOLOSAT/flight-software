/**
 * @file    pus161.c
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Source file for PUS 161 functions (MISO)
 * @date    08/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "miso/pus161.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"

/***************************** Macros Definitions ****************************/

#define PUS_S161SS2_DATA_SIZE 1u
#define PUS_S161SS4_DATA_SIZE 2u

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static pus161Data_t *system_usage_pointer;

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitS161(pus161Data_t *pus161_data)
 * @brief       Function that initialises PUS 161 with shared data struct
 * @param[in]   pus161_data shared data struct
 * @retval      #PUS_SUCCESSFUL always
 */
pusStatus_t InitS161(pus161Data_t *pus161_data)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((pus161_data != NULL) && (pus161_data->number_of_tasks <= PUS161_MAX_TASK_NB))
    {
        system_usage_pointer = pus161_data;
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS2 TM (idle time report) when requested by a S161SS1
 * @param[in]   tc S161SS1 TC that requests this TM
 * @param[out]  tm S161SS2 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Build S161SS2 TM
        pusStatus_t test_build = BuildS161SS2(tm, system_usage_pointer->idle_time);
        if (test_build != PUS_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
            *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS161SS2(pusTM_t *tm)
 * @brief       Function that send S161SS2 TM (idle time report)
 * @param[out]  tm          TM to be sent
 * @param[in]   idle_time   Idle time
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS161SS2(pusTM_t *tm, uint8_t idle_time)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tm != NULL)
    {
        // Build TM
        return_value = BuildTM(tm, 161u, 2u, (pusData_t *)&idle_time, PUS_S161SS2_DATA_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS4 TM (stack usage report) when requested by a S161SS3
 * @param[in]   tc S161SS3 TC that requests this TM
 * @param[out]  tm S161SS4 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Build S161SS4 TM
        pusStatus_t test_build = BuildS161SS4(tm, system_usage_pointer->highest_stack_consumer, system_usage_pointer->max_stack_usage);
        if (test_build != PUS_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
            *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS161SS4(pusTM_t *tm)
 * @brief       Function that send S161SS4 TM (stack usage report)
 * @param[out]  tm TM to be sent
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[PUS_S161SS4_DATA_SIZE] = {0};

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
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS6 TM (system usage report) when requested by a S161SS5
 * @param[in]   tc S161SS5 TC that requests this TM
 * @param[out]  tm S161SS6 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Build S161SS4 TM
        pusStatus_t test_build = BuildS161SS6(tm, system_usage_pointer);
        if (test_build != PUS_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
            *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;

}

/**
 * @fn          BuildS161SS6(pusTM_t *tm, pus161Data_t *system_usage)
 * @brief       Function that send S161SS6 TM (system usage report)
 * @param[out]  tm TM to be sent
 * @param[in]   system_usage System usage used to compute S161SS6
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS161SS6(pusTM_t *tm, pus161Data_t *system_usage)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[TM_MAX_DATA_SIZE] = {0};

    // Function Core
    if ((tm != NULL) && (system_usage != NULL))
    {
        // Check if the size of the report can be contained in TM data
        uint32_t report_size = system_usage->number_of_tasks * sizeof(pus161TaskInfo_t);
        if (report_size <= TM_MAX_DATA_SIZE)
        {
            // Copy report in data
            for (uint32_t i = 0u; i < report_size; i++)
            {
                memcpy(&data[i*sizeof(pus161TaskInfo_t)], &system_usage->system_report[i], sizeof(pus161TaskInfo_t));
            }

            // Build TM 
            return_value = BuildTM(tm, 161u, 6u, (pusData_t *)&data, report_size);
        }
        else
        {
            return_value = PUS_INVALID_PARAM;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}
