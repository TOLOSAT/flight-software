/**
 * @file    pus161.c
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Source file for PUS 161 functions (MISO)
 * @date    08/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "miso/pus161.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static pus161Data_t *pus161_data_pointer;

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitS161(pus161Data_t *pus161_data)
 * @brief       Function that initialises PUS 161 with shared data struct
 * @param[in]   pus161_data shared data struct
 * @retval      #PUS_SUCCESSFUL always
 */
pusStatus_t InitS161(pus161Data_t *pus161_data)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    pus161_data_pointer = pus161_data;
    memset(pus161_data_pointer, 0u, sizeof(pus161Data_t));

    return return_value;
}

/**
 * @fn          ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS2 TM (idle time report)
 * @param[in]   tc S161SS1 TC that requests this TM
 * @param[out]  tm S161SS2 TM that we will send
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
        uint32_t idle = pus161_data_pointer->idle_time;

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
 * @fn          ExecuteS161SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS4 TM (Stack usage report)
 * @param[in]   tc S161SS3 TC that requests this TM
 * @param[out]  tm S161SS4 TM that we will send
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
        uint32_t stack = pus161_data_pointer->max_stack_usage;

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
 * @fn          ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS6 TM (Stack usage report)
 * @param[in]   tc S161SS5 TC that requests this TM
 * @param[out]  tm S161SS6 TM that we will send
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
        // To do

        // Set up data
        // To do
        
        // Build TM 
        return_value = BuildTM(tm, 161u, 6u, (pusData_t *)&data, 4);
        
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;

}
