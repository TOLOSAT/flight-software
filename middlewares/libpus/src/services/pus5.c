/**
 * @file    pus5.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 5 functions (Event reporting)
 * @date    06/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */


/******************************* Include Files *******************************/

#include "services/pus5.h"
#include "pus_tools/tm_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          BuildS5SS1234(pusTM_t *tm, pusEventSeverity_t severity, eventReport_t *report)
 * @brief       Function that send S5SS1, S5SS2, S5SS3 or S5SS4 TM (event report)
 * @param[out]  tm TM that will be sent
 * @param[in]   severity Severity of the event
 * @param[in]   report Event report
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL or severity is not 1,2,3 or 4
 * @retval      #PUS_ERROR if cannot build TM
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS5SS1234(pusTM_t *tm, pusEventSeverity_t severity, eventReport_t *report)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (report != NULL) && (severity <= PUS5_HIGH_SEVERITY_EVENT))
    {
        // Build TM
        return_value = BuildTM(tm, 5u, severity, (pusData_t *)report, EVENT_REPORT_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}