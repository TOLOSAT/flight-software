/**
 * @file tc_management.h
 * @author Merlin Kooshmanian
 * @brief Source file for TC management
 * @date 02/07/2023
 *
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "pus_tools/tc_management.h"
#include "pus_tools/crc_computation.h"
#include "pus_tools/endianness_management.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

pusStatus_t CheckCRC(pusTC_t *tc);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              FormatTC(pusTC_t *tc)
 * @brief           Function that format TC the right way
 * @param[in,out]   tc Pointer to the TC we want to format
 * @retval          #PUS_SUCCESSFUL always
 *
 * As we've done a silly memcpy with the uart driver, the
 * TC fields don't have the right endianness, or aren't in
 * the right place.
 */
pusStatus_t FormatTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core

    // Endianness Correction
    tc->spp_header.packet_id = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
    tc->spp_header.packet_sequence_control = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_sequence_control);
    tc->spp_header.packet_data_length = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length);
    tc->tc_header.source_id = HALF_WORD_BYTE_SWAP(tc->tc_header.source_id);

    // Put CRC at the right place
    tc->crc = ARRAY_TO_UINT16_BIG_ENDIAN((tc->data + tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u));
    tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] = 0u;
    tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 2u] = 0u;

    return return_value;
}

/**
 * @fn          CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error)
 * @brief       Function that verifies if TC is valid (right version, type, size)
 * @param[in]   tc Pointer to the TC variable where we want to verify it validity.
 * @param[out]  error Pointer to pass error type to TM(1,2)
 * @retval      #PUS_ERROR if
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    uint16_t packet_id = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
    uint16_t data_size = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length) + 1u;

    // Function Core
    // Check Packet Version Number
    if (((packet_id & PACKET_VERSION_NUMBER_MASK) >> PACKET_VERSION_NUMBER_OFFSET) == VALID_PACKET_VERSION_NUMBER)
    {
        // Check Packet Type
        if (((packet_id & PACKET_TYPE_MASK) >> PACKET_TYPE_OFFSET) == TC_TYPE)
        {
            // Check Secondary Header Presence
            if (((packet_id & HEADER_PRESENCE_MASK) >> HEADER_PRESENCE_OFFSET) == HEADER_PRESENT)
            {
                // Check Size
                if (data_size >= TC_HEADER_SIZE + CRC_TRAILER_SIZE)
                {
                    // Check CRC
                    if(CheckCRC(tc) != PUS_SUCCESSFUL)
                    {
                        return_value = PUS_ERROR;
                        *error = PUS_ACCEPTANCE_INVALID_CRC;
                    }

                }
                else
                {
                    return_value = PUS_ERROR;
                    *error = PUS_ACCEPTANCE_INVALID_FORMAT;
                }
            }
            else
            {
                return_value = PUS_ERROR;
                *error = PUS_ACCEPTANCE_INVALID_FORMAT;
            }
        }
        else
        {
            return_value = PUS_ERROR;
            *error = PUS_ACCEPTANCE_INVALID_FORMAT;
        }
    }
    else
    {
        return_value = PUS_ERROR;
        *error = PUS_ACCEPTANCE_INVALID_FORMAT;
    }

    return return_value;
}

/**
 * @fn              EraseTC(pusTC_t *tc)
 * @brief           Function that erase a TC, it fills it with zeros
 * @param[in,out]   tc Pointer to the TC we want to erase
 * @retval          #PUS_SUCCESSFUL always
 */
pusStatus_t EraseTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    memset(tc, 0u, TC_MAX_SIZE);

    return return_value;
}

/**
 * @fn          CheckCRC(pusTC_t *tc)
 * @brief       Function that verifies a received TC has not been corrupted
 * @param[in]   tc Pointer to the TC variable where we want to check it CRC
 * @retval      #PUS_ERROR if the computed CRC is different than the received CRC
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t CheckCRC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    uint16_t data_size = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length) + 1u;
    pusCRC_t reiceved_crc = ARRAY_TO_UINT16_BIG_ENDIAN((tc->data + data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE));
    pusCRC_t computed_crc = 0u;

    // Function Core
    computed_crc = computeCRC((uint8_t *)tc, data_size + SPP_HEADER_SIZE - CRC_TRAILER_SIZE);
    if (computed_crc != reiceved_crc)
    {
        return_value = PUS_ERROR;
    }

    return return_value;
}