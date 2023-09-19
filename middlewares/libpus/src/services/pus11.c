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
#include "tolosat_fs.h"
#include "conf/fs_conf.h"
#include "pus_tools/schedule_management.h"
#include "pus_tools/time_management.h"

/***************************** Macros Definitions ****************************/

#define ZERO_FILLED_DATA_SIZE 1024u /**< Size of zero filled data (used for reset purposes) */

/*************************** Functions Declarations **************************/

static pusStatus_t GetAvailableData(pus11DataIndex_t *data_index);
static pusStatus_t ResetScheduleAndData(void);
static pusStatus_t GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info);
static pusStatus_t SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info);
static pusStatus_t GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index);
static pusStatus_t SetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus11_status
 * @brief   Indicates if pus11 is enable or disable
 */
pus11Status_t g_pus11_status = PUS11_ENABLE;

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitPus11(void)
 * @brief   This function init pus 11 files
 * @retval  #PUS_SUCCESSFUL always
 */
pusStatus_t InitPus11(void)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    fsSize_t file_size = 0;
    fsStatus_t test_fs;

    // Function Core
    // Check if pus11 files are complete
    test_fs = FsGetFileSize(PUS11_SCHED_FILE, &file_size);
    if ((test_fs == FS_SUCCESSFUL) && (file_size == SCHEDULE_SIZE))
    {
        test_fs = FsGetFileSize(PUS11_DATA_FILE, &file_size);
        if ((test_fs == FS_SUCCESSFUL) && (file_size == PUS11_DATA_TABLE_SIZE))
        {
            return_value = PUS_SUCCESSFUL;
        }
        else if ((test_fs == FS_SUCCESSFUL) && (file_size != PUS11_DATA_TABLE_SIZE))
        {
            // Pus11 files are incomplete
            pusStatus_t test_reset = ResetScheduleAndData();
            if (test_reset != PUS_SUCCESSFUL)
            {
                return_value = PUS_ERROR;
            }
        }
        else
        {
            return_value = PUS_ERROR;
        }
    }
    else if ((test_fs == FS_SUCCESSFUL) && (file_size != SCHEDULE_SIZE))
    {
        // Pus11 files are incomplete
        pusStatus_t test_reset = ResetScheduleAndData();
        if (test_reset != PUS_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_ERROR;
    }

    return return_value;
}

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
    // Unused Parameters
    (void)(tc);
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Reset schedule
        (void)memset((void *)&g_pus11_schedule, 0u, SCHEDULE_SIZE);

        // Reset data table
        (void)memset((void *)&g_pus11_data_table, 0u, PUS11_DATA_TABLE_SIZE);
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
    pusAddActivityTCDataField_t tc_data = {0};
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
            (void)memcpy((void *)&tc_data, (void *)tc->data, TC_MAX_DATA_SIZE);

            // Get Current time
            cucTime_t current_time = {0};
            test_val = GetCUCTime(&current_time);
            if (test_val == PUS_SUCCESSFUL)
            {
                // Check if requested timestamp is in the futur
                test_val = CompareCUCTimes(&current_time, &tc_data.timestamp);
                if (test_val == PUS_SUCCESSFUL)
                {
                    // Check if there is still data available
                    if (g_pus11_data_table.info.nb_data < PUS11_MAXIMUM_DATA)
                    {
                        // Get a data slot
                        pus11DataIndex_t new_data_index = 0u;
                        test_val = GetAvailableData(&new_data_index);
                        if (test_val == PUS_SUCCESSFUL)
                        {
                            // Put data in data table
                            (void)memcpy((void *)&g_pus11_data_table.data[new_data_index].raw_data, (void *)tc_data.data, PUS11_ACTIVITY_DATA_MAX_SIZE);
                            g_pus11_data_table.data[new_data_index].status = PUS11_DATA_UNAVAILABLE;
                            g_pus11_data_table.info.nb_data++;

                            // Create Activity based on TC data
                            pusActivity_t activity = {0};
                            activity.timestamp = tc_data.timestamp;
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
 * @fn          GetDelayedTC(pusTC_t *delayed_tc)
 * @brief       Get delayed TC if there is any available
 * @param[out]  delayed_tc Delayed TC that was freed
 * @retval      #PUS_INVALID_PARAM if delayed_tc is null pointer
 * @retval      #PUS_NOT_AVAILABLE if there is not delayed tc available
 * @retval      #PUS_ERROR if an error occured
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t GetDelayedTC(pusTC_t *delayed_tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusStatus_t test_val;

    // Function Core
    if (delayed_tc != NULL)
    {
        // Get last activity in schedule
        pusActivity_t freed_activity = {0};
        test_val = PopActivityInSchedule(&g_pus11_schedule, &freed_activity);
        if (test_val == PUS_SUCCESSFUL)
        {
            // Now we are getting data from the data table
            (void)memcpy((void *)delayed_tc, (void *)&g_pus11_data_table.data[freed_activity.data].raw_data, PUS11_ACTIVITY_DATA_MAX_SIZE);

            // Then we free data in table
            (void)memset((void *)&g_pus11_data_table.data[freed_activity.data].raw_data, 0u, PUS11_ACTIVITY_DATA_MAX_SIZE);
            g_pus11_data_table.data[freed_activity.data].status = PUS11_DATA_AVAILABLE;
            g_pus11_data_table.info.nb_data--;
        }
        else if (test_val == PUS_NOT_AVAILABLE)
        {
            return_value = PUS_NOT_AVAILABLE;
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
 * @fn              GetAvailableData(pus11DataTable_t *data_table, pus11DataIndex_t *data_index)
 * @brief           This function gets the closest available data from the writing pointer
 * @param[out]      data_index New data index
 * @retval          #PUS_INVALID_PARAM if a pointer is NULL
 * @retval          #PUS_ERROR if no data is available
 * @retval          #PUS_SUCCESSFUL else
 */
static pusStatus_t GetAvailableData(pus11DataIndex_t *data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (data_index != NULL)
    {
        pus11DataIndex_t current_write_index = g_pus11_data_table.info.write_index;
        while ((g_pus11_data_table.data[current_write_index].status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE) && (current_write_index != g_pus11_data_table.info.write_index))
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
        if ((current_write_index == g_pus11_data_table.info.write_index) && (g_pus11_data_table.data[current_write_index].status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE))
        {
            return_value = PUS_ERROR;
        }
        else
        {
            // Update available data and write index
            *data_index = current_write_index;
            g_pus11_data_table.info.write_index = current_write_index + 1u;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn      ResetScheduleAndData(void)
 * @brief   This function reset schedule and data file (filling them with zeros)
 * @retval  #PUS_ERROR if write in FS has encountered an error
 * @retval  #PUS_SUCCESSFUL else
 */
static pusStatus_t ResetScheduleAndData(void)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    uint8_t zero_filled_data[ZERO_FILLED_DATA_SIZE] = {0};
    fsStatus_t write_status = FS_SUCCESSFUL;
    uint32_t reset_bytes;
    uint32_t diff;

    // Function Core
    // Delete data from pus11 sched file
    reset_bytes = 0u;
    while ((write_status == FS_SUCCESSFUL) && (reset_bytes < SCHEDULE_SIZE))
    {
        // I did a cpp-suppress because i can't see the problem (maybe a false positive)
        diff = (SCHEDULE_SIZE - reset_bytes); // cppcheck-suppress misra-c2012-10.7
        if (diff >= ZERO_FILLED_DATA_SIZE)
        {
            write_status = FsWrite(PUS11_SCHED_FILE, reset_bytes, (fsData_t *)&zero_filled_data, ZERO_FILLED_DATA_SIZE);
            reset_bytes += ZERO_FILLED_DATA_SIZE;
        }
        else
        {
            write_status = FsWrite(PUS11_SCHED_FILE, reset_bytes, (fsData_t *)&zero_filled_data, diff);
            reset_bytes += diff;
        }
    }

    // Delete data from pus11 data file
    reset_bytes = 0u;
    while ((write_status == FS_SUCCESSFUL) && (reset_bytes < PUS11_DATA_TABLE_SIZE))
    {
        // I did a cpp-suppress because i can't see the problem (maybe a false positive)
        diff = (PUS11_DATA_TABLE_SIZE - reset_bytes); // cppcheck-suppress misra-c2012-10.7
        if (diff >= ZERO_FILLED_DATA_SIZE)
        {
            write_status = FsWrite(PUS11_DATA_FILE, reset_bytes, (fsData_t *)&zero_filled_data, ZERO_FILLED_DATA_SIZE);
            reset_bytes += ZERO_FILLED_DATA_SIZE;
        }
        else
        {
            write_status = FsWrite(PUS11_DATA_FILE, reset_bytes, (fsData_t *)&zero_filled_data, diff);
            reset_bytes += diff;
        }
    }

    // Check if write went well
    if (write_status != FS_SUCCESSFUL)
    {
        return_value = PUS_ERROR;
    }

    return return_value;
}

/**
 * @fn          GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
 * @brief       Get PUS11 info from data table
 * @param[out]  pus11_table_info Infos from pus11 table
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    fsStatus_t fs_status;

    // Function Core
    if (pus11_table_info != NULL)
    {
        fs_status = FsRead(PUS11_DATA_FILE, 0u, (fsData_t *)pus11_table_info, PUS11_DATA_TABLE_INFO_SIZE);
        if (fs_status != FS_SUCCESSFUL)
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
 * @fn          SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
 * @brief       Set PUS11 info from data table
 * @param[in]   pus11_table_info Infos for pus11 table
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    fsStatus_t fs_status;

    // Function Core
    if (pus11_table_info != NULL)
    {
        fs_status = FsWrite(PUS11_DATA_FILE, 0u, (fsData_t *)pus11_table_info, PUS11_DATA_TABLE_INFO_SIZE);
        if (fs_status != FS_SUCCESSFUL)
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
 * @fn          GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
 * @brief       Get PUS11 data from data table
 * @param[out]  pus11_data Data from pus11 table
 * @param[in]   data_index Data index
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    fsStatus_t fs_status;

    // Function Core
    if (pus11_data != NULL)
    {
        // I did a cpp-suppress because i can't see the problem (maybe a false positive)
        fsSize_t offset = PUS11_DATA_TABLE_INFO_SIZE + (data_index * PUS11_MAXIMUM_DATA_SIZE); // cppcheck-suppress misra-c2012-10.7
        fs_status = FsRead(PUS11_DATA_FILE, offset, (fsData_t *)pus11_data, PUS11_MAXIMUM_DATA_SIZE);
        if (fs_status != FS_SUCCESSFUL)
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
 * @fn          SetDataFromTable(pus11Data_t *pus11_data)
 * @brief       Set PUS11 data from data table
 * @param[in]   pus11_data Data for pus11 table
 * @param[in]   data_index Data index
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t SetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    fsStatus_t fs_status;

    // Function Core
    if (pus11_data != NULL)
    {
        // I did a cpp-suppress because i can't see the problem (maybe a false positive)
        fsSize_t offset = PUS11_DATA_TABLE_INFO_SIZE + (data_index * PUS11_MAXIMUM_DATA_SIZE); // cppcheck-suppress misra-c2012-10.7
        fs_status = FsWrite(PUS11_DATA_FILE, offset, (fsData_t *)pus11_data, PUS11_MAXIMUM_DATA_SIZE);
        if (fs_status != FS_SUCCESSFUL)
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
