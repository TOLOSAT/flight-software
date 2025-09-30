/**
 * @file    pus3.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 3 functions (Housekeeping)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "tm_management.h"
#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          BuildS3SS25(pusTM_t *tm, housekeepingReport_t *report)
 * @brief       Function that send S3SS25 TM (housekeeping report)
 * @param[out]  tm TM that will be sent
 * @param[in]   report Housekeeping report
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS3SS25(pusTM_t *tm, housekeepingReport_t *report)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((tm != NULL) && (report != NULL))
    {
        // Build TM
        return_value = BuildTM(tm, 3u, 25u, (pusData_t *)report, HOUSEKEEPING_REPORT_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send enable HK report by HKID (if HKID = 0 enable all)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_INVALID_PARAM if HKID does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused Parameters
    (void)(tm);

    // Check parameter(s)
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + HOUSEKEEPING_ID_SIZE + CRC_TRAILER_SIZE))
        {
            // TO DO : USE HK syscalls when available
        }
        else
        {
            return_value = RET_INVALID_PARAM;
            *error_code  = PUS_EXECUTION_UNEXPECTED_DATA;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send disable HK report by HKID (if HKID = 0 disable all)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_NOT_AVAILABLE if HKID does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused Parameters
    (void)(tm);

    // Check parameter(s)
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + HOUSEKEEPING_ID_SIZE + CRC_TRAILER_SIZE))
        {
            // TO DO : USE HK syscalls when available
        }
        else
        {
            return_value = RET_INVALID_PARAM;
            *error_code  = PUS_EXECUTION_UNEXPECTED_DATA;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
