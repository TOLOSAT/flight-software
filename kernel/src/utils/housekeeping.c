/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for housekeeping
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/housekeeping.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t SearchHKID(hkId_t hkid, uint32_t *lineno);

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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (hkid != HKID_ALL)
    {
        // Enable a specific HK
        uint32_t lineno = 0u;
        return_value = SearchHKID(hkid, &lineno);
        if (RET_SUCCESSFUL)
        {
            g_hk_desc_table[lineno].hk_status = HK_ENABLE;
        }
    }
    else
    {
        // Enable all HKs
        for (uint32_t lineno = 0u; lineno < NB_HK; lineno++)
        {
            g_hk_desc_table[lineno].hk_status = HK_ENABLE;
        }
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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (hkid != HKID_ALL)
    {
        // Disable a specific HK
        uint32_t lineno = 0u;
        return_value = SearchHKID(hkid, &lineno);
        if (RET_SUCCESSFUL)
        {
            g_hk_desc_table[lineno].hk_status = HK_DISABLE;
        }
    }
    else
    {
        // Disable all HKs
        for (uint32_t lineno = 0u; lineno < NB_HK; lineno++)
        {
            g_hk_desc_table[lineno].hk_status = HK_DISABLE;
        }
    }

    return return_value;
}

/**
 * @fn          EmitHK(hk_t *hk)
 * @brief       Function that emits a housekeeping
 * @param[in]   hk  Housekeeping that will be emited
 * @retval      #RET_INVALID_PARAM if the hk is a null pointer
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t EmitHK(hk_t *hk)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (hk != NULL)
    {
        // TO DO : write the function
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          CollectHKs(void)
 * @brief       Function that collects all the available houskeeping
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t CollectHKs(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    // TO DO : write the function

    return return_value;
}

/**
 * @fn          SearchHKID(hkId_t hkid, uint32_t *lineno)
 * @brief       Function that looks for an HKID in the HK desc table
 * @param[in]   hkid HouseKeeping ID
 * @param[out]  lineno Line in the g_hk_desc_table
 * @retval      #RET_NOT_AVAILABLE if HKID does not exist
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SearchHKID(hkId_t hkid, uint32_t *lineno)
{
    // Variable Initialisation
    returnCode_t return_value = RET_NOT_AVAILABLE;
    uint32_t left = 0u;
    uint32_t right = NB_HK - 1u;
    uint32_t cursor = left + (right - left) / 2u;

    // Function Core
    while ((left <= right) && (right < NB_HK) && (return_value != RET_SUCCESSFUL))
    {
        if (g_hk_desc_table[cursor].hkid == hkid)
        {
            *lineno = cursor;
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