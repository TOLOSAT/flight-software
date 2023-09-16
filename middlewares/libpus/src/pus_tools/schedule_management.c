/**
 * @file    schedule_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for schedules
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "pus_tools/schedule_management.h"
#include "pus_tools/time_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t GetAvailableNode(pusSchedule_t *schedule, pusNodeIndex_t *available_node);
static pusStatus_t InsertNodeInSchedule(pusSchedule_t *schedule, pusActivity_t *activity, pusNodeIndex_t new_node_index);
static pusStatus_t ReleaseOldestActivity(pusSchedule_t *schedule, pusActivity_t *activity);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              PushActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
 * @brief           Push an activity into the schedule
 * @param[in,out]   schedule Schedule that will receive the activity
 * @param[in]       activity Activity to push
 * @retval          #PUS_INVALID_PARAM if a pointer is null
 * @retval          #PUS_ERROR if there is no more place available in the schedule
 * @retval          #PUS_ERROR if an error has been encountered
 * @retval          #PUS_SUCCESSFUL else
 */
pusStatus_t PushActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusStatus_t test_val;

    // Function Core
    if ((schedule != NULL) && (activity != NULL))
    {
        // Check if there is still room in schedule
        if (schedule->info.nb_activities < MAXIMUM_ACTIVITIES_PER_SCHEDULE)
        {
            // Get a node
            pusNodeIndex_t new_node_index = 0u;
            test_val = GetAvailableNode(schedule, &new_node_index);
            if (test_val == PUS_SUCCESSFUL)
            {
                // Insert New node in schedule
                test_val = InsertNodeInSchedule(schedule, activity, new_node_index);
                if (test_val != PUS_SUCCESSFUL)
                {
                    return_value = PUS_ERROR;
                }
            }
            else
            {
                return_value = PUS_ERROR;
            }
        }
        else
        {
            return_value = PUS_ERROR;
        }
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
 * @retval          #PUS_NOT_AVAILABLE if there is no more activity in the schedule
 * @retval          #PUS_NOT_AVAILABLE if there is no activity that can be released
 * @retval          #PUS_ERROR if an error has been encountered
 * @retval          #PUS_SUCCESSFUL else
 */
pusStatus_t PopActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusStatus_t test_val;

    // Function Core
    if ((schedule != NULL) && (activity != NULL))
    {
        // Check if there is an activity in schedule
        if (schedule->info.nb_activities != 0u)
        {
            // Get current time
            cucTime_t current_time = {0};
            test_val = GetCUCTime(&current_time);
            if (test_val == PUS_SUCCESSFUL)
            {
                // Now check if oldest node can be released or not
                cucTime_t oldest_node_time = schedule->activity_nodes[schedule->info.oldest_activity_index].activity.timestamp;
                test_val = CompareCUCTimes(&oldest_node_time, &current_time);
                if (test_val == PUS_SUCCESSFUL)
                {
                    // It means that oldest_node_time <= current_time so we can release activity
                    test_val = ReleaseOldestActivity(schedule, activity);
                    if (test_val != PUS_SUCCESSFUL)
                    {
                        return_value = PUS_ERROR;
                    }
                }
                else
                {
                    // It means that oldest_node_time > current_time so we cannot release activity
                    return_value = PUS_NOT_AVAILABLE;
                }
            }
            else
            {
                return_value = PUS_ERROR;
            }
        }
        else
        {
            // No activities available in schedule
            return_value = PUS_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GetAvailableNode(pusSchedule_t *schedule, pusNodeIndex_t *available_node)
 * @brief           This function gets the closest available node from the writing pointer
 * @param[in,out]   schedule Schedule from which a new node is taken
 * @param[out]      available_node New node index
 * @retval          #PUS_INVALID_PARAM if a pointer is NULL
 * @retval          #PUS_ERROR if no node is available
 * @retval          #PUS_SUCCESSFUL else
 */
static pusStatus_t GetAvailableNode(pusSchedule_t *schedule, pusNodeIndex_t *available_node)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    if ((schedule != NULL) && (available_node != NULL))
    {
        pusNodeIndex_t current_write_index = schedule->info.write_index;
        while ((schedule->activity_nodes[current_write_index].status == (pusNodeIndex_t)ACTIVITY_NODE_UNAVAILABLE) && (current_write_index != schedule->info.write_index))
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
        if ((current_write_index == schedule->info.write_index) && (schedule->activity_nodes[current_write_index].status == (pusNodeIndex_t)ACTIVITY_NODE_UNAVAILABLE))
        {
            return_value = PUS_ERROR;
        }
        else
        {
            // Update available node and write index
            *available_node = current_write_index;
            schedule->info.write_index = current_write_index + 1u;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              pusStatus_t InsertNodeInSchedule(pusSchedule_t *schedule, pusActivity_t *activity, pusNodeIndex_t *new_node_index)
 * @brief           This function releases the oldest activity and update the schedule
 * @param[in,out]   schedule Schedule from which the oldest activity is release
 * @param[in]       activity Oldest activity released content
 * @param[in]       new_node_index New node index
 * @retval          #PUS_INVALID_PARAM if a pointer is NULL
 * @retval          #PUS_SUCCESSFUL else
 */
static pusStatus_t InsertNodeInSchedule(pusSchedule_t *schedule, pusActivity_t *activity, pusNodeIndex_t new_node_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((schedule != NULL) && (activity != NULL))
    {
        // Check if there is at least one node in schedule
        if (schedule->info.nb_activities == 0u)
        {
            // If there is no node in shedule we just add new node
            // Update node
            schedule->activity_nodes[new_node_index].status = ACTIVITY_NODE_UNAVAILABLE;
            schedule->activity_nodes[new_node_index].activity.timestamp = activity->timestamp;
            schedule->activity_nodes[new_node_index].activity.data = activity->data;
            schedule->activity_nodes[new_node_index].previous_node_index = UNEXISTING_NODE_INDEX;
            schedule->activity_nodes[new_node_index].next_node_index = UNEXISTING_NODE_INDEX;

            // Then update info
            schedule->info.nb_activities++;
            schedule->info.oldest_activity_index = new_node_index;
        }
        else
        {
            // If there is at least one node we are looking for the node that will be just
            // after new node. We start with the oldest node.
            pusNodeIndex_t next_node = schedule->info.oldest_activity_index;
            uint32_t counter = 0u;

            // Start looking for the next node
            pusStatus_t is_newer = CompareCUCTimes(&schedule->activity_nodes[next_node].activity.timestamp, &activity->timestamp);
            while ((is_newer == PUS_SUCCESSFUL) && (schedule->activity_nodes[next_node].next_node_index != UNEXISTING_NODE_INDEX) && (counter < MAXIMUM_ACTIVITIES_PER_SCHEDULE))
            {
                next_node = schedule->activity_nodes[next_node].next_node_index;
                is_newer = CompareCUCTimes(&schedule->activity_nodes[next_node].activity.timestamp, &activity->timestamp);
                counter++;
            }

            // While loop stops now we are going to look result
            if (counter < MAXIMUM_ACTIVITIES_PER_SCHEDULE)
            {
                if (schedule->activity_nodes[next_node].next_node_index != UNEXISTING_NODE_INDEX)
                {
                    // We just found the next node
                    pusNodeIndex_t previous_node = schedule->activity_nodes[next_node].previous_node_index;

                    // Update new node
                    schedule->activity_nodes[new_node_index].status = ACTIVITY_NODE_UNAVAILABLE;
                    schedule->activity_nodes[new_node_index].activity.timestamp = activity->timestamp;
                    schedule->activity_nodes[new_node_index].activity.data = activity->data;
                    schedule->activity_nodes[new_node_index].previous_node_index = previous_node;
                    schedule->activity_nodes[new_node_index].next_node_index = next_node;

                    // Update next node
                    schedule->activity_nodes[next_node].previous_node_index = new_node_index;

                    // Check if new node is not the oldest node
                    if (previous_node != UNEXISTING_NODE_INDEX)
                    {
                        // If there is a previous node we update it
                        schedule->activity_nodes[previous_node].next_node_index = new_node_index;
                    }
                    else
                    {
                        // Else new node is the oldest node
                        schedule->info.oldest_activity_index = new_node_index;
                    }

                    // Then update info
                    schedule->info.nb_activities++;
                }
                else
                {
                    // Then we reached the end of the linked list. Now check if node is before or after last node.
                    if (is_newer == PUS_SUCCESSFUL)
                    {
                        // It means that in fact next_node is in reality previous node
                        pusNodeIndex_t previous_node = next_node;
                        next_node = UNEXISTING_NODE_INDEX;

                        // Update new node
                        schedule->activity_nodes[new_node_index].status = ACTIVITY_NODE_UNAVAILABLE;
                        schedule->activity_nodes[new_node_index].activity.timestamp = activity->timestamp;
                        schedule->activity_nodes[new_node_index].activity.data = activity->data;
                        schedule->activity_nodes[new_node_index].previous_node_index = previous_node;
                        schedule->activity_nodes[new_node_index].next_node_index = next_node;

                        // Update previous node
                        schedule->activity_nodes[previous_node].next_node_index = new_node_index;

                        // Then update info
                        schedule->info.nb_activities++;
                    }
                    else
                    {
                        // It means that in fact next_node is still the last node
                        pusNodeIndex_t previous_node = schedule->activity_nodes[next_node].previous_node_index;

                        // Update new node
                        schedule->activity_nodes[new_node_index].status = ACTIVITY_NODE_UNAVAILABLE;
                        schedule->activity_nodes[new_node_index].activity.timestamp = activity->timestamp;
                        schedule->activity_nodes[new_node_index].activity.data = activity->data;
                        schedule->activity_nodes[new_node_index].previous_node_index = previous_node;
                        schedule->activity_nodes[new_node_index].next_node_index = next_node;

                        // Update next node
                        schedule->activity_nodes[next_node].previous_node_index = new_node_index;

                        // Check if new node is not the oldest node
                        if (previous_node != UNEXISTING_NODE_INDEX)
                        {
                            // If there is a previous node we update it
                            schedule->activity_nodes[previous_node].next_node_index = new_node_index;
                        }
                        else
                        {
                            // Else new node is the oldest node
                            schedule->info.oldest_activity_index = new_node_index;
                        }

                        // Then update info
                        schedule->info.nb_activities++;
                    }
                }
            }
            else
            {
                // We went around the schedule without finding any node
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              ReleaseOldestActivity(pusSchedule_t *schedule, pusActivity_t *activity)
 * @brief           This function releases the oldest activity and update the schedule
 * @param[in,out]   schedule Schedule from which the oldest activity is release
 * @param[out]      activity Oldest activity released content
 * @retval          #PUS_INVALID_PARAM if a pointer is NULL
 * @retval          #PUS_SUCCESSFUL else
 */
static pusStatus_t ReleaseOldestActivity(pusSchedule_t *schedule, pusActivity_t *activity)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((schedule != NULL) && (activity != NULL))
    {
        // Variable Initialization
        pusNodeIndex_t former_oldest_node_index = schedule->info.oldest_activity_index;
        pusNodeIndex_t new_oldest_node_index = schedule->activity_nodes[former_oldest_node_index].next_node_index;

        // First, we save copy of the node content to exiting activity variable
        activity->timestamp = schedule->activity_nodes[former_oldest_node_index].activity.timestamp;
        activity->data = schedule->activity_nodes[former_oldest_node_index].activity.data;

        // Then, we free the former oldest node
        (void)memset(&schedule->activity_nodes[former_oldest_node_index], 0u, ACTIVITY_NODE_SIZE);

        // Then, we update the new oldest node if it exists
        if (new_oldest_node_index != UNEXISTING_NODE_INDEX)
        {
            schedule->activity_nodes[new_oldest_node_index].previous_node_index = UNEXISTING_NODE_INDEX;
        }

        // Finally, we update schedule info
        schedule->info.oldest_activity_index = new_oldest_node_index;
        schedule->info.nb_activities--;
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}