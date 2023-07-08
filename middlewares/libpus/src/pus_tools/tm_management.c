/**
 * @file tm_management.h
 * @author Merlin Kooshmanian
 * @brief Source file for TM management
 * @date 06/07/2023
 *
 * Last Update : 06/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <string.h>

#include "pus_tools/tm_management.h"
#include "pus_tools/crc_computation.h"
#include "pus_tools/endianness_mgmt.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

uint16_t tm_counter = 0u;

/************************* Functions Definitions *****************************/

/**
 * @fn      BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size)
 * @brief   Function that build a TM.
 * @param   tm Pointer to the TM we want to create
 * @param   service PUS Service of TM.
 * @param   subservice PUS Subservice of TM.
 * @param   data Data Packet.
 * @param   data_size Size of data packet.
 * @retval  PUS_INVALID_PARAM if tm is null pointer or service or subservice equal to 0
 * @retval  PUS_SUCCESSFUL always
 */
pusStatus_t BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tm != NULL && service > 0 && subservice > 0)
    {
        // Build SPP Header
        tm->spp_header.packet_id = (PACKET_VERSION_NUMBER_MASK & (VALID_PACKET_VERSION_NUMBER << PACKET_VERSION_NUMBER_OFFSET)) |
                                   (PACKET_TYPE_MASK & (TM_TYPE << PACKET_TYPE_OFFSET)) |
                                   (HEADER_PRESENCE_MASK & (HEADER_PRESENT << HEADER_PRESENCE_OFFSET)) |
                                   (APID_MASK & APID);
        tm->spp_header.packet_sequence_control = 0xc000 + (0x3ffff & tm_counter);
        tm_counter++;
        tm->spp_header.packet_data_length = TM_HEADER_SIZE + data_size + CRC_TRAILER_SIZE - 1u;

        // Build TM Header
        tm->tm_header.version_timeref = (PUS_VERSION_NUMBER_MASK & (VALID_PUS_VERSION_NUMBER << PUS_VERSION_NUMBER_OFFSET));
        tm->tm_header.service = service;
        tm->tm_header.subservice = subservice;
        tm->tm_header.message_counter = 0u;
        tm->tm_header.destination_id = 0u;
        tm->tm_header.time = 0u; // Must be getTime function

        // Build Data
        if (data_size > 0)
        {
            memcpy(tm->data, data, data_size);
        }

        // Build CRC
        tm->crc = computeCRC((uint8_t *)tm, SPP_HEADER_SIZE + TM_HEADER_SIZE + data_size);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return (return_value);
}