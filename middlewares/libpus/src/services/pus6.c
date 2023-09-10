/**
 * @file    pus6.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 6 functions (Memory management)
 * @date    08/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "services/pus6.h"
#include "conf/pus6_conf.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"
#include "tolosat_fs_types.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static FIL g_pus6_buffer_file = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that load data to memory
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    *error_code = PUS_EXECUTION_NO_ERROR;
    pusTCLoadDataField_t load_data = {0};
    FRESULT test_fs;

    // Function Core
    if ((tc != NULL) && (tm != NULL))
    {
        // First get data from TC
        (void)memcpy((void *)&load_data, (void *)tc->data, TC_MAX_DATA_SIZE);
        // Swip Endianness
        load_data.offset = WORD_BYTE_SWAP(load_data.offset);
        load_data.length = WORD_BYTE_SWAP(load_data.length);

        // Check if memory id and memory file exists
        if ((load_data.memory_id < (uint8_t)NB_MEMORY_DEVICES) && (load_data.base < MAX_NB_FILES_PER_DEVICES))
        {
            // Open requested file
            test_fs = f_open(&g_pus6_buffer_file, g_files_static_conf[load_data.memory_id][load_data.base].name, g_files_static_conf[load_data.memory_id][load_data.base].access_mode);
            if (test_fs == FR_OK)
            {
                // Places the write pointer in the right place
                test_fs = f_lseek(&g_pus6_buffer_file, load_data.offset);
                if (test_fs == FR_OK)
                {
                    // Copy data onto file
                    uint32_t bytes_written = 0u;
                    test_fs = f_write(&g_pus6_buffer_file, load_data.data, load_data.length, (UINT *)&bytes_written);
                    if ((test_fs == FR_OK) && (bytes_written == load_data.length))
                    {
                        // Close data
                        test_fs = f_close(&g_pus6_buffer_file);
                        if (test_fs != FR_OK)
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
            return_value = PUS_INVALID_PARAM;
            *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that dump data from memory
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    *error_code = PUS_EXECUTION_NO_ERROR;
    pusTCDumpDataField_t requested_data = {0};
    pusTMDumpDataField_t dumped_data = {0};
    FRESULT test_fs;

    // Function Core
    if ((tc != NULL) && (tm != NULL))
    {
        // First get data from TC
        (void)memcpy((void *)&requested_data, (void *)tc->data, MEMORY_TC_DATA_DUMP_SIZE);
        // Swip Endianness
        requested_data.offset = WORD_BYTE_SWAP(requested_data.offset);
        requested_data.length = WORD_BYTE_SWAP(requested_data.length);

        // Check if memory id and memory file exists
        if ((requested_data.memory_id < (uint8_t)NB_MEMORY_DEVICES) && (requested_data.base < MAX_NB_FILES_PER_DEVICES))
        {

            // Open requested file
            test_fs = f_open(&g_pus6_buffer_file, g_files_static_conf[requested_data.memory_id][requested_data.base].name, g_files_static_conf[requested_data.memory_id][requested_data.base].access_mode);
            if (test_fs == FR_OK)
            {
                // Places the read pointer in the right place
                test_fs = f_lseek(&g_pus6_buffer_file, requested_data.offset);
                if (test_fs == FR_OK)
                {
                    // Copy data from file
                    uint32_t bytes_read = 0u;
                    test_fs = f_read(&g_pus6_buffer_file, dumped_data.data, requested_data.length, (UINT *)&bytes_read);
                    if ((test_fs == FR_OK) && (bytes_read == requested_data.length))
                    {
                        // Close data
                        test_fs = f_close(&g_pus6_buffer_file);
                        if (test_fs == FR_OK)
                        {
                            // Update data an build TM
                            dumped_data.memory_id = requested_data.memory_id;
                            dumped_data.base = requested_data.base;
                            dumped_data.offset = requested_data.offset;
                            dumped_data.length = requested_data.length;
                            pusStatus_t test_build = BuildS6SS4(tm, &dumped_data);
                            if (test_build != PUS_SUCCESSFUL)
                            {
                                return_value = PUS_ERROR;
                                *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
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
                *error_code = PUS_EXECUTION_FAILED;
            }
        }
        else
        {
            return_value = PUS_INVALID_PARAM;
            *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump)
 * @brief       Function that send S6SS4 TM (Memory Dump)
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

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
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}