/**
 * @file    pus6.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 6 functions (Memory management)
 * @date    08/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "services/pus6.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"
#include "tolosat_fs.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump);

/*************************** Variables Definitions ***************************/

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
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
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

        // Write data into FS
        fsStatus_t test_fs = FsWrite(load_data.base, load_data.offset, load_data.data, load_data.length);
        if (test_fs != FS_SUCCESSFUL)
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
 * @fn          ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that dump data from memory
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
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

        // Read data from FS
        fsStatus_t test_fs = FsRead(requested_data.base, requested_data.offset, dumped_data.data, requested_data.length);
        if (test_fs == FS_SUCCESSFUL)
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
        return_value = PUS_INVALID_PARAM;
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
static pusStatus_t IN_PUS_TEXT_SECTION BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump)
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