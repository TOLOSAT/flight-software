/**
 * @file    tc_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC management
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup tc_management TC Management
 * @brief PUS telecommand (TC) management layer.
 * @{
 */

#ifndef TC_MANAGEMENT_H
#define TC_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"
#include "tools/tables_management.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @struct  pusReceiveContext_t
 * @brief   Struct type for receive context
 */
typedef struct {
    pusContextStatus_t status;          /**< @brief Receive context status */
    pusRoutingTable_t *routing_table;   /**< @brief Pointer to the routing table */
    pusTableSize_t routing_table_size;  /**< @brief Size of the routing table */
    uint32_t ref_rx;                    /**< @brief Resource where the TC come from */
    deviceType_t rx_type;               /**< @brief Type of resource used to receive the TC (e.g. buffer or peripheral) */
    deviceNo_t dev_rx;                  /**< @brief Device bound to the RX resource */
    bufferNo_t buffer_ack;              /**< @brief Buffer where the ACK TM will be sent */
    deviceNo_t dev_ack;                 /**< @brief Device bound to the ACK buffer */
    pusTC_t *tc;                        /**< @brief Pointer to a TC data field in the case it needs to be allocated in the DMA section */
} pusReceiveContext_t;

/** 
 * @struct  pusExecutionContext_t
 * @brief   Struct type for execution context
 */
typedef struct {
    pusContextStatus_t status;              /**< @brief Execution context status */
    pusExecutionTable_t *execution_table;   /**< @brief Pointer to the execution table */
    pusTableSize_t execution_table_size;    /**< @brief Size of the execution table */
    bufferNo_t buffer_tc;                   /**< @brief Buffer where the TC come from */
    bufferNo_t buffer_tm;                   /**< @brief Buffer where the TM will be sent */
    bufferNo_t buffer_ack;                  /**< @brief Buffer where the ACK TM will be sent */
    deviceNo_t dev_tc;                      /**< @brief Device bound to the TC buffer */
    deviceNo_t dev_tm;                      /**< @brief Device bound to the TM buffer */
    deviceNo_t dev_ack;                     /**< @brief Device bound to the ACK buffer */
} pusExecutionContext_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitTCReceiveContext(pusReceiveContext_t *receive_context);
extern returnCode_t ReceiveTC(pusReceiveContext_t *receive_context);
extern returnCode_t InitTCExecutionContext(pusExecutionContext_t *execution_context);
extern returnCode_t ExecuteTC(pusExecutionContext_t *execution_context);

#endif /* TC_MANAGEMENT_H */

/** 
 * @}
 * @}
 * @}
 */