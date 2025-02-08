/**
 * @file    pus11.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 11 functions (Time-based scheduling)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus11.h"
#include "tools/schedule_management.h"

/***************************** Macros Definitions ****************************/

#define ZERO_FILLED_DATA_SIZE 512u /**< Size of zero filled data (used for reset purposes) */

/*************************** Functions Declarations **************************/

static returnCode_t GetDelayedTC(pusTC_t *delayed_tc, time_t *next_tc_release_date);
static returnCode_t GetAvailableData(pus11DataIndex_t *data_index);
static returnCode_t ResetScheduleAndData(void);
static returnCode_t GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info);
static returnCode_t SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info);
static returnCode_t GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index);
static returnCode_t SetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index);

/*************************** Variables Definitions ***************************/

/**
 * @var     pus11_context_pointer
 * @brief   Pointer to the pus11 context
 */
static pus11Context_t *pus11_context_pointer;

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitPus11(pus11Context_t *pus11_context)
 * @brief       This function init pus 11 files
 * @param[in]   pus11_context PUS11 context used for configuration
 * @retval      #RET_SUCCESSFUL always
 */
returnCode_t InitPus11(pus11Context_t *pus11_context)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    length_t file_size        = 0;

    // First set pus11_context_pointer with the correct context
    pus11_context_pointer = pus11_context;

    // Then initialises the devices
    return_value = DeviceOpen(&pus11_context->dev_pus11_schedule, DEVICE_TYPE_FILE, pus11_context->fil_pus11_schedule);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = DeviceOpen(&pus11_context->dev_pus11_data, DEVICE_TYPE_FILE, pus11_context->fil_pus11_data);
        if (return_value == RET_SUCCESSFUL)
        {
            // Check if pus11 files are complete
            return_value = DeviceIoctl(pus11_context->dev_pus11_schedule, IOCTL_FS_GET_SIZE, &file_size, sizeof(length_t));
            if (return_value == RET_SUCCESSFUL)
            {
                if (file_size == SCHEDULE_SIZE)
                {
                    return_value = DeviceIoctl(pus11_context->dev_pus11_data, IOCTL_FS_GET_SIZE, &file_size, sizeof(length_t));
                    if (return_value == RET_SUCCESSFUL)
                    {
                        if (file_size == PUS11_DATA_TABLE_SIZE)
                        {
                            // Pus11 files are complete
                            return_value = RET_SUCCESSFUL;
                        }
                        else
                        {
                            // Pus11 files are incomplete
                            return_value = ResetScheduleAndData();
                        }
                    }
                }
                else
                {
                    // Pus11 files are incomplete
                    return_value = ResetScheduleAndData();
                }
            }
        }
    }

    // Then initialise delayed TC buffer device
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = DeviceOpen(&pus11_context->dev_delayed_tc, DEVICE_TYPE_BUFFER, pus11_context->buffer_delayed_tc);
    }

    return return_value;
}

