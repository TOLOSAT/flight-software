/**
 * @file    pus3.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 3 functions (Housekeeping)
 * @date    06/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "services/pus3.h"
#include "pus_tools/tm_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          BuildS3SS25(pusTM_t *tm, houseKeepingReport_t *report)
 * @brief       Function that send S3SS25 TM (housekeeping report)
 * @param[out]  tm TM that will be sent
 * @param[in]   report Housekeeping report
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS3SS25(pusTM_t *tm, houseKeepingReport_t *report)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (report != NULL))
    {
        // Build TM
        return_value = BuildTM(tm, 3u, 25u, (pusData_t *)report, HOUSEKEEPING_REPORT_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send enable HK report by HKID (if HKID = 0 enable all)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 *
 * @warning This function has not yet been coded. It will be coded when the HK manager is.
 */
pusStatus_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    *error_code = PUS_EXECUTION_NO_ERROR;

    // Function Core
    if ((tc != NULL) && (tm != NULL))
    {
        /* To Do */
        (void)(tc);
        (void)(tm);
        (void)(error_code);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send disable HK report by HKID (if HKID = 0 disable all)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot execute TC
 * @retval      #PUS_SUCCESSFUL else
 *
 * @warning This function has not yet been coded. It will be coded when the HK manager is.
 */
pusStatus_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    *error_code = PUS_EXECUTION_NO_ERROR;

    // Function Core
    if ((tc != NULL) && (tm != NULL))
    {
        /* To Do */
        (void)(tc);
        (void)(tm);
        (void)(error_code);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
        *error_code = PUS_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IsHKReportAvailable(uint32_t HKID)
 * @brief       Function that says if HK report for this HKID is enable
 * @param[in]   HKID HouseKeeping ID of the HK report
 * @retval      #PUS_INVALID_PARAM if HKID is 0
 * @retval      #PUS_ERROR if HK report is disable for this HKID
 * @retval      #PUS_SUCCESSFUL if HK report is available for this HKID
 *
 * @warning This function has not yet been coded. It will be coded when the HK manager is.
 */
pusStatus_t IsHKReportAvailable(uint32_t HKID)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (HKID == 0u)
    {
        /* To Do */
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}