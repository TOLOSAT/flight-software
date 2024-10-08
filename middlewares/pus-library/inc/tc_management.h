/**
 * @file    tc_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC management
 * @date    02/07/2023
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
 * @struct  pusExecutionContext_t
 * @brief   Struct type for execution context
 */
typedef struct {
    pusContextStatus_t status;              /**< @brief Execution context status */
    pusExecutionTable_t *execution_table;   /**< @brief Pointer to the execution table */
    pusTableSize_t execution_table_size;    /**< @brief Size of the execution table */
    bufferNo_t buffer_tc;                   /**< @brief Device where the TC come from */
    bufferNo_t buffer_tm;                   /**< @brief Device where the TM will be sent */
    bufferNo_t buffer_ack;                  /**< @brief Device where the ACK TM will be sent */
    deviceNo_t dev_tc;                      /**< @brief Device bounded to the TC buffer */
    deviceNo_t dev_tm;                      /**< @brief Device bounded to the TM buffer */
    deviceNo_t dev_ack;                     /**< @brief Device bounded to the ACK buffer */
} pusExecutionContext_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t ReceiveTC(pusTC_t *tc, deviceNo_t dev_tc);
extern returnCode_t ProcessNewTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, deviceNo_t dev_ack);
extern returnCode_t InitTCExecutionContext(pusExecutionContext_t *execution_context);
extern returnCode_t ExecuteTC(pusExecutionContext_t *execution_context);

#endif /* TC_MANAGEMENT_H */

/** 
 * @}
 * @}
 * @}
 */