/**
 * @fn          ReleaseDelayedTC(pus11Context_t *pus11_context, time_t *next_tc_release_date)
 * @brief       Function that tries to release a delayed tc and transfer to the delayed tc buffer
 * @param[in]   pus11_context           PUS11 context used for configuration
 * @param[out]  next_tc_release_date    Next TC release date
 * @retval      #RET_NOT_AVAILABLE if no delayed TC is available
 * @retval      #RET_ERROR if schedule encountered an error
 * @retval      #RET_ERROR if device writting failed
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReleaseDelayedTC(pus11Context_t *pus11_context, time_t *next_tc_release_date)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    pusTC_t delayed_tc        = { 0 };

    // Check if pus11 is enabled
    if ((pus11_context != NULL) && (pus11_context->pus11_status == PUS11_ENABLE))
    {
        // Get delayed TC if there is any
        return_value = GetDelayedTC(&delayed_tc, next_tc_release_date);
        if (return_value == RET_SUCCESSFUL)
        {
            // Delayed TC available, send it to TC receiver
            return_value = DeviceWrite(pus11_context->dev_delayed_tc, (data_t)&delayed_tc, TC_MAX_SIZE);
            if (return_value == RET_SUCCESSFUL)
            {
                return_value = SendSignal(TC_RECEIVER_TASK, SIGNAL_NEW_TC);
            }
        }
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will enable time based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused Parameters
    (void)(tc);
    (void)(tm);

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Enable PUS11
        pus11_context_pointer->pus11_status = PUS11_ENABLE;
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will disable time based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused Parameters
    (void)(tc);
    (void)(tm);

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Enable PUS11
        pus11_context_pointer->pus11_status = PUS11_DISABLE;
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will reset time-based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_NOT_AVAILABLE if cannot reset the schedule
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused Parameters
    (void)(tc);
    (void)(tm);

    // Check parameter(s)
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Reset pus11 files
        returnCode_t test_reset = ResetScheduleAndData();
        if (test_reset != RET_SUCCESSFUL)
        {
            return_value = RET_NOT_AVAILABLE;
            *error_code  = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that will add activity to a time based schedule
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_NOT_AVAILABLE if the PUS11 has been disabled
 * @retval      #RET_NOT_AVAILABLE if the tc timestamp is outdated
 * @retval      #RET_ERROR if an error has been encountered related to the schedule
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value           = RET_SUCCESSFUL;
    pusAddActivityTCDataField_t tc_data = { 0 };

    // Unused Parameters
    (void)(tm);

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Check if PUS11 is enable
        if (pus11_context_pointer->pus11_status == PUS11_ENABLE)
        {
            // Get data from TC
            (void)memcpy((void *)&tc_data, (void *)tc->data, TC_MAX_DATA_SIZE);

            // Get Current time
            time_t current_time    = 0u;
            returnCode_t test_time = GetTime(&current_time);
            if (test_time == RET_SUCCESSFUL)
            {
                // Check if requested timestamp is in the futur
                time_t tc_timestamp = ((uint64_t)(tc_data.timestamp.time_header) << 56) | ((uint64_t)(tc_data.timestamp.coarse_time[0]) << 48)
                                      | ((uint64_t)(tc_data.timestamp.coarse_time[1]) << 40) | ((uint64_t)(tc_data.timestamp.coarse_time[2]) << 32)
                                      | ((uint64_t)(tc_data.timestamp.coarse_time[3]) << 24) | ((uint64_t)(tc_data.timestamp.fine_time[0]) << 16)
                                      | ((uint64_t)(tc_data.timestamp.fine_time[1]) << 8) | ((uint64_t)(tc_data.timestamp.fine_time[2]));
                if (current_time <= tc_timestamp)
                {
                    pus11DataTableInfo_t pus11_table_info = { 0 };
                    // Check if there is still data available
                    return_value = GetInfoFromTable(&pus11_table_info);
                    if ((return_value == RET_SUCCESSFUL) && (pus11_table_info.nb_data < PUS11_MAXIMUM_DATA))
                    {
                        pus11DataIndex_t new_data_index = 0u;
                        // Get a data slot
                        return_value = GetAvailableData(&new_data_index);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Put incomming data in data struct
                            pus11Data_t pus11_data = { 0 };
                            (void)memcpy((void *)&pus11_data.raw_data, (void *)tc_data.data, PUS11_ACTIVITY_DATA_MAX_SIZE);
                            pus11_data.status = PUS11_DATA_UNAVAILABLE;

                            // Send data to file
                            return_value = SetDataFromTable(&pus11_data, new_data_index);
                            if (return_value == RET_SUCCESSFUL)
                            {
                                // Create Activity based on TC data
                                pusActivity_t activity = { 0 };
                                activity.timestamp     = tc_timestamp;
                                activity.data          = new_data_index;

                                // Insert activity in schedule
                                return_value = PushActivityInSchedule(pus11_context_pointer->dev_pus11_schedule, &activity);
                                if (return_value != RET_SUCCESSFUL)
                                {
                                    *error_code = PUS_EXECUTION_FAILED;
                                }
                            }
                            else
                            {
                                *error_code = PUS_EXECUTION_FAILED;
                            }
                        }
                        else
                        {
                            *error_code = PUS_EXECUTION_FAILED;
                        }
                    }
                    else
                    {
                        *error_code = PUS_EXECUTION_FAILED;
                    }
                }
                else
                {
                    return_value = RET_NOT_AVAILABLE;
                    *error_code  = PUS_EXECUTION_UNEXPECTED_DATA;
                }
            }
            else
            {
                return_value = RET_ERROR;
                *error_code  = PUS_EXECUTION_FAILED;
            }
        }
        else
        {
            return_value = RET_NOT_AVAILABLE;
            *error_code  = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetDelayedTC(pusTC_t *delayed_tc, time_t *next_tc_release_date)
 * @brief       Get delayed TC if there is any available
 * @param[out]  delayed_tc              Delayed TC that was freed
 * @param[out]  next_tc_release_date    Next TC release date
 * @retval      #RET_INVALID_PARAM if delayed_tc is null pointer
 * @retval      #RET_NOT_AVAILABLE if there is not delayed tc available
 * @retval      #RET_ERROR if an error occured
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t GetDelayedTC(pusTC_t *delayed_tc, time_t *next_tc_release_date)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (delayed_tc != NULL))
    {
        pusActivity_t freed_activity = { 0 };
        // Get last activity in schedule
        return_value = PopActivityInSchedule(pus11_context_pointer->dev_pus11_schedule, &freed_activity, next_tc_release_date);
        if (return_value == RET_SUCCESSFUL)
        {
            pus11Data_t pus11_data = { 0 };
            // Get data from file
            return_value = GetDataFromTable(&pus11_data, freed_activity.data);
            if (return_value == RET_SUCCESSFUL)
            {
                // Now we are getting data from the data table
                (void)memcpy((void *)delayed_tc, (void *)&pus11_data.raw_data, PUS11_ACTIVITY_DATA_MAX_SIZE);

                // Then we free data
                (void)memset((void *)&pus11_data.raw_data, 0u, PUS11_ACTIVITY_DATA_MAX_SIZE);
                pus11_data.status = PUS11_DATA_AVAILABLE;

                // Send this updated data to file
                return_value = SetDataFromTable(&pus11_data, freed_activity.data);
                if (return_value == RET_SUCCESSFUL)
                {
                    pus11DataTableInfo_t pus11_table_info = { 0 };
                    // Get current info before update
                    return_value = GetInfoFromTable(&pus11_table_info);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Update data number
                        pus11_table_info.nb_data--;
                        return_value = SetInfoFromTable(&pus11_table_info);
                    }
                }
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
 * @fn              GetAvailableData(pus11DataTable_t *data_table, pus11DataIndex_t *data_index)
 * @brief           This function gets the closest available data from the writing pointer
 * @param[out]      data_index New data index
 * @retval          #RET_INVALID_PARAM if a pointer is NULL
 * @retval          #RET_NOT_AVAILABLE if no data is available
 * @retval          #RET_ERROR if FS has encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t GetAvailableData(pus11DataIndex_t *data_index)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (data_index != NULL)
    {
        pus11DataTableInfo_t pus11_table_info = { 0 };
        // First get table info
        returnCode_t test_val = GetInfoFromTable(&pus11_table_info);
        if (test_val == RET_SUCCESSFUL)
        {
            // Initialize data variable and current write index
            pus11Data_t pus11_data               = { 0 };
            pus11DataIndex_t current_write_index = pus11_table_info.write_index;

            // Get data at current write index
            test_val = GetDataFromTable(&pus11_data, current_write_index);

            // Find a new slot if current slot is not available
            while ((test_val == RET_SUCCESSFUL) && (pus11_data.status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE)
                   && (current_write_index != pus11_table_info.write_index))
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
                test_val = GetDataFromTable(&pus11_data, current_write_index);
            }

            // Check if no error occured
            if (test_val == RET_SUCCESSFUL)
            {
                // Make sure you haven't gone full circle
                if ((current_write_index == pus11_table_info.write_index) && (pus11_data.status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE))
                {
                    return_value = RET_NOT_AVAILABLE;
                }
                else
                {
                    // Data index receive current index
                    *data_index = current_write_index;

                    // Update available info
                    pus11_table_info.write_index = current_write_index + 1u;
                    pus11_table_info.nb_data++;
                    // Send it to file
                    test_val = SetInfoFromTable(&pus11_table_info);
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
 * @fn      ResetScheduleAndData(void)
 * @brief   This function reset schedule and data file (filling them with zeros)
 * @retval  #RET_ERROR if write in FS has encountered an error
 * @retval  #RET_SUCCESSFUL else
 */
