/**
 * @file pus_constants.h
 * @author Merlin Kooshmanian
 * @brief Header for PUS constants
 * @date 08/07/2023
 *
 * Last Update : 08/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef PUS_CONSTANTS_H
#define PUS_CONSTANTS_H

/************************** Constant Definitions *****************************/

#define TC_MAX_SIZE         256u    /**< Maximum Size of a TC */
#define TM_MAX_SIZE         256u    /**< Maximum Size of a TM */
#define SPP_HEADER_SIZE     6u      /**< Space Packet Header Size */
#define CRC_TRAILER_SIZE    2u      /**< Size for CRC Trailer */
#define TC_HEADER_SIZE      5u      /**< Size of a TC Header */
#define TM_HEADER_SIZE      15u     /**< Size of a TM Header */
#define TC_MAX_DATA_SIZE    (TC_MAX_SIZE - SPP_HEADER_SIZE - TC_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TC data */
#define TM_MAX_DATA_SIZE    (TM_MAX_SIZE - SPP_HEADER_SIZE - TM_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TM data */

/* SPP Header Constant */
#define PACKET_VERSION_NUMBER_MASK      0xe000
#define PACKET_VERSION_NUMBER_OFFSET    13u
#define VALID_PACKET_VERSION_NUMBER     0u
#define PACKET_TYPE_MASK                0x1000
#define PACKET_TYPE_OFFSET              12u
#define TC_TYPE                         1u
#define TM_TYPE                         0u
#define HEADER_PRESENCE_MASK            0x0800
#define HEADER_PRESENCE_OFFSET          11u
#define HEADER_PRESENT                  1u
#define APID_MASK                       0x0fff
#define APID                            0u

/* PUS Header Constant */
#define PUS_VERSION_NUMBER_MASK         0xf0
#define PUS_VERSION_NUMBER_OFFSET       4u
#define VALID_PUS_VERSION_NUMBER        1u

/* PUS ACCEPTANCE ERROR */
#define PUS_ACCEPTANCE_NO_ERROR         0u  /**< TC is valid */
#define PUS_ACCEPTANCE_INVALID_FORMAT   1u  /**< TC is not well formated (wrong version, size or type) */
#define PUS_ACCEPTANCE_INVALID_CRC      2u  /**< Received CRC is not equal to computed CRC */

#endif /* PUS_CONSTANTS_H */
