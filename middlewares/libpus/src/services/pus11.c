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

/*************************** Variables Definitions ***************************/

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
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    *error_code = PUS_EXECUTION_NO_ERROR;

    // Function Core
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        /* To Do */
        (void)(tc);
        (void)(tm);
        (void)(error_code);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
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
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    *error_code = PUS_EXECUTION_NO_ERROR;

    // Function Core
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        /* To Do */
        (void)(tc);
        (void)(tm);
        (void)(error_code);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
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
    *error_code = PUS_EXECUTION_NO_ERROR;

    // Function Core
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        /* To Do */
        (void)(tc);
        (void)(tm);
        (void)(error_code);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
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
    *error_code = PUS_EXECUTION_NO_ERROR;
    pusAddActivityTCDataField_t request = {0};
    pusStatus_t test_val;

    // Function Core
    if ((tc != NULL) && (error_code != NULL))
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
                // Create Activity based on TC data
                pusActivity_t activity = {0};
                activity.timestamp = request.timestamp;
                activity.data = request.data;

                // Insert activity in schedule
                test_val = PushActivityInSchedule(&g_pus11_schedule, &activity);
                if(test_val != PUS_SUCCESSFUL)
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
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}
