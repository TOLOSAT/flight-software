/**
 * @file    pus9.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 9 functions (Time management)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "tm_management.h"
#include "services/pus9.h"

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
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if time cannot be set
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS9SS128(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

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
            returnCode_t set_time_status = SetTime(upcoming_time);
            if (set_time_status != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
                *error_code = PUS_EXECUTION_FAILED;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
            *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}