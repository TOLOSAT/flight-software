/**
 * @file    pus11.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 11 functions (Time-based scheduling)
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "kernel.h"

#include "pus.h"
#include "services/pus11.h"

/***************************** Macros Definitions ****************************/

#define ZERO_FILLED_DATA_SIZE 512u /**< Size of zero filled data (used for reset purposes) */

/*************************** Functions Declarations **************************/

static pusStatus_t IN_PUS_TEXT_SECTION GetAvailableData(pus11DataIndex_t *data_index);
static pusStatus_t IN_PUS_TEXT_SECTION ResetScheduleAndData(void);
static pusStatus_t IN_PUS_TEXT_SECTION GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info);
static pusStatus_t IN_PUS_TEXT_SECTION SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info);
static pusStatus_t IN_PUS_TEXT_SECTION GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index);
static pusStatus_t IN_PUS_TEXT_SECTION SetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index);

/*************************** Variables Definitions ***************************/

/**
 * @var     pus11_status
 * @brief   Indicates if pus11 is enable or disable
 */
static pus11Status_t IN_PUS_DATA_SECTION pus11_status = PUS11_ENABLE;

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitPus11(void)
 * @brief   This function init pus 11 files
 * @retval  #PUS_SUCCESSFUL always
 */
pusStatus_t IN_PUS_TEXT_SECTION InitPus11(void)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    length_t file_size = 0;
    kernelStatus_t test_fs;

    // Function Core
    // Check if pus11 files are complete
    test_fs = FsIoctl(PUS11_SCHED_FILE, FS_IOCTL_GET_SIZE, &file_size, sizeof(length_t));
    if ((test_fs == KERNEL_SUCCESSFUL) && (file_size == SCHEDULE_SIZE))
    {
        test_fs = FsIoctl(PUS11_DATA_FILE, FS_IOCTL_GET_SIZE, &file_size, sizeof(length_t));
        if ((test_fs == KERNEL_SUCCESSFUL) && (file_size == PUS11_DATA_TABLE_SIZE))
        {
            return_value = PUS_SUCCESSFUL;
        }
        else if ((test_fs == KERNEL_SUCCESSFUL) && (file_size != PUS11_DATA_TABLE_SIZE))
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
    else if ((test_fs == KERNEL_SUCCESSFUL) && (file_size != SCHEDULE_SIZE))
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
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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
        pus11_status = PUS11_ENABLE;
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
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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
        pus11_status = PUS11_DISABLE;
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
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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

        // Reset pus11 files
        pusStatus_t test_reset = ResetScheduleAndData();
        if (test_reset != PUS_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
            *error_code = PUS_EXECUTION_FAILED;
        }
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
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusAddActivityTCDataField_t tc_data = {0};

    // Function Core
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Check if PUS11 is enable
        if (pus11_status == PUS11_ENABLE)
        {
            // Get data from TC
            (void)memcpy((void *)&tc_data, (void *)tc->data, TC_MAX_DATA_SIZE);

            // Get Current time
            time_t current_time = 0u;
            kernelStatus_t test_time = GetTime(&current_time);
            if (test_time == KERNEL_SUCCESSFUL)
            {
                // Check if requested timestamp is in the futur
                time_t tc_timestamp = ((uint64_t)(tc_data.timestamp.time_header) << 56) | \
                                      ((uint64_t)(tc_data.timestamp.coarse_time[0]) << 48) | \
                                      ((uint64_t)(tc_data.timestamp.coarse_time[1]) << 40) | \
                                      ((uint64_t)(tc_data.timestamp.coarse_time[2]) << 32) | \
                                      ((uint64_t)(tc_data.timestamp.coarse_time[3]) << 24) | \
                                      ((uint64_t)(tc_data.timestamp.fine_time[0]) << 16) | \
                                      ((uint64_t)(tc_data.timestamp.fine_time[1]) << 8) | \
                                      ((uint64_t)(tc_data.timestamp.fine_time[2]));
                if (current_time <= tc_timestamp)
                {
                    // Check if there is still data available
                    pus11DataTableInfo_t pus11_table_info = {0};
                    pusStatus_t test_val = GetInfoFromTable(&pus11_table_info);
                    if ((test_val == PUS_SUCCESSFUL) && (pus11_table_info.nb_data < PUS11_MAXIMUM_DATA))
                    {
                        // Get a data slot
                        pus11DataIndex_t new_data_index = 0u;
                        test_val = GetAvailableData(&new_data_index);
                        if (test_val == PUS_SUCCESSFUL)
                        {
                            // Put incomming data in data struct
                            pus11Data_t pus11_data = {0};
                            (void)memcpy((void *)&pus11_data.raw_data, (void *)tc_data.data, PUS11_ACTIVITY_DATA_MAX_SIZE);
                            pus11_data.status = PUS11_DATA_UNAVAILABLE;

                            // Send data to file
                            test_val = SetDataFromTable(&pus11_data, new_data_index);
                            if (test_val == PUS_SUCCESSFUL)
                            {
                                // Create Activity based on TC data
                                pusActivity_t activity = {0};
                                activity.timestamp = tc_timestamp;
                                activity.data = new_data_index;

                                // Insert activity in schedule
                                test_val = PushActivityInSchedule(PUS11_SCHED_FILE, &activity);
                                if (test_val != PUS_SUCCESSFUL)
                                {
                                    return_value = PUS_ERROR;
                                    *error_code = PUS_EXECUTION_FAILED;
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
                            *error_code = PUS_EXECUTION_FAILED;
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
            *error_code = PUS_EXECUTION_FAILED;
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
pusStatus_t IN_PUS_TEXT_SECTION GetDelayedTC(pusTC_t *delayed_tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (delayed_tc != NULL)
    {
        // Get last activity in schedule
        pusActivity_t freed_activity = {0};
        pusStatus_t test_val = PopActivityInSchedule(PUS11_SCHED_FILE, &freed_activity);
        if (test_val == PUS_SUCCESSFUL)
        {
            pus11Data_t pus11_data = {0};
            // Get data from file
            test_val = GetDataFromTable(&pus11_data, freed_activity.data);
            if (test_val == PUS_SUCCESSFUL)
            {
                // Now we are getting data from the data table
                (void)memcpy((void *)delayed_tc, (void *)&pus11_data.raw_data, PUS11_ACTIVITY_DATA_MAX_SIZE);

                // Then we free data
                (void)memset((void *)&pus11_data.raw_data, 0u, PUS11_ACTIVITY_DATA_MAX_SIZE);
                pus11_data.status = PUS11_DATA_AVAILABLE;

                // Send this updated data to file
                test_val = SetDataFromTable(&pus11_data, freed_activity.data);
                if (test_val == PUS_SUCCESSFUL)
                {
                    // Get current info before update
                    pus11DataTableInfo_t pus11_table_info = {0};
                    test_val = GetInfoFromTable(&pus11_table_info);
                    if (test_val == PUS_SUCCESSFUL)
                    {
                        // Update data number
                        pus11_table_info.nb_data--;
                        test_val = SetInfoFromTable(&pus11_table_info);
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
                return_value = PUS_ERROR;
            }
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
static pusStatus_t IN_PUS_TEXT_SECTION GetAvailableData(pus11DataIndex_t *data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (data_index != NULL)
    {
        // First get table info
        pus11DataTableInfo_t pus11_table_info = {0};
        pusStatus_t test_val = GetInfoFromTable(&pus11_table_info);
        if (test_val == PUS_SUCCESSFUL)
        {
            // Initialize data variable and current write index
            pus11Data_t pus11_data = {0};
            pus11DataIndex_t current_write_index = pus11_table_info.write_index;

            // Get data at current write index
            test_val = GetDataFromTable(&pus11_data, current_write_index);

            // Find a new slot if current slot is not available
            while ((test_val == PUS_SUCCESSFUL) && (pus11_data.status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE) && (current_write_index != pus11_table_info.write_index))
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
            if (test_val == PUS_SUCCESSFUL)
            {
                // Make sure you haven't gone full circle
                if ((current_write_index == pus11_table_info.write_index) && (pus11_data.status == (pus11DataIndex_t)PUS11_DATA_UNAVAILABLE))
                {
                    return_value = PUS_ERROR;
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
                    if (test_val != PUS_SUCCESSFUL)
                    {
                        return_value = PUS_ERROR;
                    }
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
 * @fn      ResetScheduleAndData(void)
 * @brief   This function reset schedule and data file (filling them with zeros)
 * @retval  #PUS_ERROR if write in FS has encountered an error
 * @retval  #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION ResetScheduleAndData(void)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    kernelStatus_t write_status = KERNEL_SUCCESSFUL;
    data_t zero_filled_data[ZERO_FILLED_DATA_SIZE] = {0};
    length_t origin = 0u;

    // Delete data from pus11 sched file
    // Set read/write pointer to the beginning of the file
    kernelStatus_t test_fs = FsIoctl(PUS11_SCHED_FILE, FS_IOCTL_SEEK, &origin, sizeof(origin));
    if (test_fs == KERNEL_SUCCESSFUL)
    {
        // Write 0s in the file
        length_t remaining_bytes = SCHEDULE_SIZE; // cppcheck-suppress misra-c2012-10.6; False positive, there is no wider type asignment, SCHEDULE_SIZE is uint32_t
        while ((write_status == KERNEL_SUCCESSFUL) && (remaining_bytes > 0u))
        {
            if (remaining_bytes >= ZERO_FILLED_DATA_SIZE)
            {
                write_status = FsWrite(PUS11_SCHED_FILE, (data_t)&zero_filled_data, ZERO_FILLED_DATA_SIZE);
                remaining_bytes -= ZERO_FILLED_DATA_SIZE;
            }
            else
            {
                write_status = FsWrite(PUS11_SCHED_FILE, (data_t)&zero_filled_data, remaining_bytes);
                remaining_bytes = 0u;
            }
        }

        // Delete data from pus11 data file
        // Set read/write pointer to the beginning of the file
        test_fs = FsIoctl(PUS11_DATA_FILE, FS_IOCTL_SEEK, &origin, sizeof(origin));
        if (test_fs == KERNEL_SUCCESSFUL)
        {
            // Write 0s in the file
            remaining_bytes = PUS11_DATA_TABLE_SIZE; // cppcheck-suppress misra-c2012-10.6; False positive, there is no wider type asignment, PUS11_DATA_TABLE_SIZE is uint32_t
            while ((write_status == KERNEL_SUCCESSFUL) && (remaining_bytes > 0u))
            {
                if (remaining_bytes >= ZERO_FILLED_DATA_SIZE)
                {
                    write_status = FsWrite(PUS11_DATA_FILE, (data_t)&zero_filled_data, ZERO_FILLED_DATA_SIZE);
                    remaining_bytes -= ZERO_FILLED_DATA_SIZE;
                }
                else
                {
                    write_status = FsWrite(PUS11_DATA_FILE, (data_t)&zero_filled_data, remaining_bytes);
                    remaining_bytes = 0u;
                }
            }

            // Check if write went well
            if (write_status != KERNEL_SUCCESSFUL)
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
static pusStatus_t IN_PUS_TEXT_SECTION GetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (pus11_table_info != NULL)
    {
        // Move the read/write pointer to the beginning (where the info table is located)
        length_t origin = 0u;
        kernelStatus_t test_fs = FsIoctl(PUS11_DATA_FILE, FS_IOCTL_SEEK, &origin, sizeof(origin));
        if (test_fs == KERNEL_SUCCESSFUL)
        {
            // Then read the info table
            test_fs = FsRead(PUS11_DATA_FILE, (data_t)pus11_table_info, PUS11_DATA_TABLE_INFO_SIZE);
            if (test_fs != KERNEL_SUCCESSFUL)
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
 * @fn          SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
 * @brief       Set PUS11 info from data table
 * @param[in]   pus11_table_info Infos for pus11 table
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION SetInfoFromTable(pus11DataTableInfo_t *pus11_table_info)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (pus11_table_info != NULL)
    {
        // Move the read/write pointer to the beginning (where the info table is located)
        length_t origin = 0u;
        kernelStatus_t test_fs = FsIoctl(PUS11_DATA_FILE, FS_IOCTL_SEEK, &origin, sizeof(origin));
        if (test_fs == KERNEL_SUCCESSFUL)
        {
            // Then write the info table
            test_fs = FsWrite(PUS11_DATA_FILE, (data_t)pus11_table_info, PUS11_DATA_TABLE_INFO_SIZE);
            if (test_fs != KERNEL_SUCCESSFUL)
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
 * @fn          GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
 * @brief       Get PUS11 data from data table
 * @param[out]  pus11_data Data from pus11 table
 * @param[in]   data_index Data index
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION GetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (pus11_data != NULL)
    {
        // Move the read/write pointer to the desired data field
        length_t offset = PUS11_DATA_TABLE_INFO_SIZE + (data_index * PUS11_MAXIMUM_DATA_SIZE); // cppcheck-suppress misra-c2012-10.7; False positive, there is no wider type arithmetic conversion, (data_index * PUS11_MAXIMUM_DATA_SIZE) is a uint32_t
        kernelStatus_t test_fs = FsIoctl(PUS11_DATA_FILE, FS_IOCTL_SEEK, &offset, sizeof(offset));
        if (test_fs == KERNEL_SUCCESSFUL)
        {
            // Then read data in table
            test_fs = FsRead(PUS11_DATA_FILE, (data_t)pus11_data, PUS11_MAXIMUM_DATA_SIZE);
            if (test_fs != KERNEL_SUCCESSFUL)
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
 * @fn          SetDataFromTable(pus11Data_t *pus11_data)
 * @brief       Set PUS11 data from data table
 * @param[in]   pus11_data Data for pus11 table
 * @param[in]   data_index Data index
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if write in FS has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION SetDataFromTable(pus11Data_t *pus11_data, pus11DataIndex_t data_index)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (pus11_data != NULL)
    {
        // Move the read/write pointer to the desired data field
        length_t offset = PUS11_DATA_TABLE_INFO_SIZE + (data_index * PUS11_MAXIMUM_DATA_SIZE); // cppcheck-suppress misra-c2012-10.7; False positive, there is no wider type arithmetic conversion, (data_index * PUS11_MAXIMUM_DATA_SIZE) is a uint32_t
        kernelStatus_t test_fs = FsIoctl(PUS11_DATA_FILE, FS_IOCTL_SEEK, &offset, sizeof(offset));
        if (test_fs == KERNEL_SUCCESSFUL)
        {
            // Then write data in table
            test_fs = FsWrite(PUS11_DATA_FILE, (data_t)pus11_data, PUS11_MAXIMUM_DATA_SIZE);
            if (test_fs != KERNEL_SUCCESSFUL)
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
