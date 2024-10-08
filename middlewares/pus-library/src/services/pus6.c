/**
 * @file    pus6.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 6 functions (Memory management)
 * @date    08/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "tm_management.h"
#include "services/pus6.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that load data to memory
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if writting in the file does not work (cannot open / write file)
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    pusTCLoadDataField_t load_data = {0};

    // Function Core
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // First get data from TC
        (void)memcpy((void *)&load_data, (void *)tc->data, TC_MAX_DATA_SIZE);
        // Swip Endianness
        load_data.offset = WORD_BYTE_SWAP(load_data.offset);
        load_data.length = WORD_BYTE_SWAP(load_data.length);

        // First open a device for this file
        deviceNo_t temp_dev_pus6 = 0u;
        returnCode_t test_fs = DeviceOpen(&temp_dev_pus6, DEVICE_TYPE_FILE, load_data.base, DEVICE_NO_EXTRA_INFO);
        if (test_fs == RET_SUCCESSFUL)
        {
            // Move read/write pointer
            test_fs = DeviceIoctl(temp_dev_pus6, FS_IOCTL_SEEK, &load_data.offset, sizeof(load_data.offset));
            if (test_fs == RET_SUCCESSFUL)
            {
                // Write data into FS
                test_fs = DeviceWrite(temp_dev_pus6, load_data.data, load_data.length);
                if (test_fs != RET_SUCCESSFUL)
                {
                    return_value = RET_ERROR;
                    *error_code = PUS_EXECUTION_FAILED;
                }
            }
            else
            {
                return_value = RET_ERROR;
                *error_code = PUS_EXECUTION_FAILED;
            }

            // Then close the device anyway (to avoid blocking the ressource)
            (void)DeviceClose(temp_dev_pus6);
        }
        else
        {
            return_value = RET_ERROR;
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that dump data from memory
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if reading the file does not work (cannot open / read file)
 * @retval      #RET_ERROR if cannot build the TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    pusTCDumpDataField_t requested_data = {0};
    pusTMDumpDataField_t dumped_data = {0};

    // Function Core
    if ((tc != NULL) && (tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // First get data from TC
        (void)memcpy((void *)&requested_data, (void *)tc->data, MEMORY_TC_DATA_DUMP_SIZE);
        // Swip Endianness
        requested_data.offset = WORD_BYTE_SWAP(requested_data.offset);
        requested_data.length = WORD_BYTE_SWAP(requested_data.length);

        // First open a device for this file
        deviceNo_t temp_dev_pus6 = 0u;
        returnCode_t test_fs = DeviceOpen(&temp_dev_pus6, DEVICE_TYPE_FILE, requested_data.base, DEVICE_NO_EXTRA_INFO);
        if (test_fs == RET_SUCCESSFUL)
        {
            // Move read/write pointer
            test_fs = DeviceIoctl(temp_dev_pus6, FS_IOCTL_SEEK, &requested_data.offset, sizeof(requested_data.offset));
            if (test_fs == RET_SUCCESSFUL)
            {
                // Read data from FS
                test_fs = DeviceRead(temp_dev_pus6, dumped_data.data, requested_data.length);
                if (test_fs == RET_SUCCESSFUL)
                {
                    // Update data an build TM
                    dumped_data.memory_id = requested_data.memory_id;
                    dumped_data.base = requested_data.base;
                    dumped_data.offset = requested_data.offset;
                    dumped_data.length = requested_data.length;
                    returnCode_t test_build = BuildS6SS4(tm, &dumped_data);
                    if (test_build != RET_SUCCESSFUL)
                    {
                        return_value = RET_ERROR;
                        *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
                    }
                }
                else
                {
                    return_value = RET_ERROR;
                    *error_code = PUS_EXECUTION_FAILED;
                }
            }
            else
            {
                return_value = RET_ERROR;
                *error_code = PUS_EXECUTION_FAILED;
            }

            // Then close the device anyway (to avoid blocking the ressource)
            (void)DeviceClose(temp_dev_pus6);
        }
        else
        {
            return_value = RET_ERROR;
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump)
 * @brief       Function that send S6SS4 TM (Memory Dump)
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (memory_dump != NULL))
    {
        // Compute size
        uint16_t data_size = MEMORY_ID_SIZE + MEMORY_BASE_SIZE + MEMORY_OFFSET_SIZE + MEMORY_LENGTH_SIZE + memory_dump->length;

        // Swip Endianness
        memory_dump->offset = WORD_BYTE_SWAP(memory_dump->offset);
        memory_dump->length = WORD_BYTE_SWAP(memory_dump->length);

        // Build TM
        return_value = BuildTM(tm, 6u, 4u, (pusData_t *)memory_dump, data_size);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}