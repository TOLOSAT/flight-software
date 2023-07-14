/**
 * @file pus1.c
 * @author Merlin Kooshmanian
 * @brief Source file for PUS 1 functions
 * @date 06/07/2023
 *
 * Last Update : 06/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "services/pus1.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/endianness_management.h"

/***************************** Macros Definitions ****************************/

#define S1SS1_DATA_SIZE 4u /**< Size of PUS S1SS1 data field */
#define S1SS2_DATA_SIZE 5u /**< Size of PUS S1SS2 data field */
#define S1SS7_DATA_SIZE 4u /**< Size of PUS S1SS7 data field */
#define S1SS8_DATA_SIZE 5u /**< Size of PUS S1SS8 data field */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          BuildS1SS1(pusTC_t *tc, pusTM_t *acceptance_tm)
 * @brief       Function that send S1SS1 TM (acceptance acknowledgment)
 * @param[in]   tc TC we want to acknowledge
 * @param[out]  acceptance_tm Acceptance TM we will send 
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS1SS1(pusTC_t *tc, pusTM_t *acceptance_tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[S1SS1_DATA_SIZE] = {0};
    sppHeader_t spp_header_buffer;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        spp_header_buffer = tc->spp_header;
        spp_header_buffer.packet_id = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_sequence_control);
        memcpy(&data, &spp_header_buffer, S1SS1_DATA_SIZE);
        BuildTM(acceptance_tm, 1u, 1u, (pusData_t *)&data, S1SS1_DATA_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS1SS2(pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error)
 * @brief       Function that send S1SS2 TM (acceptance non acknowledgment)
 * @param[in]   tc TC we want to non acknowledge
 * @param[out]  acceptance_tm Acceptance TM we will send 
 * @param[in]   acceptance_error Error that explain why we non acknowledge
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS1SS2(pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[S1SS2_DATA_SIZE];
    sppHeader_t spp_header_buffer;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL) && (acceptance_error != 0))
    {
        spp_header_buffer = tc->spp_header;
        spp_header_buffer.packet_id = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_sequence_control);
        memcpy(&data, &spp_header_buffer, S1SS2_DATA_SIZE - 1u);
        data[S1SS2_DATA_SIZE - 1u] = acceptance_error;
        BuildTM(acceptance_tm, 1u, 2u, (pusData_t *)&data, S1SS2_DATA_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS1SS7(pusTC_t *tc, pusTM_t *execution_tm)
 * @brief       Function that send S1SS7 TM (execution acknowledgment)
 * @param[in]   tc TC we want to acknowledge
 * @param[out]  execution_tm Execution TM we will send 
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS1SS7(pusTC_t *tc, pusTM_t *execution_tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[S1SS7_DATA_SIZE] = {0};
    sppHeader_t spp_header_buffer;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        spp_header_buffer = tc->spp_header;
        spp_header_buffer.packet_id = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_sequence_control);
        memcpy(&data, &spp_header_buffer, S1SS7_DATA_SIZE);
        BuildTM(execution_tm, 1u, 7u, (pusData_t *)&data, S1SS7_DATA_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildS1SS8(pusTC_t *tc, pusTM_t *execution_tm, pusAcceptanceError_t acceptance_error)
 * @brief       Function that send S1SS8 TM (acceptance non execution)
 * @param[in]   tc TC we want to non acknowledge
 * @param[out]  execution_tm Execution TM we will send 
 * @param[in]   acceptance_error Error that explain why we non acknowledge
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t BuildS1SS8(pusTC_t *tc, pusTM_t *execution_tm, pusAcceptanceError_t acceptance_error)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusData_t data[S1SS8_DATA_SIZE];
    sppHeader_t spp_header_buffer;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL) && (acceptance_error != 0))
    {
        spp_header_buffer = tc->spp_header;
        spp_header_buffer.packet_id = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_id);
        spp_header_buffer.packet_sequence_control = HALF_WORD_BYTE_SWAP(spp_header_buffer.packet_sequence_control);
        memcpy(&data, &spp_header_buffer, S1SS8_DATA_SIZE - 1u);
        data[S1SS8_DATA_SIZE - 1u] = acceptance_error;
        BuildTM(execution_tm, 1u, 8u, (pusData_t *)&data, S1SS8_DATA_SIZE);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}
