/**
 * @file    pus1.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for PUS 1 functions (Request verification)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus1.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          BuildS1SS1(const pusTC_t *tc, pusTM_t *acceptance_tm)
 * @brief       Function that send S1SS1 TM (acceptance acknowledgment)
 * @param[in]   tc TC we want to acknowledge
 * @param[out]  acceptance_tm Acceptance TM we will send
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS1SS1(const pusTC_t *tc, pusTM_t *acceptance_tm)
{
    // Variable Initialisation
    returnCode_t return_value       = RET_SUCCESSFUL;
    pusData_t data[S1SS1_DATA_SIZE] = { 0 };
    sppHeader_t spp_header_buffer   = { 0 };

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        // Set up headers
        spp_header_buffer.packet_id               = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_sequence_control);

        // Set up data
        (void)memcpy((void *)&data, (void *)&spp_header_buffer, S1SS1_DATA_SIZE);

        // Build TM
        return_value = BuildTM(acceptance_tm, 1u, 1u, (pusData_t *)&data, S1SS1_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS1SS2(const pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error)
 * @brief       Function that send S1SS2 TM (acceptance non acknowledgment)
 * @param[in]   tc TC we want to non acknowledge
 * @param[out]  acceptance_tm Acceptance TM we will send
 * @param[in]   acceptance_error Error that explain why we non acknowledge
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS1SS2(const pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    pusData_t data[S1SS2_DATA_SIZE];
    sppHeader_t spp_header_buffer = { 0 };

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL) && (acceptance_error != 0u))
    {
        // Set up headers
        spp_header_buffer.packet_id               = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_sequence_control);

        // Set up data
        (void)memcpy((void *)&data, (void *)&spp_header_buffer, S1SS2_DATA_SIZE - 1u);
        data[S1SS2_DATA_SIZE - 1u] = acceptance_error;

        // Build TM
        return_value = BuildTM(acceptance_tm, 1u, 2u, (pusData_t *)&data, S1SS2_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS1SS7(const pusTC_t *tc, pusTM_t *execution_tm)
 * @brief       Function that send S1SS7 TM (execution acknowledgment)
 * @param[in]   tc TC we want to acknowledge
 * @param[out]  execution_tm Execution TM we will send
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS1SS7(const pusTC_t *tc, pusTM_t *execution_tm)
{
    // Variable Initialisation
    returnCode_t return_value       = RET_SUCCESSFUL;
    pusData_t data[S1SS7_DATA_SIZE] = { 0 };
    sppHeader_t spp_header_buffer   = { 0 };

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        // Set up headers
        spp_header_buffer.packet_id               = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_sequence_control);

        // Set up data
        (void)memcpy((void *)&data, (void *)&spp_header_buffer, S1SS7_DATA_SIZE);

        // Build TM
        return_value = BuildTM(execution_tm, 1u, 7u, (pusData_t *)&data, S1SS7_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS1SS8(const pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error)
 * @brief       Function that send S1SS8 TM (execution non acknowledgment)
 * @param[in]   tc TC we want to non acknowledge
 * @param[out]  execution_tm Execution TM we will send
 * @param[in]   execution_error Error that explain why we non acknowledge
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_ERROR if cannot build TM
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildS1SS8(const pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    pusData_t data[S1SS8_DATA_SIZE];
    sppHeader_t spp_header_buffer = { 0 };

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL) && (execution_error != 0u))
    {
        // Set up headers
        spp_header_buffer.packet_id               = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_sequence_control);

        // Set up data
        (void)memcpy((void *)&data, (void *)&spp_header_buffer, S1SS8_DATA_SIZE - 1u);
        data[S1SS8_DATA_SIZE - 1u] = execution_error;

        // Build TM
        return_value = BuildTM(execution_tm, 1u, 8u, (pusData_t *)&data, S1SS8_DATA_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
