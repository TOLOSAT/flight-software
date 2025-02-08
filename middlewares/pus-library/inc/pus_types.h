/**
 * @file    pus_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header for PUS types
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 */

#ifndef PUS_TYPES_H
#define PUS_TYPES_H

/******************************* Include Files *******************************/

#include "common_types.h"
#include "pus_constants.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    pusContextStatus_t
 * @brief   Type enum use to indicates if this pus context is initialized or not
 */
typedef enum
{
    PUS_CONTEXT_NOT_INITIALIZED = 0u, /**< Context as been initialized */
    PUS_CONTEXT_INITIALIZED     = 1u, /**< Context as not been initialized */
    PUS_CONTEXT_ERROR           = 2u, /**< Context is in a error mode (not working anymore) */
} pusContextStatus_t;

/** @brief Size for table type */
typedef uint32_t pusTableSize_t;

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
typedef uint16_t pusSourceID_t;

/** @brief Destination ID Field type */
typedef uint16_t pusDestinationID_t;

/** @brief PUS Data type */
typedef uint8_t pusData_t;

/** @brief CRC for TMs or TCs */
typedef uint16_t pusCRC_t;

/**
 * @struct  cucTime_t
 * @brief   Struct type for CUC Time
 */
typedef struct
{
    uint8_t time_header;                   /**< @brief Field that helps to know which standard was choosen */
    uint8_t coarse_time[COARSE_TIME_SIZE]; /**< @brief Field that contains time in second since reference */
    uint8_t fine_time[FINE_TIME_SIZE];     /**< @brief Field that contains time fraction */
} ATTR_BYTE_ALIGNED cucTime_t;
ASSERT_SIZE(cucTime_t, CUC_TIME_SIZE)

/**
 * @struct  sppHeader_t
 * @brief   Struct type for a SPP Header
 */
typedef struct
{
    sppPacketId_t packet_id;                         /**< @brief Packet ID */
    sppPacketSequenceCtrl_t packet_sequence_control; /**< @brief TMTC counter for this ID */
    sppDataLength_t packet_data_length;              /**< @brief Packet Data Field Length */
} ATTR_BYTE_ALIGNED sppHeader_t;
ASSERT_SIZE(sppHeader_t, SPP_HEADER_SIZE)

/**
 * @struct  pusTCHeader_t
 * @brief   Struct type for a TC
 */
typedef struct
{
    tcVersionFlags_t version_flags; /**< @brief PUS Version and Acknowledgment Flag */
    pusService_t service;           /**< @brief PUS Service */
    pusSubService_t subservice;     /**< @brief PUS SubService */
    pusSourceID_t source_id;        /**< @brief ID of source application */
} ATTR_BYTE_ALIGNED pusTCHeader_t;
ASSERT_SIZE(pusTCHeader_t, TC_HEADER_SIZE)

/**
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
} ATTR_BYTE_ALIGNED pusTMHeader_t;
ASSERT_SIZE(pusTMHeader_t, TM_HEADER_SIZE)

/**
 * @struct  pusTC_t
 * @brief   Struct type for a TC
 */
typedef struct
{
    sppHeader_t spp_header;           /**< @brief Space Packet Header */
    pusTCHeader_t tc_header;          /**< @brief PUS TC Header */
    pusData_t data[TC_MAX_DATA_SIZE]; /**< @brief TC Raw Data */
    pusCRC_t crc;                     /**< @brief TC CRC */
} ATTR_BYTE_ALIGNED pusTC_t;
ASSERT_SIZE(pusTC_t, TC_MAX_SIZE)

/**
 * @struct  pusTM_t
 * @brief   Struct type for a TM
 */
typedef struct
{
    sppHeader_t spp_header;           /**< @brief Space Packet Header */
    pusTMHeader_t tm_header;          /**< @brief PUS TM Header */
    pusData_t data[TM_MAX_DATA_SIZE]; /**< @brief TM Raw Data */
    pusCRC_t crc;                     /**< @brief TM CRC */
} ATTR_BYTE_ALIGNED pusTM_t;
ASSERT_SIZE(pusTM_t, TM_MAX_SIZE)

/**
 * @brief Acceptance Error Type
 */
typedef uint8_t pusAcceptanceError_t;

/**
 * @brief Execution Error Type
 */
typedef uint8_t pusExecutionError_t;

#endif /* PUS_TYPES_H */

/**
 * @}
 * @}
 */