static returnCode_t ResetScheduleAndData(void)
{
    returnCode_t return_value                      = RET_SUCCESSFUL;
    data_t zero_filled_data[ZERO_FILLED_DATA_SIZE] = { 0 };
    length_t origin                                = 0u;

    // Check parameter(s)
    if (pus11_context_pointer != NULL)
    {
        // Delete data from pus11 sched file
        // Set read/write pointer to the beginning of the file
        return_value = DeviceIoctl(pus11_context_pointer->dev_pus11_schedule, IOCTL_FS_SEEK, &origin, sizeof(origin));
        if (return_value == RET_SUCCESSFUL)
        {
            // Write 0s in the file
            length_t remaining_bytes = SCHEDULE_SIZE; // cppcheck-suppress misra-c2012-10.6; False positive, there is no wider type asignment, SCHEDULE_SIZE is uint32_t
            while ((return_value == RET_SUCCESSFUL) && (remaining_bytes > 0u))
            {
                if (remaining_bytes >= ZERO_FILLED_DATA_SIZE)
                {
                    return_value     = DeviceWrite(pus11_context_pointer->dev_pus11_schedule, (data_t)&zero_filled_data, ZERO_FILLED_DATA_SIZE);
                    remaining_bytes -= ZERO_FILLED_DATA_SIZE;
                }
                else
                {
                    return_value    = DeviceWrite(pus11_context_pointer->dev_pus11_schedule, (data_t)&zero_filled_data, remaining_bytes);
                    remaining_bytes = 0u;
                }
            }

            // Delete data from pus11 data file
            // Set read/write pointer to the beginning of the file
            return_value = DeviceIoctl(pus11_context_pointer->dev_pus11_data, IOCTL_FS_SEEK, &origin, sizeof(origin));
            if (return_value == RET_SUCCESSFUL)
            {
                // Write 0s in the file
                remaining_bytes = PUS11_DATA_TABLE_SIZE; // cppcheck-suppress misra-c2012-10.6; False positive, there is no wider type asignment, PUS11_DATA_TABLE_SIZE is uint32_t
                while ((return_value == RET_SUCCESSFUL) && (remaining_bytes > 0u))
                {
                    if (remaining_bytes >= ZERO_FILLED_DATA_SIZE)
                    {
                        return_value     = DeviceWrite(pus11_context_pointer->dev_pus11_data, (data_t)&zero_filled_data, ZERO_FILLED_DATA_SIZE);
                        remaining_bytes -= ZERO_FILLED_DATA_SIZE;
                    }
                    else
                    {
                        return_value    = DeviceWrite(pus11_context_pointer->dev_pus11_data, (data_t)&zero_filled_data, remaining_bytes);
                        remaining_bytes = 0u;
                    }
                }
            }
        }
    }

    return return_value;
}

