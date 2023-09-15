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
#include "conf/pus3_conf.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t SearchHKRefFromHKID(hkId_t hkid, hkRef_t *ref);

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
 * @retval      #PUS_ERROR if HKID does not exist
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + HOUSEKEEPING_ID_SIZE + CRC_TRAILER_SIZE))
        {
            // Get HKID from TC
            hkId_t hkid = ARRAY_TO_UINT32_BIG_ENDIAN(tc->data);
            if (hkid != 0u)
            {
                hkRef_t ref = 0u;
                pusStatus_t test_val = SearchHKRefFromHKID(hkid, &ref);
                if (test_val == PUS_SUCCESSFUL)
                {
                    g_hk_dynamic_conf[ref].hk_status = PUS3_ENABLE;
                }
                else
                {
                    // HKID does not exit
                    return_value = PUS_ERROR;
                    *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
                }
            }
            else
            {
                // Enable all HK
                for (hkRef_t ref = 0u; ref < (hkRef_t)NB_HK; ref++)
                {
                    g_hk_dynamic_conf[ref].hk_status = PUS3_ENABLE;
                }
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

/**
 * @fn          ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send disable HK report by HKID (if HKID = 0 disable all)
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for S1SS8 TM
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if HKID does not exist
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
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

        if ((tc->spp_header.packet_data_length + 1u) == (TC_HEADER_SIZE + HOUSEKEEPING_ID_SIZE + CRC_TRAILER_SIZE))
        {
            // Get HKID from TC
            hkId_t hkid = ARRAY_TO_UINT32_BIG_ENDIAN(tc->data);
            if (hkid != 0u)
            {
                hkRef_t ref = 0u;
                pusStatus_t test_val = SearchHKRefFromHKID(hkid, &ref);
                if (test_val == PUS_SUCCESSFUL)
                {
                    g_hk_dynamic_conf[ref].hk_status = PUS3_DISABLE;
                }
                else
                {
                    // HKID does not exit
                    return_value = PUS_ERROR;
                    *error_code = PUS_EXECUTION_UNEXPECTED_DATA;
                }
            }
            else
            {
                // Disable all HK
                for (hkRef_t ref = 0u; ref < (hkRef_t)NB_HK; ref++)
                {
                    g_hk_dynamic_conf[ref].hk_status = PUS3_DISABLE;
                }
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

/**
 * @fn          IsHKReportAvailable(hkId_t hkid)
 * @brief       Function that says if HK report for this HKID is enable
 * @param[in]   hkid HouseKeeping ID of the HK report
 * @retval      #PUS_INVALID_PARAM if HKID is 0 or does not exist
 * @retval      #PUS_ERROR if HK report is disable for this HKID
 * @retval      #PUS_SUCCESSFUL if HK report is available for this HKID
 */
pusStatus_t IsHKReportAvailable(hkId_t hkid)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    hkRef_t ref = 0u;

    // Function Core
    if (hkid != 0u)
    {
        pusStatus_t test_val = SearchHKRefFromHKID(hkid, &ref);
        if (test_val == PUS_SUCCESSFUL)
        {
            if (g_hk_dynamic_conf[ref].hk_status == PUS3_ENABLE)
            {
                return_value = PUS_SUCCESSFUL;
            }
            else
            {
                return_value = PUS_ERROR;
            }
        }
        else
        {
            // HKID does not exit
            return_value = PUS_INVALID_PARAM;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SearchHKRefFromHKID(hkId_t hkid, hkRef_t *ref)
 * @brief       Function that says if HK report for this HKID is enable
 * @param[in]   hkid HouseKeeping ID of the HK report
 * @param[out]  ref HouseKeeping ID of the HK report
 * @retval      #PUS_ERROR if HKID does not exist
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t SearchHKRefFromHKID(hkId_t hkid, hkRef_t *ref)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_ERROR;
    hkRef_t left = 0u;
    hkRef_t right = (hkRef_t)NB_HK - 1u;
    hkRef_t cursor = left + (right - left) / 2u;

    // Function Core
    while ((left <= right) && (right < (hkRef_t)NB_HK) && (return_value != PUS_SUCCESSFUL))
    {
        if (g_hk_dynamic_conf[cursor].hkid == hkid)
        {
            *ref = g_hk_dynamic_conf[cursor].ref;
            return_value = PUS_SUCCESSFUL;
        }
        else if (g_hk_dynamic_conf[cursor].hkid < hkid)
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