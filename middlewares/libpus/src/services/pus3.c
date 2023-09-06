/**
 * @file    pus3.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 3 functions
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