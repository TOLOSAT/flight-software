/**
 * @file    tm_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for TM management
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "tools/crc_computation.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitTMSendContext(pusSendContext_t *send_context)
 * @brief       Function that initialise the send context for TM sending
 * @param[in]   send_context Execution context for the task dealing with TM sending
 * @retval      #RET_INVALID_PARAM if a pointer is a null pointer or send table size is null
 * @retval      #RET_ERROR if initialisation failed because of device binding
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t InitTMSendContext(pusSendContext_t *send_context)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    returnCode_t device_status;

    // Check parameter(s)
    if ((send_context != NULL) && (send_context->send_table != NULL) && (send_context->send_table_size != 0u) && (send_context->tm != NULL))
    {
        // First initiliase the TX device
        device_status = DeviceOpen(&send_context->dev_tx, send_context->tx_type, send_context->ref_tx);

        // If nothing wrong happen initialises all incoming TM devices
        uint32_t i = 0u;
        while ((i < send_context->send_table_size) && (device_status == RET_SUCCESSFUL))
        {
            device_status = DeviceOpen(&send_context->send_table[i].dev_buffer, DEVICE_TYPE_BUFFER, send_context->send_table[i].buffer);
            i++;
        }

        // If everything went right update context status
        if (device_status == RET_SUCCESSFUL)
        {
            send_context->status = PUS_CONTEXT_INITIALIZED;
        }
        else
        {
            send_context->status = PUS_CONTEXT_ERROR;
        }
    }
    else
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn          SendTM(pusSendContext_t *send_context)
 * @brief       Function that send reads incoming TM from the entry buffers and send them
 * @param[in]   send_context Send context for the task dealing with TM sending
 * @retval      #RET_INVALID_PARAM if send_context is not initialised
 * @retval      #RET_ERROR if cannot read entry buffers
 * @retval      #RET_ERROR if cannot send TM in the TX device
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SendTM(pusSendContext_t *send_context)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    pusTM_t *tm               = send_context->tm; // Renaming for easier usage

    // Check parameter(s)
    if (send_context->status == PUS_CONTEXT_INITIALIZED)
    {
        // Read each buffer in the send_table
        uint32_t i = 0u;
        while ((i < send_context->send_table_size) && (return_value == RET_SUCCESSFUL))
        {
            // Empty the buffer continuously
            returnCode_t tx_status = RET_SUCCESSFUL;
            while (tx_status == RET_SUCCESSFUL)
            {
                tx_status = DeviceRead(send_context->send_table[i].dev_buffer, (data_t)tm, TM_MAX_SIZE);
                if (tx_status == RET_SUCCESSFUL)
                {
                    // Get size of TM then format it
                    length_t tm_size = tm->spp_header.packet_data_length + SPP_HEADER_SIZE + 1u;
                    (void)FormatTM(tm);

                    // Send TM
                    tx_status = DeviceWrite(send_context->dev_tx, (data_t)tm, tm_size);
                }
            }

            // Check if it stops because of an error or because the buffer was empty
            if (tx_status != RET_NOT_AVAILABLE)
            {
                return_value = RET_ERROR;
            }

            i++;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size)
 * @brief       Function that build a TM.
 * @param[out]  tm Pointer to the TM we want to create
 * @param[in]   service PUS Service of TM.
 * @param[in]   subservice PUS Subservice of TM.
 * @param[in]   data Data Packet.
 * @param[in]   data_size Size of data packet.
 * @retval      #RET_INVALID_PARAM if tm is null pointer or service or subservice equal to 0
 * @retval      #RET_ERROR if cannot fill time field
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size)
{
    returnCode_t return_value  = RET_SUCCESSFUL;
    static uint16_t tm_counter = 0u;

    // Check parameter(s)
    if ((tm != NULL) && (service > 0u) && (subservice > 0u))
    {
        // Build SPP Header
        tm->spp_header.packet_id =                                                                           //
            (PACKET_VERSION_NUMBER_MASK & ((uint16_t)PACKET_VERSION_NUMBER << PACKET_VERSION_NUMBER_OFFSET)) // Packet Version Number (0)
            | (PACKET_TYPE_MASK & ((uint16_t)TM_TYPE << PACKET_TYPE_OFFSET))                                 // Packet Type (TM)
            | (HEADER_PRESENCE_MASK & ((uint16_t)HEADER_PRESENT << HEADER_PRESENCE_OFFSET))                  // Secondary Header (yes)
            | (APID_MASK & OBC_APID);                                                                        // APID (0x55)
        tm->spp_header.packet_sequence_control = 0xc000u + (0x3ffffu & tm_counter);
        tm_counter++;
        tm->spp_header.packet_data_length = TM_HEADER_SIZE + data_size + CRC_TRAILER_SIZE - 1u;

        // Build TM Header
        tm->tm_header.version_timeref = (PUS_VERSION_NUMBER_MASK & (PUS_VERSION_NUMBER << PUS_VERSION_NUMBER_OFFSET));
        tm->tm_header.service         = service;
        tm->tm_header.subservice      = subservice;
        tm->tm_header.message_counter = 0u;
        tm->tm_header.destination_id  = 0u;

        // Build Data
        if (data_size > 0u)
        {
            (void)memcpy(tm->data, data, data_size);
        }

        // Timestamp TM
        time_t current_time    = 0u;
        returnCode_t test_time = GetTime(&current_time);
        if (test_time == RET_SUCCESSFUL)
        {
            tm->tm_header.time.time_header    = (uint8_t)(((current_time) >> 56) & 0xffu);
            tm->tm_header.time.coarse_time[0] = (uint8_t)(((current_time) >> 48) & 0xffu);
            tm->tm_header.time.coarse_time[1] = (uint8_t)(((current_time) >> 40) & 0xffu);
            tm->tm_header.time.coarse_time[2] = (uint8_t)(((current_time) >> 32) & 0xffu);
            tm->tm_header.time.coarse_time[3] = (uint8_t)(((current_time) >> 24) & 0xffu);
            tm->tm_header.time.fine_time[0]   = (uint8_t)(((current_time) >> 16) & 0xffu);
            tm->tm_header.time.fine_time[1]   = (uint8_t)(((current_time) >> 8) & 0xffu);
            tm->tm_header.time.fine_time[2]   = (uint8_t)((current_time) & 0xffu);
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              FormatTM(pusTM_t *tm)
 * @brief           Function that format tm the right way
 * @param[in,out]   tm Pointer to the TM we want to format
 * @retval          #RET_INVALID_PARAM if tm is null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * As we work we little endian processors, but the TM and TM are big endian
 * formated, we need to swap to big endian before send the TM.
 *
 * @warning This function wont format TM data field, it has to be format before.
 */
