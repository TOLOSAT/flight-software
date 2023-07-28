/**
 * @file pus17.c
 * @author Merlin Kooshmanian
 * @brief Source file for PUS 17 functions
 * @date 12/07/2023
 *
 * Last Update : 12/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "services/pus17.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS17SS1(pusTC_t *tc, pusTM_t *tm)
 * @brief       Function that send S17SS2 TM (connexion report)
 * @param[in]   tc S17SS1 TC (this parameter is unused for these service and subservice)
 * @param[out]  tm S17SS2 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecuteS17SS1(pusTC_t *tc, pusTM_t *tm)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tm != NULL)
    {
        BuildTM(tm, 17u, 2u, NULL, 0);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}