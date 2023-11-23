/**
 * @file    pus_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header for PUS types
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef PUS_TYPES_H
#define PUS_TYPES_H

/******************************* Include Files *******************************/

#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include "pus_constants.h"
#include "time_types.h"

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

/***************************** Types Definitions *****************************/

/**
 * @enum    pusStatus_t
 * @brief   PUS functions specific returns
 */
typedef enum
{
    PUS_SUCCESSFUL = 0u,    /**< Function succeed */
    PUS_ERROR = 1u,         /**< Function failed */
    PUS_INVALID_PARAM = 2u, /**< Function parameter is not valid */
    PUS_NOT_AVAILABLE = 3u,        /**< Function has nothing available to deal with */
} pusStatus_t;

/*******************************/
/******* PUS GENERIC TYPE ******/
/*******************************/

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
 * @struct  sppHeader_t
 * @brief   Struct type for a SPP Header
 */
typedef struct
{
    sppPacketId_t packet_id;                            /**< @brief Packet ID */
    sppPacketSequenceCtrl_t packet_sequence_control;    /**< @brief TMTC counter for this ID */
    sppDataLength_t packet_data_length;                 /**< @brief Packet Data Field Length */
} BYTE_ALIGNED sppHeader_t;
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
} BYTE_ALIGNED pusTCHeader_t;
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
} BYTE_ALIGNED pusTMHeader_t;
ASSERT_SIZE(pusTMHeader_t, TM_HEADER_SIZE)

/**
 * @struct  pusTC_t
 * @brief   Struct type for a TC
 */
typedef struct
{
    sppHeader_t spp_header;             /**< @brief Space Packet Header */
    pusTCHeader_t tc_header;            /**< @brief PUS TC Header */
    pusData_t data[TC_MAX_DATA_SIZE];   /**< @brief TC Raw Data */
    pusCRC_t crc;                       /**< @brief TC CRC */
} BYTE_ALIGNED pusTC_t;
ASSERT_SIZE(pusTC_t, TC_MAX_SIZE)

/**
 * @struct  pusTM_t
 * @brief   Struct type for a TM
 */
typedef struct
{
    sppHeader_t spp_header;             /**< @brief Space Packet Header */
    pusTMHeader_t tm_header;            /**< @brief PUS TM Header */
    pusData_t data[TM_MAX_DATA_SIZE];   /**< @brief TM Raw Data */
    pusCRC_t crc;                       /**< @brief TM CRC */
} BYTE_ALIGNED pusTM_t;
ASSERT_SIZE(pusTM_t, TM_MAX_SIZE)

/*******************************/
/***** PUS 1 SPECIFIC TYPE *****/
/*******************************/

/**
 * @brief Acceptance Error Type
 */
typedef uint8_t pusAcceptanceError_t;

/**
 * @brief Execution Error Type
 */
typedef uint8_t pusExecutionError_t;

/*******************************/
/***** PUS 3 SPECIFIC TYPE *****/
/*******************************/

/**
 * @struct  housekeepingReport_t
 * @brief   Struct type for an housekeeping report
 */
typedef struct
{
    uint32_t HKID;                          /**< @brief HouseKeeping ID */
    uint8_t data[HOUSEKEEPING_DATA_SIZE];   /**< @brief HouseKeeping data */
} BYTE_ALIGNED housekeepingReport_t;
ASSERT_SIZE(housekeepingReport_t, HOUSEKEEPING_REPORT_SIZE)

/*******************************/
/***** PUS 5 SPECIFIC TYPE *****/
/*******************************/

/**
 * @enum    pusEventSeverity_t
 * @brief   PUS 5 event severity enum
 */
typedef enum
{
    PUS5_INFORMATIVE_EVENT = 0u,        /**< Informative event */
    PUS5_LOW_SEVERITY_EVENT = 1u,       /**< Low severity event */
    PUS5_MEDIUM_SEVERITY_EVENT = 2u,    /**< Medium severity event */
    PUS5_HIGH_SEVERITY_EVENT = 3u,      /**< High severity event */
} pusEventSeverity_t;

/**
 * @struct  eventReport_t
 * @brief   Struct type for an event report
 */
typedef struct
{
    uint32_t EID;                   /**< @brief Event ID */
    uint8_t data[EVENT_DATA_SIZE];  /**< @brief Event data */
} BYTE_ALIGNED eventReport_t;
ASSERT_SIZE(eventReport_t, EVENT_REPORT_SIZE)

/*******************************/
/***** PUS 6 SPECIFIC TYPE *****/
/*******************************/

/**
 * @struct  pusTCLoadDataField_t
 * @brief   Struct type for memory load TC data field
 */
typedef struct
{
    uint8_t memory_id;                          /**< @brief Memory ID (= disk ID) that will be loaded in memory */
    uint8_t base;                               /**< @brief Data base (= file ref no) that will be loaded in memory */
    uint32_t offset;                            /**< @brief Data offset in base that will be loaded in memory */
    uint32_t length;                            /**< @brief Data length that will be loaded in memory */
    uint8_t data[MEMORY_TC_DATA_LOAD_MAX_SIZE]; /**< @brief Data that will be loaded in memory */
} BYTE_ALIGNED pusTCLoadDataField_t;
ASSERT_SIZE(pusTCLoadDataField_t, TC_MAX_DATA_SIZE)

/**
 * @struct  pusTCDumpDataField_t
 * @brief   Struct type for memory dump TC data field
 */
typedef struct
{
    uint8_t memory_id;  /**< @brief Memory ID (= disk ID) that will be loaded in memory */
    uint8_t base;       /**< @brief Data base (= file ref no) that will be loaded in memory */
    uint32_t offset;    /**< @brief Data offset in base that will be loaded in memory */
    uint32_t length;    /**< @brief Data length that will be loaded in memory */
} BYTE_ALIGNED pusTCDumpDataField_t;
ASSERT_SIZE(pusTCDumpDataField_t, MEMORY_TC_DATA_DUMP_SIZE)

/**
 * @struct  pusTMDumpDataField_t
 * @brief   Struct type for memory dump TM data field
 */
typedef struct
{
    uint8_t memory_id;                          /**< @brief Memory ID (= disk ID) that will be dumped from memory */
    uint8_t base;                               /**< @brief Data base (= file ref no) that will be dumped from memory */
    uint32_t offset;                            /**< @brief Data offset in base that will be dumped from memory */
    uint32_t length;                            /**< @brief Data length that will be dumped from memory */
    uint8_t data[MEMORY_TM_DATA_DUMP_MAX_SIZE]; /**< @brief Data that will be dumped from memory */
} BYTE_ALIGNED pusTMDumpDataField_t;
ASSERT_SIZE(pusTMDumpDataField_t, TM_MAX_DATA_SIZE)

/*******************************/
/***** PUS 11 SPECIFIC TYPE ****/
/*******************************/

/**
 * @struct  pusAddActivityTCDataField_t
 * @brief   Struct type for add activity data field
 */
typedef struct
{
    cucTime_t timestamp;    /**< @brief Activity Timestamp */
    uint8_t data[PUS11_ACTIVITY_DATA_MAX_SIZE];          /**< @brief Activity Data (is a TC but currently dummy uint32) */
} BYTE_ALIGNED pusAddActivityTCDataField_t;
ASSERT_SIZE(pusAddActivityTCDataField_t, TC_MAX_DATA_SIZE)

#endif /* PUS_TYPES_H */
