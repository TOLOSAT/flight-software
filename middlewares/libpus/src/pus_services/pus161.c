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

#include "pus_common.h"
#include "pus_services/pus161.h"

/***************************** Macros Definitions ****************************/

#define PUS_S161SS2_DATA_SIZE 1u /**< TM(161,2) data size */
#define PUS_S161SS4_DATA_SIZE 2u /**< TM(161,4) data size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     pus161_data
 * @brief   Pointer to the PUS 161 system usage data struct
 */
static pus161Data_t IN_PUS_DATA_SECTION pus161_data = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitS161(uint8_t number_of_task, pus161Data_t **p_pus161_data)
 * @brief       Function that initialises PUS 161 with shared data struct
 * @param[in]   number_of_task number of tasks in the system
 * @param[out]  p_pus161_data pointer to a pointer that will linked with pus161 data
 * @retval      #PUS_SUCCESSFUL always
 */
pusStatus_t IN_PUS_TEXT_SECTION InitS161(uint8_t number_of_task, pus161Data_t **p_pus161_data)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((number_of_task != 0u) && (number_of_task <= PUS161_MAX_TASK_NB))
    {
        // Initialise task ref fields
        for (uint32_t i = 0u; i < number_of_task; i++)
        {
            pus161_data.system_report[i].task_ref = i;
        }
        pus161_data.number_of_tasks = number_of_task;
        
        // Update the pointer
        *p_pus161_data = &pus161_data;
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
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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
        pusStatus_t test_build = BuildS161SS2(tm, pus161_data.idle_time);
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
 * @fn          BuildS161SS2(pusTM_t *tm, uint8_t idle_time)
 * @brief       Function that send S161SS2 TM (idle time report)
 * @param[out]  tm          TM to be sent
 * @param[in]   idle_time   Idle time
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION BuildS161SS2(pusTM_t *tm, uint8_t idle_time)
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
 * @fn          ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS4 TM (stack usage report) when requested by a S161SS3
 * @param[in]   tc S161SS3 TC that requests this TM
 * @param[out]  tm S161SS4 TM that we will send
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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
        pusStatus_t test_build = BuildS161SS4(tm, pus161_data.highest_stack_consumer, pus161_data.max_stack_usage);
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
 * @fn          BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
 * @brief       Function that send S161SS4 TM (stack usage report)
 * @param[out]  tm TM to be sent
 * @param[in]   highest_stack_consumer Task that is the highest stack consummer (in percent of its own stack)
 * @param[in]   max_stack_usage Stack usage for that stack
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION BuildS161SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
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
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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
        pusStatus_t test_build = BuildS161SS6(tm, &pus161_data);
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
 * @fn          BuildS161SS6(pusTM_t *tm, pus161Data_t *pus161_data)
 * @brief       Function that send S161SS6 TM (system usage report)
 * @param[out]  tm TM to be sent
 * @param[in]   pus161_data System usage used to compute S161SS6
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION BuildS161SS6(pusTM_t *tm, pus161Data_t *pus161_data)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[TM_MAX_DATA_SIZE] = {0};

    // Function Core
    if ((tm != NULL) && (pus161_data != NULL))
    {
        // Check if the size of the report can be contained in TM data
        uint32_t report_size = pus161_data->number_of_tasks * sizeof(pus161TaskInfo_t);
        if (report_size <= TM_MAX_DATA_SIZE)
        {
            // Copy report in data
            for (uint32_t i = 0u; i < report_size; i++)
            {
                (void)memcpy((void *)&data[i*sizeof(pus161TaskInfo_t)], (void *)&pus161_data->system_report[i], sizeof(pus161TaskInfo_t));
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
