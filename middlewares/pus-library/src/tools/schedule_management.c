/**
 * @file    schedule_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for schedules
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tools/schedule_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t GetAvailableNode(deviceNo_t schedule_deviceno, pusNodeIndex_t *available_node);
static returnCode_t InsertNodeInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity, pusNodeIndex_t new_node_index);
static returnCode_t ReleaseOldestActivity(deviceNo_t schedule_deviceno, pusActivity_t *activity);
static returnCode_t GetInfoFromSchedule(deviceNo_t schedule_deviceno, pusScheduleInfo_t *schedule_info);
static returnCode_t SetInfoFromSchedule(deviceNo_t schedule_deviceno, pusScheduleInfo_t *schedule_info);
static returnCode_t GetNodeFromSchedule(deviceNo_t schedule_deviceno, pusActivityNode_t *activity_node, pusNodeIndex_t node_index);
static returnCode_t SetNodeFromSchedule(deviceNo_t schedule_deviceno, pusActivityNode_t *activity_node, pusNodeIndex_t node_index);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          PushActivityInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity)
 * @brief       Push an activity into the schedule
 * @param[in]   schedule_deviceno Schedule file number that will receive the activity
 * @param[in]   activity Activity to push
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_NOT_AVAILABLE if there is no more place available in the schedule
 * @retval      #RET_ERROR if an error has been encountered
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t PushActivityInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (activity != NULL)
    {
        // Check if there is still room in schedule
        pusScheduleInfo_t schedule_info = {0};
        return_value = GetInfoFromSchedule(schedule_deviceno, &schedule_info);
        if (return_value == RET_SUCCESSFUL)
        {
            if (schedule_info.nb_activities < MAXIMUM_ACTIVITIES_PER_SCHEDULE)
            {
                // Get a node
                pusNodeIndex_t new_node_index = 0u;
                return_value = GetAvailableNode(schedule_deviceno, &new_node_index);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Insert New node in schedule
                    return_value = InsertNodeInSchedule(schedule_deviceno, activity, new_node_index);
                }
            }
            else
            {
                return_value = RET_NOT_AVAILABLE;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          PopActivityInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity)
 * @brief       Pop an activity from the schedule
 * @param[in]   schedule_deviceno Schedule file number from where the activity will be removed
 * @param[out]  activity Activity removed
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_NOT_AVAILABLE if there is no more activity in the schedule
 * @retval      #RET_NOT_AVAILABLE if there is no activity that can be released
 * @retval      #RET_ERROR if an error has been encountered
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t PopActivityInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (activity != NULL)
    {
        // Get Schedule info
        returnCode_t test_val = RET_SUCCESSFUL;
        pusScheduleInfo_t schedule_info = {0};
        test_val = GetInfoFromSchedule(schedule_deviceno, &schedule_info);
        if (test_val == RET_SUCCESSFUL)
        {
            // Check if there is an activity in schedule
            if (schedule_info.nb_activities != 0u)
            {
                // Get current time
                time_t current_time = 0;
                returnCode_t test_time = GetTime(&current_time);
                if (test_time == RET_SUCCESSFUL)
                {
                    // Get oldest node
                    pusActivityNode_t oldest_node = {0};
                    test_val = GetNodeFromSchedule(schedule_deviceno, &oldest_node, schedule_info.oldest_activity_index);
                    if (test_val == RET_SUCCESSFUL)
                    {
                        // Now check if oldest node can be released or not
                        if (oldest_node.activity.timestamp <= current_time)
                        {
                            // It means that oldest_node_time <= current_time so we can release activity
                            test_val = ReleaseOldestActivity(schedule_deviceno, activity);
                            if (test_val != RET_SUCCESSFUL)
                            {
                                return_value = RET_ERROR;
                            }
                        }
                        else
                        {
                            // It means that oldest_node_time > current_time so we cannot release activity
                            return_value = RET_NOT_AVAILABLE;
                        }
                    }
                    else
                    {
                        return_value = RET_ERROR;
                    }
                }
                else
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                // No activities available in schedule
                return_value = RET_NOT_AVAILABLE;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetAvailableNode(deviceNo_t schedule_deviceno, pusNodeIndex_t *available_node)
 * @brief       This function gets the closest available node from the writing pointer
 * @param[in]   schedule_deviceno Schedule file number from which a new node is taken
 * @param[out]  available_node New node index
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if no node is available
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t GetAvailableNode(deviceNo_t schedule_deviceno, pusNodeIndex_t *available_node)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (available_node != NULL)
    {
        // First get table info
        pusScheduleInfo_t schedule_info = {0};
        returnCode_t test_val = RET_SUCCESSFUL;
        test_val = GetInfoFromSchedule(schedule_deviceno, &schedule_info);
        if (test_val == RET_SUCCESSFUL)
        {
            // Initialize data variable and current write index
            pusActivityNode_t activity_node = {0};
            pusNodeIndex_t current_write_index = schedule_info.write_index;

            // Get node at current write index
            test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, current_write_index);

            // Find a new slot if current slot is not available
            while ((test_val == RET_SUCCESSFUL) && (activity_node.status == (pusNodeIndex_t)ACTIVITY_NODE_UNAVAILABLE) && (current_write_index != schedule_info.write_index))
            {
                if (current_write_index == MAXIMUM_ACTIVITIES_PER_SCHEDULE)
                {
                    current_write_index = 0u;
                }
                else
                {
                    current_write_index++;
                }

                // Get New data slot
                test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, current_write_index);
            }

            // Check if no error occured
            if (test_val == RET_SUCCESSFUL)
            {
                // Make sure you haven't gone full circle
                if ((current_write_index == schedule_info.write_index) && (activity_node.status == (pusNodeIndex_t)ACTIVITY_NODE_UNAVAILABLE))
                {
                    return_value = RET_ERROR;
                }
                else
                {
                    // Available node index receive current index
                    *available_node = current_write_index;

                    // Update available info
                    schedule_info.write_index = current_write_index + 1u;
                    // Send it to file
                    test_val = SetInfoFromSchedule(schedule_deviceno, &schedule_info);
                    if (test_val != RET_SUCCESSFUL)
                    {
                        return_value = RET_ERROR;
                    }
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          InsertNodeInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity, pusNodeIndex_t new_node_index)
 * @brief       This function insert new activity in schedule
 * @param[in]   schedule_deviceno Schedule file number from which the oldest activity is inserted
 * @param[in]   activity Oldest activity released content
 * @param[in]   new_node_index New node index
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t InsertNodeInSchedule(deviceNo_t schedule_deviceno, pusActivity_t *activity, pusNodeIndex_t new_node_index)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (activity != NULL)
    {
        // Get schedule info
        pusScheduleInfo_t schedule_info = {0};
        returnCode_t test_val = RET_SUCCESSFUL;
        test_val = GetInfoFromSchedule(schedule_deviceno, &schedule_info);
        if (test_val == RET_SUCCESSFUL)
        {
            // Check if there is at least one node in schedule
            if (schedule_info.nb_activities == 0u)
            {
                // If there is no node in shedule we just add new node
                pusActivityNode_t new_activity_node = {0};

                new_activity_node.status = ACTIVITY_NODE_UNAVAILABLE;
                new_activity_node.activity.timestamp = activity->timestamp;
                new_activity_node.activity.data = activity->data;
                new_activity_node.previous_node_index = UNEXISTING_NODE_INDEX;
                new_activity_node.next_node_index = UNEXISTING_NODE_INDEX;

                // Update node
                test_val = SetNodeFromSchedule(schedule_deviceno, &new_activity_node, new_node_index);
                if (test_val == RET_SUCCESSFUL)
                {
                    // Then update info
                    schedule_info.nb_activities++;
                    schedule_info.oldest_activity_index = new_node_index;
                    test_val = SetInfoFromSchedule(schedule_deviceno, &schedule_info);
                    if (test_val != RET_SUCCESSFUL)
                    {
                        return_value = RET_ERROR;
                    }
                }
            }
            else
            {
                // If there is at least one node we are looking for the node that will be just after new node.
                pusActivityNode_t activity_node = {0};

                // We start with the oldest node.
                pusNodeIndex_t next_node = schedule_info.oldest_activity_index;
                uint32_t counter = 0u;

                // Start looking for the next node
                test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, next_node);
                while ((test_val == RET_SUCCESSFUL) && (activity_node.activity.timestamp <= activity->timestamp) && (activity_node.next_node_index != UNEXISTING_NODE_INDEX) && (counter < MAXIMUM_ACTIVITIES_PER_SCHEDULE))
                {
                    // Update next node
                    next_node = activity_node.next_node_index;
                    test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, next_node);

                    // Increment counter (use to avoid a full turn)
                    counter++;
                }

                // Check if no error occured
                if (test_val == RET_SUCCESSFUL)
                {
                    // While loop stops now we are going to look result
                    if (counter < MAXIMUM_ACTIVITIES_PER_SCHEDULE)
                    {
                        if (activity_node.next_node_index != UNEXISTING_NODE_INDEX)
                        {
                            // We just found the next node
                            pusNodeIndex_t previous_node = activity_node.previous_node_index;

                            // Update new node
                            pusActivityNode_t new_activity_node = {0};
                            new_activity_node.status = ACTIVITY_NODE_UNAVAILABLE;
                            new_activity_node.activity.timestamp = activity->timestamp;
                            new_activity_node.activity.data = activity->data;
                            new_activity_node.previous_node_index = previous_node;
                            new_activity_node.next_node_index = next_node;

                            // Update node
                            test_val = SetNodeFromSchedule(schedule_deviceno, &new_activity_node, new_node_index);
                            if (test_val == RET_SUCCESSFUL)
                            {
                                // Update next node
                                activity_node.previous_node_index = new_node_index;
                                test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, next_node);
                                if (test_val == RET_SUCCESSFUL)
                                {
                                    // Check if new node is not the oldest node
                                    if (previous_node != UNEXISTING_NODE_INDEX)
                                    {
                                        // If there is a previous node, first  we get it
                                        test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, previous_node);
                                        if (test_val == RET_SUCCESSFUL)
                                        {
                                            // Then we update previous node
                                            activity_node.next_node_index = new_node_index;
                                            test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, previous_node);
                                            if (test_val != RET_SUCCESSFUL)
                                            {
                                                return_value = RET_ERROR;
                                            }
                                        }
                                        else
                                        {
                                            return_value = RET_ERROR;
                                        }
                                    }
                                    else
                                    {
                                        // Else new node is the oldest node
                                        schedule_info.oldest_activity_index = new_node_index;
                                    }

                                    // Then update info
                                    schedule_info.nb_activities++;
                                    test_val = SetInfoFromSchedule(schedule_deviceno, &schedule_info);
                                    if (test_val != RET_SUCCESSFUL)
                                    {
                                        return_value = RET_ERROR;
                                    }
                                }
                                else
                                {
                                    return_value = RET_ERROR;
                                }
                            }
                            else
                            {
                                return_value = RET_ERROR;
                            }
                        }
                        else
                        {
                            // Then we reached the end of the linked list. Now check if node is before or after last node.
                            if (activity_node.activity.timestamp <= activity->timestamp)
                            {
                                // It means that in fact next_node is in reality previous node
                                pusNodeIndex_t previous_node = next_node;
                                next_node = UNEXISTING_NODE_INDEX;

                                // Update new node
                                pusActivityNode_t new_activity_node = {0};
                                new_activity_node.status = ACTIVITY_NODE_UNAVAILABLE;
                                new_activity_node.activity.timestamp = activity->timestamp;
                                new_activity_node.activity.data = activity->data;
                                new_activity_node.previous_node_index = previous_node;
                                new_activity_node.next_node_index = next_node;

                                // Update node
                                test_val = SetNodeFromSchedule(schedule_deviceno, &new_activity_node, new_node_index);
                                if (test_val == RET_SUCCESSFUL)
                                {
                                    // Update previous node (which is the node contained in activity_node)
                                    activity_node.next_node_index = new_node_index;
                                    test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, previous_node);
                                    if (test_val == RET_SUCCESSFUL)
                                    {
                                        // Then update info
                                        schedule_info.nb_activities++;
                                        test_val = SetInfoFromSchedule(schedule_deviceno, &schedule_info);
                                        if (test_val != RET_SUCCESSFUL)
                                        {
                                            return_value = RET_ERROR;
                                        }
                                    }
                                    else
                                    {
                                        return_value = RET_ERROR;
                                    }
                                }
                                else
                                {
                                    return_value = RET_ERROR;
                                }
                            }
                            else
                            {
                                // It means that in fact next_node is still the last node
                                pusNodeIndex_t previous_node = activity_node.previous_node_index;

                                // Update new node
                                pusActivityNode_t new_activity_node = {0};
                                new_activity_node.status = ACTIVITY_NODE_UNAVAILABLE;
                                new_activity_node.activity.timestamp = activity->timestamp;
                                new_activity_node.activity.data = activity->data;
                                new_activity_node.previous_node_index = previous_node;
                                new_activity_node.next_node_index = next_node;

                                // Update node
                                test_val = SetNodeFromSchedule(schedule_deviceno, &new_activity_node, new_node_index);
                                if (test_val == RET_SUCCESSFUL)
                                {
                                    // Update next node
                                    activity_node.previous_node_index = new_node_index;
                                    test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, next_node);
                                    if (test_val == RET_SUCCESSFUL)
                                    {
                                        // Check if new node is not the oldest node
                                        if (previous_node != UNEXISTING_NODE_INDEX)
                                        {
                                            // If there is a previous node, first  we get it
                                            test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, previous_node);
                                            if (test_val == RET_SUCCESSFUL)
                                            {
                                                // Then we update previous node
                                                activity_node.next_node_index = new_node_index;
                                                test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, previous_node);
                                                if (test_val != RET_SUCCESSFUL)
                                                {
                                                    return_value = RET_ERROR;
                                                }
                                            }
                                            else
                                            {
                                                return_value = RET_ERROR;
                                            }
                                        }
                                        else
                                        {
                                            // Else new node is the oldest node
                                            schedule_info.oldest_activity_index = new_node_index;
                                        }

                                        // Then update info
                                        schedule_info.nb_activities++;
                                        test_val = SetInfoFromSchedule(schedule_deviceno, &schedule_info);
                                        if (test_val != RET_SUCCESSFUL)
                                        {
                                            return_value = RET_ERROR;
                                        }
                                    }
                                    else
                                    {
                                        return_value = RET_ERROR;
                                    }
                                }
                                else
                                {
                                    return_value = RET_ERROR;
                                }
                            }
                        }
                    }
                    else
                    {
                        // We went around the schedule without finding any node
                        return_value = RET_ERROR;
                    }
                }
                else
                {
                    return_value = RET_ERROR;
                }
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReleaseOldestActivity(deviceNo_t schedule_deviceno, pusActivity_t *activity)
 * @brief       This function releases the oldest activity and update the schedule
 * @param[in]   schedule_deviceno Schedule file number from which the oldest activity is release
 * @param[out]  activity Oldest activity released content
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t ReleaseOldestActivity(deviceNo_t schedule_deviceno, pusActivity_t *activity)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (activity != NULL)
    {
        // First get schedule info
        pusScheduleInfo_t schedule_info = {0};
        returnCode_t test_val = RET_SUCCESSFUL;
        test_val = GetInfoFromSchedule(schedule_deviceno, &schedule_info);
        if (test_val == RET_SUCCESSFUL)
        {
            // Get former oldest node (the one that will be released)
            pusActivityNode_t activity_node = {0};
            pusNodeIndex_t former_oldest_node_index = schedule_info.oldest_activity_index;
            test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, former_oldest_node_index);
            if (test_val == RET_SUCCESSFUL)
            {
                // Get new oldest node
                pusNodeIndex_t new_oldest_node_index = activity_node.next_node_index;

                // First, we save copy of the node content to exiting activity variable
                activity->timestamp = activity_node.activity.timestamp;
                activity->data = activity_node.activity.data;

                // Then, we free the former oldest node
                (void)memset(&activity_node, 0u, ACTIVITY_NODE_SIZE);
                test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, former_oldest_node_index);
                if (test_val == RET_SUCCESSFUL)
                {
                    // Then, we update the new oldest node if it exists
                    if (new_oldest_node_index != UNEXISTING_NODE_INDEX)
                    {
                        // Get new oldest node
                        test_val = GetNodeFromSchedule(schedule_deviceno, &activity_node, new_oldest_node_index);
                        if (test_val == RET_SUCCESSFUL)
                        {
                            activity_node.previous_node_index = UNEXISTING_NODE_INDEX;
                            test_val = SetNodeFromSchedule(schedule_deviceno, &activity_node, new_oldest_node_index);
                            if (test_val != RET_SUCCESSFUL)
                            {
                                return_value = RET_ERROR;
                            }
                        }
                        else
                        {
                            return_value = RET_ERROR;
                        }
                    }

                    // Finally, we update schedule info
                    schedule_info.oldest_activity_index = new_oldest_node_index;
                    schedule_info.nb_activities--;
                    test_val = SetInfoFromSchedule(schedule_deviceno, &schedule_info);
                    if (test_val != RET_SUCCESSFUL)
                    {
                        return_value = RET_ERROR;
                    }
                }
                else
                {
                    return_value = RET_ERROR;
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetInfoFromSchedule(deviceNo_t schedule_deviceno, pusScheduleInfo_t *schedule_info)
 * @brief       Get schedule info from schedule
 * @param[in]   schedule_deviceno Schedule file number
 * @param[out]  schedule_info Infos from schedule
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t GetInfoFromSchedule(deviceNo_t schedule_deviceno, pusScheduleInfo_t *schedule_info)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (schedule_info != NULL)
    {
        // Move the read/write pointer to the beginning (where the schedule info table is located)
        length_t offset = 0u;
        returnCode_t test_fs = DeviceIoctl(schedule_deviceno, IOCTL_FS_SEEK, &offset, sizeof(offset));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then read the schedule info table
            test_fs = DeviceRead(schedule_deviceno, (data_t)schedule_info, SCHEDULE_INFO_SIZE);
            if (test_fs != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetInfoFromSchedule(deviceNo_t schedule_deviceno, pusScheduleInfo_t *schedule_info)
 * @brief       Set schedule info toward schedule
 * @param[in]   schedule_deviceno Schedule file number
 * @param[out]  schedule_info Infos for schedule
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SetInfoFromSchedule(deviceNo_t schedule_deviceno, pusScheduleInfo_t *schedule_info)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (schedule_info != NULL)
    {
        // Move the read/write pointer to the beginning (where the schedule info table is located)
        length_t origin = 0u;
        returnCode_t test_fs = DeviceIoctl(schedule_deviceno, IOCTL_FS_SEEK, &origin, sizeof(origin));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then write the schedule info table
            test_fs = DeviceWrite(schedule_deviceno, (data_t)schedule_info, SCHEDULE_INFO_SIZE);
            if (test_fs != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetNodeFromSchedule(deviceNo_t schedule_deviceno, pusActivityNode_t *activity_node, pusNodeIndex_t node_index)
 * @brief       Get schedule node from schedule
 * @param[in]   schedule_deviceno Schedule file number
 * @param[out]  activity_node Node from schedule
 * @param[in]   node_index node index
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t GetNodeFromSchedule(deviceNo_t schedule_deviceno, pusActivityNode_t *activity_node, pusNodeIndex_t node_index)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (activity_node != NULL)
    {
        // Move the read/write pointer to the desired data field
        length_t offset = SCHEDULE_INFO_SIZE + (node_index * ACTIVITY_NODE_SIZE);
        returnCode_t test_fs = DeviceIoctl(schedule_deviceno, IOCTL_FS_SEEK, &offset, sizeof(offset));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then read data in table
            test_fs = DeviceRead(schedule_deviceno, (data_t)activity_node, ACTIVITY_NODE_SIZE);
            if (test_fs != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetNodeFromSchedule(deviceNo_t schedule_deviceno, pusActivityNode_t *activity_node, pusNodeIndex_t node_index)
 * @brief       Set schedule node toward schedule
 * @param[in]   schedule_deviceno Schedule file number
 * @param[out]  activity_node Node for schedule
 * @param[in]   node_index node index
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SetNodeFromSchedule(deviceNo_t schedule_deviceno, pusActivityNode_t *activity_node, pusNodeIndex_t node_index)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (activity_node != NULL)
    {
        // Move the read/write pointer to the desired data field
        length_t offset = SCHEDULE_INFO_SIZE + (node_index * ACTIVITY_NODE_SIZE);
        returnCode_t test_fs = DeviceIoctl(schedule_deviceno, IOCTL_FS_SEEK, &offset, sizeof(offset));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then write data in table
            test_fs = DeviceWrite(schedule_deviceno, (data_t)activity_node, ACTIVITY_NODE_SIZE);
            if (test_fs != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
