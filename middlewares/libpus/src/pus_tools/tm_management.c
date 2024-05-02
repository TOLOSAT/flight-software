/**
 * @file    tm_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for TM management
 * @date    06/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "pus_tools/tm_management.h"
#include "pus_tools/crc_computation.h"
#include "pus_tools/endianness_management.h"
#include "time_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var g_tm_counter
 * @brief Global Variable that is used for tm numbering
 */
uint16_t g_tm_counter = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @fn          BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size)
 * @brief       Function that build a TM.
 * @param[out]  tm Pointer to the TM we want to create
 * @param[in]   service PUS Service of TM.
 * @param[in]   subservice PUS Subservice of TM.
 * @param[in]   data Data Packet.
 * @param[in]   data_size Size of data packet.
 * @retval      #PUS_INVALID_PARAM if tm is null pointer or service or subservice equal to 0
 * @retval      #PUS_ERROR if cannot fill time field
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tm != NULL) && (service > 0u) && (subservice > 0u))
    {
        // Build SPP Header
        tm->spp_header.packet_id = (PACKET_VERSION_NUMBER_MASK & ((uint16_t)VALID_PACKET_VERSION_NUMBER << PACKET_VERSION_NUMBER_OFFSET)) | // cppcheck-suppress [badBitmaskCheck,unmatchedSuppression]; Clearer even if it uses an unnecessary bitmask
                                   (PACKET_TYPE_MASK & ((uint16_t)TM_TYPE << PACKET_TYPE_OFFSET)) |                                         // cppcheck-suppress [badBitmaskCheck,unmatchedSuppression]; Clearer even if it uses an unnecessary bitmask
                                   (HEADER_PRESENCE_MASK & ((uint16_t)HEADER_PRESENT << HEADER_PRESENCE_OFFSET)) |
                                   (APID_MASK & OBC_APID);
        tm->spp_header.packet_sequence_control = 0xc000u + (0x3ffffu & g_tm_counter);
        g_tm_counter++;
        tm->spp_header.packet_data_length = TM_HEADER_SIZE + data_size + CRC_TRAILER_SIZE - 1u;

        // Build TM Header
        tm->tm_header.version_timeref = (PUS_VERSION_NUMBER_MASK & (VALID_PUS_VERSION_NUMBER << PUS_VERSION_NUMBER_OFFSET));
        tm->tm_header.service = service;
        tm->tm_header.subservice = subservice;
        tm->tm_header.message_counter = 0u;
        tm->tm_header.destination_id = 0u;

        // Build Data
        if (data_size > 0u)
        {
            (void)memcpy(tm->data, data, data_size);
        }

        // Timestamp TM
        timeStatus_t test_time = GetCUCTime(&tm->tm_header.time);
        if (test_time != TIME_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              FormatTM(pusTM_t *tm)
 * @brief           Function that format tm the right way
 * @param[in,out]   tm Pointer to the TM we want to format
 * @retval          #PUS_INVALID_PARAM if tm is null pointer
 * @retval          #PUS_SUCCESSFUL else
 *
 * As we work we little endian processors, but the TM and TM are big endian
 * formated, we need to swap to big endian before sending the TM.
 *
 * @warning This function wont format TM data field, it has to be format before.
 */
pusStatus_t IN_PUS_TEXT_SECTION FormatTM(pusTM_t *tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tm != NULL)
    {
        uint16_t data_size = tm->spp_header.packet_data_length + 1u;

        // Endianness Correction
        tm->spp_header.packet_id = HALF_WORD_BYTE_SWAP(tm->spp_header.packet_id);
        tm->spp_header.packet_sequence_control = HALF_WORD_BYTE_SWAP(tm->spp_header.packet_sequence_control);
        tm->spp_header.packet_data_length = HALF_WORD_BYTE_SWAP(tm->spp_header.packet_data_length);
        tm->tm_header.message_counter = HALF_WORD_BYTE_SWAP(tm->tm_header.message_counter);
        tm->tm_header.destination_id = HALF_WORD_BYTE_SWAP(tm->tm_header.destination_id);

        // Put CRC at the right place
        tm->crc = computeCRC((uint8_t *)tm, data_size + SPP_HEADER_SIZE - CRC_TRAILER_SIZE);
        tm->data[data_size - TM_HEADER_SIZE - CRC_TRAILER_SIZE] = (pusData_t)((0xff00u & tm->crc) >> 8u);
        tm->data[data_size - TM_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] = (pusData_t)(0x00ffu & tm->crc);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              EraseTM(pusTM_t *tm)
 * @brief           Function that erase a TM, it fills it with zeros
 * @param[in,out]   tm Pointer to the TM we want to erase
 * @return          Nothing
 */
void IN_PUS_TEXT_SECTION EraseTM(pusTM_t *tm)
{
    // Function Core
    (void)memset(tm, 0u, TM_MAX_SIZE);
}