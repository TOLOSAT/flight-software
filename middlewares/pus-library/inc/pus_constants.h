/**
 * @file    pus_constants.h
 * @author  Merlin Kooshmanian
 * @brief   Header for PUS constants
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 */

#ifndef PUS_CONSTANTS_H
#define PUS_CONSTANTS_H

/***************************** Macros Definitions ****************************/

/************************************/
/************** SIGNALS *************/
/************************************/

#define SIGNAL_NEW_TC                (SIGNAL_USER15 | SIGNAL_PERIPHERAL_RX_DONE) /**< Signal indicading a new TC is available */
#define SIGNAL_NEW_TM                SIGNAL_USER14                               /**< Signal indicading a new TM is available */

/************************************/
/******* PUS GENERIC CONSTANTS ******/
/************************************/

#define OBC_APID                     0x55u /**< On Board Computer APID */

#define TC_MAX_SIZE                  256u                                                                /**< Maximum Size of a TC */
#define TM_MAX_SIZE                  256u                                                                /**< Maximum Size of a TM */
#define SPP_HEADER_SIZE              6u                                                                  /**< Space Packet Header Size */
#define CRC_TRAILER_SIZE             2u                                                                  /**< Size for CRC Trailer */
#define TC_HEADER_SIZE               5u                                                                  /**< Size of a TC Header */
#define TM_HEADER_SIZE               15u                                                                 /**< Size of a TM Header */
#define TC_MAX_DATA_SIZE             (TC_MAX_SIZE - SPP_HEADER_SIZE - TC_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TC data */
#define TM_MAX_DATA_SIZE             (TM_MAX_SIZE - SPP_HEADER_SIZE - TM_HEADER_SIZE - CRC_TRAILER_SIZE) /**< Maximum Size for TM data */

/* CUC Time constant */
#define CUC_HEAD_SIZE                1u                                                  /**< CUC header size */
#define COARSE_TIME_SIZE             4u                                                  /**< Coarse time size */
#define FINE_TIME_SIZE               3u                                                  /**< Fine time size */
#define CUC_TIME_SIZE                (CUC_HEAD_SIZE + COARSE_TIME_SIZE + FINE_TIME_SIZE) /**< CUC time variables size */
#define CUC_TIME_STR_SIZE            (2 * CUC_TIME_SIZE) /**< Number of char needed to represent CUC time as a string */

/* SPP Header Constant */
#define PACKET_VERSION_NUMBER_MASK   0xe000u /**< Bit mask to access packet version number */
#define PACKET_VERSION_NUMBER_OFFSET 13u     /**< Offset to access packet version number */
#define PACKET_VERSION_NUMBER        0u      /**< Used packet version number (0 : CCSDS 133. 0-B-1) */
#define PACKET_TYPE_MASK             0x1000u /**< Bit mask to access packet type */
#define PACKET_TYPE_OFFSET           12u     /**< Offset to access packet type */
#define TC_TYPE                      1u      /**< Packet type is TC */
#define TM_TYPE                      0u      /**< Packet type is TM */
#define HEADER_PRESENCE_MASK         0x0800u /**< Bit mask to access secondary header presence bit */
#define HEADER_PRESENCE_OFFSET       11u     /**< Offset to access secondary header presence bit */
#define HEADER_PRESENT               1u      /**< Header is present */
#define APID_MASK                    0x07ffu /**< Bit mask to access APID */

/* PUS Header Constant */
#define PUS_VERSION_NUMBER_MASK      0xf0u /**< Bit mask to access PUS version number */
#define PUS_VERSION_NUMBER_OFFSET    4u    /**< Offset to access PUS version number */
#define PUS_VERSION_NUMBER           2u    /**< Valid PUS version number (2 : ECSS-E-ST-70-41C)*/

/************************************/
/******* ACCEPTANCE CONSTANTS *******/
/************************************/

/**
 * @enum    ACCEPTANCE_ERROR
 * @brief   PUS acceptance error code
 */
enum ACCEPTANCE_ERROR
{
    PUS_ACCEPTANCE_NO_ERROR       = 0u, /**< TC is valid */
    PUS_ACCEPTANCE_INVALID_FORMAT = 1u, /**< TC is not well formated (wrong version, size or type) */
    PUS_ACCEPTANCE_INVALID_CRC    = 2u, /**< Received CRC is not equal to computed CRC */
    PUS_ACCEPTANCE_INVALID_ROUTE  = 3u, /**< Route does not exist */
    PUS_ACCEPTANCE_CANT_FORMAT    = 4u, /**< TC cannot be formatted into a readable TC */
};

/**
 * @enum    EXECUTION_ERROR
 * @brief   PUS execution error code
 */
enum EXECUTION_ERROR
{
    PUS_EXECUTION_NO_ERROR           = 0u, /**< TC can be executed */
    PUS_EXECUTION_UNAVAILABLE        = 1u, /**< TC execution procedure is unavailable */
    PUS_EXECUTION_FAILED             = 2u, /**< TC execution failed */
    PUS_EXECUTION_UNEXPECTED_DATA    = 3u, /**< TC has unexpected data */
    PUS_EXECUTION_TM_BUILDING_FAILED = 4u, /**< TC execution function cannot create TM from obtained data */
};

#endif /* PUS_CONSTANTS_H */

/**
 * @}
 * @}
 */