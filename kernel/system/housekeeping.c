/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for housekeeping
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/housekeeping.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          EnableHK(hkId_t hkid)
 * @brief       Function that enables the HK collection for a certain HKID
 * @param[in]   hkid Housekeeping ID
 * @retval      #RET_NOT_AVAILABLE if the hkid does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t EnableHK(hkId_t hkid)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (hkid != NO_HK)
    {
        // TO DO
        (void)(hkid);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          DisableHK(hkId_t hkid)
 * @brief       Function that disables the HK collection for a certain HKID
 * @param[in]   hkid Housekeeping ID
 * @retval      #RET_NOT_AVAILABLE if the hkid does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DisableHK(hkId_t hkid)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (hkid != NO_HK)
    {
        // TO DO
        (void)(hkid);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetLastHK(hkId_t hkid, hk_t *last_hk)
 * @brief       Function that gets the last hk value for an HKID
 * @param[in]   hkid    Housekeeping ID
 * @param[out]  last_hk Last housekeeping value
 * @retval      #RET_INVALID_PARAM if the hk is a null pointer
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t GetLastHK(hkId_t hkid, hk_t *last_hk)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((last_hk != NULL) && (hkid != NO_HK) && (hkid != ALL_HKS))
    {
        // TO DO
        (void)(hkid);
        (void)(last_hk);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
