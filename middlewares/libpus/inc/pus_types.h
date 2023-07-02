/**
 * @file pus_types.h
 * @author Merlin Kooshmanian
 * @brief Header type for PUS
 * @date 02/07/2023
 *
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef PUS_TYPES_H
#define PUS_TYPES_H

/***************************** Include Files *********************************/

#include <stdint.h>

/************************** Constant Definitions *****************************/

#define TC_MAX_SIZE         256u    /**< Maximum Size of a TC */
#define TM_MAX_SIZE         256u    /**< Maximum Size of a TM */
#define SPP_HEADER_SIZE     6u      /**< Space Packet Header Size */
#define CRC_TRAILER_SIZE    2u      /**< Size for CRC Trailer */
#define TC_HEADER_SIZE      4u      /**< Size of a TC Header */
#define TM_HEADER_SIZE      15u     /**< Size of a TM Header */
#define TC_MAX_DATA_SIZE    (TC_MAX_SIZE - SPP_HEADER_SIZE - TC_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TC data */
#define TM_MAX_DATA_SIZE    (TM_MAX_SIZE - SPP_HEADER_SIZE - TM_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TM data */

/**************************** Type Definitions *******************************/

/** 
 * @typedef pusStatus_t
 * @brief   PUS functions specific returns 
 */
typedef enum
{
    PUS_SUCCESSFUL = 0u,    /**< Function succeed */
    PUS_ERROR = 1u,         /**< Function failed */
    PUS_INVALID_PARAM = 2u, /**< Function parameter is not valid */
    PUS_NO_MSG = 3u,       /**< Function has no message to deal with */
} pusStatus_t;

/** @brief Packet ID for SPP Header */
typedef uint16_t sppPacketId_t;

/** @brief Packet Sequence Control for SPP Header  */
typedef uint16_t sppPacketSequenceCtrl_t;

/** @brief Packet Data Length for SPP Header */
typedef uint16_t sppDataLength_t;

/** @brief Version and Flag Field type */
typedef uint8_t tcVersionFlags_t;

/** @brief Version and Time Reference type */
typedef uint8_t tmVersionTimeRef_t;

/** @brief Service Field type */
typedef uint8_t pusService_t;

/** @brief Subservice Field type */
typedef uint8_t pusSubService_t;

/** @brief Message Counter Field type */
typedef uint16_t pusMsgCount_t;

/** @brief Source ID Field type */
typedef uint8_t pusSourceID_t;

/** @brief Destination ID Field type */
typedef uint16_t pusDestinationID_t;

/** @brief CUC Time Field type */
typedef uint64_t cucTime_t;

/** @brief PUS Data type */
typedef uint8_t pusData_t;

/** @brief CRC for TMs or TCs */
typedef uint16_t pusCRC_t;

/** 
 * @typedef sppHeader_t
 * @struct  sppHeader_t
 * @brief   Struct type for a SPP Header
 */
typedef struct
{                            
    sppPacketId_t packet_id;                            /**< @brief Packet ID */
    sppPacketSequenceCtrl_t packet_sequence_control;    /**< @brief TMTC counter for this ID */
    sppDataLength_t packet_data_length;                 /**< @brief Packet Data Field Length */
} sppHeader_t;

/** 
 * @typedef pusTCHeader_t
 * @struct  pusTCHeader_t
 * @brief   Struct type for a TC
 */
typedef struct
{                            
    tcVersionFlags_t version_flags; /**< @brief PUS Version and Acknowledgment Flag */
    pusService_t service;           /**< @brief PUS Service */
    pusSubService_t subservice;     /**< @brief PUS SubService */
    pusSourceID_t source_id;        /**< @brief ID of source application */
} pusTCHeader_t;

/** 
 * @typedef pusTMHeader_t
 * @struct  pusTMHeader_t
 * @brief   Struct type for a TM
 */
typedef struct
{                            
    tmVersionTimeRef_t version_timeref; /**< @brief PUS Version and Time Reference */
    pusService_t service;               /**< @brief PUS Service */
    pusSubService_t subservice;         /**< @brief PUS SubService */
    pusMsgCount_t message_counter;      /**< @brief Message counter */
    pusDestinationID_t destination_id;  /**< @brief ID of destination application */
    cucTime_t time;                     /**< @brief OBT when TC has been emitted */
} pusTMHeader_t;

/** 
 * @typedef pusTC_t
 * @struct  pusTC_t
 * @brief   Struct type for a TC
 */
typedef struct
{                            
    sppHeader_t spp_header;             /**< @brief Space Packet Header */
    pusTCHeader_t tc_header;            /**< @brief PUS TC Header */
    pusData_t data[TC_MAX_DATA_SIZE];   /**< @brief TC Raw Data */
    pusCRC_t crc;                       /**< @brief TC CRC */
} pusTC_t;

/** 
 * @typedef pusTM_t
 * @struct  pusTM_t
 * @brief   Struct type for a TM
 */
typedef struct
{                            
    sppHeader_t spp_header;             /**< @brief Space Packet Header */
    pusTMHeader_t tm_header;            /**< @brief PUS TM Header */
    pusData_t data[TM_MAX_DATA_SIZE];   /**< @brief TM Raw Data */
    pusCRC_t crc;                       /**< @brief TM CRC */
} pusTM_t;

/************************** Function Prototypes ******************************/

#endif /* PUS_TYPES_H */