/**
 * @fn          GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
 * @brief       Get PUS11 info from data table
 * @param[out]  pus11_table_info Infos from pus11 table
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (pus11_table_info != NULL))
    {
        length_t origin = 0u;
        // Move the read/write pointer to the beginning (where the info table is located)
        returnCode_t test_fs = DeviceIoctl(pus11_context_pointer->dev_pus11_data, IOCTL_FS_SEEK, &origin, sizeof(origin));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then read the info table
            test_fs = DeviceRead(pus11_context_pointer->dev_pus11_data, (data_t)pus11_table_info, PUS11_DATA_TABLE_INFO_SIZE);
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
 * @fn          SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
 * @brief       Set PUS11 info from data table
 * @param[in]   pus11_table_info Infos for pus11 table
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (pus11_table_info != NULL))
    {
        length_t origin = 0u;
        // Move the read/write pointer to the beginning (where the info table is located)
        returnCode_t test_fs = DeviceIoctl(pus11_context_pointer->dev_pus11_data, IOCTL_FS_SEEK, &origin, sizeof(origin));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then write the info table
            test_fs = DeviceWrite(pus11_context_pointer->dev_pus11_data, (data_t)pus11_table_info, PUS11_DATA_TABLE_INFO_SIZE);
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
 * @fn          GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
 * @brief       Get PUS11 data from data table
 * @param[out]  pus11_data Data from pus11 table
 * @param[in]   data_index Data index
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (pus11_data != NULL))
    {
        // Move the read/write pointer to the desired data field
        length_t offset = PUS11_DATA_TABLE_INFO_SIZE + (data_index * PUS11_MAXIMUM_DATA_SIZE); // cppcheck-suppress misra-c2012-10.7; False positive, there is no wider type arithmetic conversion, (data_index * PUS11_MAXIMUM_DATA_SIZE) is a uint32_t
        returnCode_t test_fs = DeviceIoctl(pus11_context_pointer->dev_pus11_data, IOCTL_FS_SEEK, &offset, sizeof(offset));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then read data in table
            test_fs = DeviceRead(pus11_context_pointer->dev_pus11_data, (data_t)pus11_data, PUS11_MAXIMUM_DATA_SIZE);
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
 * @fn          SetDataFromTable(pus11Data_t *pus11_data)
 * @brief       Set PUS11 data from data table
 * @param[in]   pus11_data Data for pus11 table
 * @param[in]   data_index Data index
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if write in FS has encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((pus11_context_pointer != NULL) && (pus11_data != NULL))
    {
        // Move the read/write pointer to the desired data field
        length_t offset = PUS11_DATA_TABLE_INFO_SIZE + (data_index * PUS11_MAXIMUM_DATA_SIZE); // cppcheck-suppress misra-c2012-10.7; False positive, there is no wider type arithmetic conversion, (data_index * PUS11_MAXIMUM_DATA_SIZE) is a uint32_t
        returnCode_t test_fs = DeviceIoctl(pus11_context_pointer->dev_pus11_data, IOCTL_FS_SEEK, &offset, sizeof(offset));
        if (test_fs == RET_SUCCESSFUL)
        {
            // Then write data in table
            test_fs = DeviceWrite(pus11_context_pointer->dev_pus11_data, (data_t)pus11_data, PUS11_MAXIMUM_DATA_SIZE);
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