returnCode_t FormatTM(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (tm != NULL)
    {
        uint16_t data_size = tm->spp_header.packet_data_length + 1u;

        // Endianness Correction
        tm->spp_header.packet_id               = HALF_WORD_BYTE_SWAP(tm->spp_header.packet_id);
        tm->spp_header.packet_sequence_control = HALF_WORD_BYTE_SWAP(tm->spp_header.packet_sequence_control);
        tm->spp_header.packet_data_length      = HALF_WORD_BYTE_SWAP(tm->spp_header.packet_data_length);
        tm->tm_header.message_counter          = HALF_WORD_BYTE_SWAP(tm->tm_header.message_counter);
        tm->tm_header.destination_id           = HALF_WORD_BYTE_SWAP(tm->tm_header.destination_id);

        // Compute CRC
        tm->crc = computeCRC((uint8_t *)tm, data_size + SPP_HEADER_SIZE - CRC_TRAILER_SIZE);

        // Put CRC at the right place
        tm->data[data_size - TM_HEADER_SIZE - CRC_TRAILER_SIZE]      = (pusData_t)((0xff00u & tm->crc) >> 8u);
        tm->data[data_size - TM_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] = (pusData_t)(0x00ffu & tm->crc);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              EraseTM(pusTM_t *tm)
 * @brief           Function that erase a TM, it fills it with zeros
 * @param[in,out]   tm Pointer to the TM we want to erase
 * @return          Nothing
 */
void EraseTM(pusTM_t *tm)
{
    (void)memset(tm, 0u, TM_MAX_SIZE);
}