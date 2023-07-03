/**
 * @file tc_management.h
 * @author Merlin Kooshmanian
 * @brief Source file for TC management
 * @date 02/07/2023
 *
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <string.h>

#include "pus_tools/tc_management.h"
#include "pus_tools/crc_computation.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      FormatTC(pusTC_t *tc)
 * @brief   Function that format TC the right way
 * @param   tc Pointer to the TC we want to format
 * @retval  PUS_SUCCESSFUL always
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
    tc->spp_header.packet_id = ((0xff00 & tc->spp_header.packet_id) >> 8u) | ((0x00ff & tc->spp_header.packet_id) << 8u);
    tc->spp_header.packet_sequence_control = ((0xff00 & tc->spp_header.packet_sequence_control) >> 8u) | ((0x00ff & tc->spp_header.packet_sequence_control) << 8u);
    tc->spp_header.packet_data_length = ((0xff00 & tc->spp_header.packet_data_length) >> 8u) | ((0x00ff & tc->spp_header.packet_data_length) << 8u);
    tc->tc_header.source_id = ((0xff00 & tc->tc_header.source_id) >> 8u) | ((0x00ff & tc->tc_header.source_id) << 8u);
    
    // Put CRC at the right place
    tc->crc = tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] << 8u | tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 2u];
    tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] = 0u;
    tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 2u] = 0u;

    return(return_value);
}

/**
 * @fn      CheckCRC(pusTC_t *tc)
 * @brief   Function that verifies a received TC has not been corrupted
 * @param   tc Pointer to the TC variable where we want to check it CRC
 * @retval  PUS_ERROR if the computed CRC is different than the received CRC
 * @retval  PUS_SUCCESSFUL else
 */
pusStatus_t CheckCRC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusCRC_t computed_crc = 0u;
    pusCRC_t reiceved_crc = 0u;
    uint32_t data_size = 0u;

    // Function Core
    data_size = (((0xff00 & tc->spp_header.packet_data_length) >> 8u) | ((0x00ff & tc->spp_header.packet_data_length) << 8u)) + 1u;
    computed_crc = computeCRC((uint8_t *) tc, data_size + SPP_HEADER_SIZE - CRC_TRAILER_SIZE);
    reiceved_crc = tc->data[data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE] << 8u | tc->data[data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u];

    if(computed_crc != reiceved_crc){
        return_value = PUS_ERROR;
    }

    return(return_value);
}

/**
 * @fn      CheckCRC(pusTC_t *tc)
 * @brief   Function that verifies if APID, Service and Subservice of the TC are valid
 * @param   tc Pointer to the TC variable where we want to verify it validity.
 * @retval  PUS_ERROR if 
 * @retval  PUS_SUCCESSFUL else
 */
pusStatus_t CheckTCValidity(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core

    return(return_value);
}

/**
 * @fn      EraseTC(pusTC_t *tc)
 * @brief   Function that erase a TC, it fills it with zeros
 * @param   tc Pointer to the TC we want to erase
 * @retval  PUS_SUCCESSFUL always
 */
pusStatus_t EraseTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    memset((uint8_t*) tc, 0u, TC_MAX_SIZE);

    return(return_value);
}