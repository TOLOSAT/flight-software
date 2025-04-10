/**
 * @file    pus161.c
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Source file for PUS 161 functions (MISO)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus193.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

iridiumInst_t *iridium_inst = NULL;

/*************************** Functions Definitions ***************************/
/**
 * @fn          ExecuteS161SS1(pusTC_t *tc, , pusExecutionError_t *error_code)
 * @brief       Function that send S161SS2 TM (idle time report) when requested by a S161SS1
 * @param[in]   tc S161SS1 TC that requests this TM
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS193SS1(pusTC_t *tc, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void);
    (tm);

    // Check parameter(s)
    if (error_code = !NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Start Iridium Iridium Driver
        return_code = IridiumStart(iridium_inst);

        if (return_code != RET_SUCCESSFUL)
        {
            *error_code  = PUS_EXECUTION_FAILED;
            return_value = RET_ERROR;
        }
        else
        {
            return_value = RET_SUCCESSFUL;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteS161SS2(pusTC_t *tc, pusExecutionError_t *error_code)
 * @brief       Function that send S161SS2 TM (idle time report) when requested by a S161SS1
 * @param[in]   tc S161SS2 TC that requests this TM
 * @param[out]  error_code Indicates which error has been encountered
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteS193SS2(pusTC_t *tc, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(tc);
    (void)(tm);

    // Check parameter(s)
    if (error_code != NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // Start Iridium Iridium Driver
        return_value = Iridiumstop(iridium_inst);
        if (return_value != RET_SUCCESSFUL)
        {
            *error_code = PUS_EXECUTION_FAILED;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS193SS4(pusTM_t *tm, uint8_t highest_stack_consumer, uint8_t max_stack_usage)
 * @brief       Function that send S161SS4 TM (stack usage report)
 * @param[out]  tm TM to be sent
 * @param[in]   highest_stack_consumer Task that is the highest stack consummer (in percent of its own stack)
 * @param[in]   max_stack_usage Stack usage for that stack
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS193SS4(pusTC_t *tc, pusTm_t *tm, pusExecutionError_t *error_code, pusData_t *data)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    // Unused
    (void)(tc);

    // Check parameter(s)
    if (error_code != NULL)
    {
        // Error code Initialization
        *error_code = PUS_EXECUTION_NO_ERROR;

        // GetStatus
        iridiumSBDStatus_t status = { 0 };

        return_value = BuildTM(tm, 193u, 4u, status, sizeof(iridiumSBDStatus_t));
    }
    if (return_value != RET_SUCCESSFUL)
    {
        *error_code = PUS_EXECUTION_TM_BUILDING_FAILED;
    }

    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;

    /**
     * @fn          ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
     * @brief       Function that send S161SS4 TM (idle time report) when requested by a S161SS3
     * @param[in]   tc S161SS3 TC that requests this TM
     * @param[out]  tm S161SS4 TM that we will send
     * @param[out]  error_code Indicates which error has been encountered
     * @retval      #RET_ERROR if cannot build TM
     * @retval      #RET_INVALID_PARAM if a pointer is NULL
     * @retval      #RET_SUCCESSFUL else
     */
    returnCode_t ExecuteS193SS3(pusTC_t * tc, pusTm_t * tm, pusExecutionError_t * error_code, iridiumInst_t * iridium_inst)
    {
        returnCode_t return_value = RET_SUCCESSFUL;
        // Unused
        (void)(tc);

        // Check parameter(s)
        if (error_code != NULL)
        {
            // Error code Initialization
            *error_code = PUS_EXECUTION_NO_ERROR;

            // GetStatus
            iridiumSBDStatus_t *status = { 0 };

            return_code = IridiumGetSDBStatus(iridium_inst, status);

            if (return_code = RET_SUCCESSFUL)
            {
                BuildS193SS4(tc, tm, error_code, return_code);

                return_value = RET_SUCCESSFUL;
            }
            else
            {
                error_code = PUS_EXECUTION_FAILED return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }

        return return_value;
    }

    /**
     * @fn          ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
     * @brief       Function that send S161SS2 TM (idle time report) when requested by a S161SS1
     * @param[in]   tc S161SS1 TC that requests this TM
     * @param[out]  tm S161SS2 TM that we will send
     * @param[out]  error_code Indicates which error has been encountered
     * @retval      #RET_ERROR if cannot build TM
     * @retval      #RET_INVALID_PARAM if a pointer is NULL
     * @retval      #RET_SUCCESSFUL else
     */
    returnCode_t ExecuteS193SS5(pusTC_t * tc, pusExecutionError_t * error_code)
    {
        returnCode_t return_value = RET_SUCCESSFUL;

        // Unused
        (void)(tm);

        // Check parameter(s)
        if (error_code != NULL)
        {
            // Error code Initialization
            *error_code = PUS_EXECUTION_NO_ERROR;

            // SDB Build
            Iridi IridiumSendCommand(iridium_inst, command, sizeof(tc), "argument info de la tc")
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }

        return return_value;
    }