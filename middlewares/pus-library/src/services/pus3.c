/**
 * @file    pus3.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 3 functions (Housekeeping)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "tm_management.h"
#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t SearchHKRefFromHKID(hkId_t hkid, hkRef_t *ref);

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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
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
    // Unused Parameters
    (void)(tm);

    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + HOUSEKEEPING_ID_SIZE + CRC_TRAILER_SIZE))
        {
            // Get HKID from TC
            hkId_t hkid = 0u;
            BIG_ENDIAN_ARRAY_TO_UINT32(tc->data, hkid);
            if (hkid != 0u)
            {
                hkRef_t ref = 0u;
                returnCode_t test_val = SearchHKRefFromHKID(hkid, &ref);
                if (test_val == RET_SUCCESSFUL)
                {
                    g_hk_desc_table[ref].hk_status = HK_ENABLE;
                }
                else
                {
                    // HKID does not exit
                    return_value = RET_INVALID_PARAM;
                    *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
                }
            }
            else
            {
                // Enable all HK
                for (hkRef_t ref = 0u; ref < (hkRef_t)NB_HK; ref++)
                {
                    g_hk_desc_table[ref].hk_status = HK_ENABLE;
                }
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

/**
 * @fn          ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send disable HK report by HKID (if HKID = 0 disable all)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_INVALID_PARAM if HKID does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + HOUSEKEEPING_ID_SIZE + CRC_TRAILER_SIZE))
        {
            // Get HKID from TC
            hkId_t hkid = 0u;
            BIG_ENDIAN_ARRAY_TO_UINT32(tc->data, hkid);
            if (hkid != 0u)
            {
                hkRef_t ref = 0u;
                returnCode_t test_val = SearchHKRefFromHKID(hkid, &ref);
                if (test_val == RET_SUCCESSFUL)
                {
                    g_hk_desc_table[ref].hk_status = HK_DISABLE;
                }
                else
                {
                    // HKID does not exit
                    return_value = RET_INVALID_PARAM;
                    *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
                }
            }
            else
            {
                // Disable all HK
                for (hkRef_t ref = 0u; ref < (hkRef_t)NB_HK; ref++)
                {
                    g_hk_desc_table[ref].hk_status = HK_DISABLE;
                }
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

/**
 * @fn          IsHKReportAvailable(hkId_t hkid)
 * @brief       Function that says if HK report for this HKID is enable
 * @param[in]   hkid HouseKeeping ID of the HK report
 * @retval      #RET_INVALID_PARAM if HKID is 0 or does not exist
 * @retval      #RET_ERROR if HK report is disable for this HKID
 * @retval      #RET_SUCCESSFUL if HK report is available for this HKID
 */
returnCode_t IsHKReportAvailable(hkId_t hkid)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    hkRef_t ref = 0u;

    // Function Core
    if (hkid != 0u)
    {
        returnCode_t test_val = SearchHKRefFromHKID(hkid, &ref);
        if (test_val == RET_SUCCESSFUL)
        {
            if (g_hk_desc_table[ref].hk_status == HK_ENABLE)
            {
                return_value = RET_SUCCESSFUL;
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            // HKID does not exit
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SearchHKRefFromHKID(hkId_t hkid, hkRef_t *ref)
 * @brief       Function that says if HK report for this HKID is enable
 * @param[in]   hkid HouseKeeping ID of the HK report
 * @param[out]  ref HouseKeeping ID of the HK report
 * @retval      #RET_ERROR if HKID does not exist
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SearchHKRefFromHKID(hkId_t hkid, hkRef_t *ref)
{
    // Variable Initialisation
    returnCode_t return_value = RET_ERROR;
    hkRef_t left = 0u;
    hkRef_t right = (hkRef_t)NB_HK - 1u;
    hkRef_t cursor = left + (right - left) / 2u;

    // Function Core
    while ((left <= right) && (right < (hkRef_t)NB_HK) && (return_value != RET_SUCCESSFUL))
    {
        if (g_hk_desc_table[cursor].hkid == hkid)
        {
            *ref = g_hk_desc_table[cursor].ref;
            return_value = RET_SUCCESSFUL;
        }
        else if (g_hk_desc_table[cursor].hkid < hkid)
        {
            left = cursor + 1u;
            cursor = left + (right - left) / 2u;
        }
        else
        {
            right = cursor - 1u;
            cursor = left + (right - left) / 2u;
        }
    }

    return return_value;
}