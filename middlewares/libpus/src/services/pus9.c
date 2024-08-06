/**
 * @file    pus9.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 9 functions (Time management)
 * @date    06/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "pus.h"
#include "services/pus9.h"
#include "endianness.h"
#include "time.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS9SS128(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that receive S9SS128 TC and update OBT
 * @param[in]   tc S9SS128 TC that contains upcoming time
 * @param[out]  tm None (this parameter is unused for these service and subservice)
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION ExecuteS9SS128(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + CUC_TIME_SIZE + CRC_TRAILER_SIZE))
        {
            time_t upcoming_time;
            // Update upcoming_time value with data field
            BIG_ENDIAN_ARRAY_TO_UINT64(tc->data, upcoming_time);
            coreStatus_t set_time_status = SetTime(upcoming_time);
            if (set_time_status != CORE_SUCCESSFUL)
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
    }

    return return_value;
}