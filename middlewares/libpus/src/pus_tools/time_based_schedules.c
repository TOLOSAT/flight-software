/**
 * @file    time_based_schedules.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for time based schedules
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "pus_tools/time_based_schedules.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              PushActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
 * @brief           Push an activity into the schedule
 * @param[in,out]   schedule Schedule that will receive the activity
 * @param[in]       activity Activity to push
 * @retval          #PUS_INVALID_PARAM if a pointer is null
 * @retval          #PUS_ERROR if there is no more place available in the schedule
 * @retval          #PUS_SUCCESSFUL else
 */
pusStatus_t PushActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((schedule != NULL) && (activity != NULL))
    {
        (void)(schedule);
        (void)(activity);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              PopActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
 * @brief           Pop an activity from the schedule
 * @param[in,out]   schedule Schedule from where the activity will be removed
 * @param[out]      activity Activity removed
 * @retval          #PUS_INVALID_PARAM if a pointer is null
 * @retval          #PUS_ERROR if there is no more activity in the schedule
 * @retval          #PUS_SUCCESSFUL else
 */
pusStatus_t PopActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((schedule != NULL) && (activity != NULL))
    {
        (void)(schedule);
        (void)(activity);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}