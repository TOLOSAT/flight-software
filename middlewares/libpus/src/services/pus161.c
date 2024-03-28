/**
 * @file    pus17.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 17 functions (Test)
 * @date    12/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "services/pus161.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"
#include "miso/miso.h"


/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteS17SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S17SS2 TM (idle time report)
 * @param[in]   tc S17SS1 TC (this parameter is unused for these service and subservice)
 * @param[out]  tm S17SS2 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[4] = {0};

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Recuperation de l'idle processeur depuis miso
        uint32_t idle = getIdleTime();

        // Set up data
        (void)memcpy((void *)&data, (void *)&idle, 4);
        

        // Build TM 
        return_value = BuildTM(tm, 161u, 2u, (pusData_t *)&data, 4);
        
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS17SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S17SS4 TM (Stack usage report)
 * @param[in]   tc S17SS3 TC (this parameter is unused for these service and subservice)
 * @param[out]  tm S17SS4 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[4] = {0};

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Get stack usage
        uint32_t stack = getStackUsage();

        // Set up data
        (void)memcpy((void *)&data, (void *)&stack, 4);
        
        // Build TM 
        return_value = BuildTM(tm, 161u, 4u, (pusData_t *)&data, 4);
        
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS17SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S17SS6 TM (Stack usage report)
 * @param[in]   tc S17SS5 TC (this parameter is unused for these service and subservice)
 * @param[out]  tm S17SS6 TM that we will send
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 */
pusStatus_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    // Unused Parameters
    (void)(tc);

    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[4] = {0};

    // Function Core
    if ((tm != NULL) && (error_code != NULL))
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Get stack usage
        char *state = getState();

        // Set up data
        (void)memcpy((void *)&data, (void *)&state, 4);
        
        // Build TM 
        return_value = BuildTM(tm, 161u, 6u, (pusData_t *)&data, 4);
        
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;

}
