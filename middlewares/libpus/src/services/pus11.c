/**
 * @file    pus11.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 11 functions (Time-based scheduling)
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "services/pus11.h"
#include "conf/pus11_conf.h"
#include "pus_tools/schedule_management.h"
#include "pus_tools/time_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t GetAvailableData(pus11DataTable_t *data_table, pus11DataIndex_t *data_index);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus11_status
 * @brief   Indicates if pus11 is enable or disable
 */
pus11Status_t g_pus11_status = PUS11_ENABLE;

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will enable time based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (error_code != NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Enable PUS11
        g_pus11_status = PUS11_ENABLE;
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will disable time based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (error_code != NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Enable PUS11
        g_pus11_status = PUS11_DISABLE;
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will reset time-based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        /* To Do */
        (void)(tc);
        (void)(tm);
        (void)(error_code);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will add activity to a time based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusAddActivityTCDataField_t request = {0};
    pusStatus_t test_val;

    // Function Core
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Check if PUS11 is enable
        if (g_pus11_status == PUS11_ENABLE)
        {
            // Get data from TC
            (void)memcpy((void *)&request, (void *)tc->data, 10u);

            // Get Current time
            cucTime_t current_time = {0};
            test_val = GetCUCTime(&current_time);
            if (test_val == PUS_SUCCESSFUL)
            {
                // Check if requested timestamp is in the futur
                test_val = CompareCUCTimes(&current_time, &request.timestamp);
                if (test_val == PUS_SUCCESSFUL)
                {
                    // Check if there is still data available
                    if (g_pus11_data_table.info.nb_data < PUS11_MAXIMUM_DATA)
                    {
                        // Get a data slot
                        pus11DataIndex_t new_data_index = 0u;
                        test_val = GetAvailableData(&g_pus11_data_table, &new_data_index);
                        if (test_val == PUS_SUCCESSFUL)
                        {
                            // Put data in data table
                            /* To Do */
                            
                            // Create Activity based on TC data
                            pusActivity_t activity = {0};
                            activity.timestamp = request.timestamp;
                            activity.data = new_data_index;

                            // Insert activity in schedule
                            test_val = PushActivityInSchedule(&g_pus11_schedule, &activity);
                            if (test_val != PUS_SUCCESSFUL)
                            {
                                return_value = PUS_ERROR;
                                *error_code = PUS_EXECUTION_UNAVAILABLE;
                            }
                        }
                        else
                        {
                            return_value = PUS_ERROR;
                            *error_code = PUS_EXECUTION_UNAVAILABLE;
                        }
                    }
                    else
                    {
                        return_value = PUS_ERROR;
                        *error_code = PUS_EXECUTION_UNAVAILABLE;
                    }
                }
                else
                {
                    return_value = PUS_ERROR;
                    *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
                }
            }
            else
            {
                return_value = PUS_ERROR;
                *error_code = PUS_EXECUTION_FAILED;
            }
        }
        else
        {
            return_value = PUS_ERROR;
            *error_code = PUS_EXECUTION_UNAVAILABLE;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GetAvailableData(pus11DataTable_t *data_table, pus11DataIndex_t *data_index)
 * @brief           This function gets the closest available data from the writing pointer
 * @param[in,out]   data_table Data from which a data will be writen
 * @param[out]      data_index New data index
 * @retval          #PUS_INVALID_PARAM if a pointer is NULL
 * @retval          #PUS_ERROR if no data is available
 * @retval          #PUS_SUCCESSFUL else
 */
static pusStatus_t GetAvailableData(pus11DataTable_t *data_table, pus11DataIndex_t *data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    if ((data_table != NULL) && (data_index != NULL))
    {
        pus11DataIndex_t current_write_index = data_table->info.write_index;
        while ((data_table->data[current_write_index].status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE) && (current_write_index != data_table->info.write_index))
        {
            if (current_write_index == MAXIMUM_ACTIVITIES_PER_SCHEDULE)
            {
                current_write_index = 0u;
            }
            else
            {
                current_write_index++;
            }
        }

        // Make sure you haven't gone full circle
        if ((current_write_index == data_table->info.write_index) && (data_table->data[current_write_index].status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE))
        {
            return_value = PUS_ERROR;
        }
        else
        {
            // Update available data and write index
            *data_index = current_write_index;
            data_table->info.write_index = current_write_index + 1u;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}