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

/*************************** Macros Definitions ******************************/

#define OBC_APID                        0x55    /**< On Board Computer APID */

#define TC_MAX_SIZE                     256u    /**< Maximum Size of a TC */
#define TM_MAX_SIZE                     256u    /**< Maximum Size of a TM */
#define SPP_HEADER_SIZE                 6u      /**< Space Packet Header Size */
#define CRC_TRAILER_SIZE                2u      /**< Size for CRC Trailer */
#define TC_HEADER_SIZE                  5u      /**< Size of a TC Header */
#define TM_HEADER_SIZE                  15u     /**< Size of a TM Header */
#define TC_MAX_DATA_SIZE                (TC_MAX_SIZE - SPP_HEADER_SIZE - TC_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TC data */
#define TM_MAX_DATA_SIZE                (TM_MAX_SIZE - SPP_HEADER_SIZE - TM_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TM data */

/* SPP Header Constant */
#define PACKET_VERSION_NUMBER_MASK      0xe000  /**< Bit mask to access packet version number */
#define PACKET_VERSION_NUMBER_OFFSET    13u     /**< Offset to access packet version number */
#define VALID_PACKET_VERSION_NUMBER     0u      /**< Valid packet version number */
#define PACKET_TYPE_MASK                0x1000  /**< Bit mask to access packet type */
#define PACKET_TYPE_OFFSET              12u     /**< Offset to access packet type */
#define TC_TYPE                         1u      /**< Packet type is TC */
#define TM_TYPE                         0u      /**< Packet type is TM */
#define HEADER_PRESENCE_MASK            0x0800  /**< Bit mask to access secondary header presence bit */
#define HEADER_PRESENCE_OFFSET          11u     /**< Offset to access secondary header presence bit */
#define HEADER_PRESENT                  1u      /**< Header is present */
#define APID_MASK                       0x0fff  /**< Bit mask to access APID */

/* PUS Header Constant */
#define PUS_VERSION_NUMBER_MASK         0xf0    /**< Bit mask to access PUS version number */
#define PUS_VERSION_NUMBER_OFFSET       4u      /**< Offset to access PUS version number */
#define VALID_PUS_VERSION_NUMBER        1u      /**< Valid PUS version number */

/* PUS ACCEPTANCE ERROR */
#define PUS_ACCEPTANCE_NO_ERROR         0u  /**< TC is valid */
#define PUS_ACCEPTANCE_INVALID_FORMAT   1u  /**< TC is not well formated (wrong version, size or type) */
#define PUS_ACCEPTANCE_INVALID_CRC      2u  /**< Received CRC is not equal to computed CRC */

#endif /* PUS_CONSTANTS_H */
