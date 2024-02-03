/**
 * @file    tc_execution.h
 * @author  Merlin Kooshmanian
 * @brief   Source file including function for generic TC execution
 * @date    14/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup tc_execution TC Execution
 * Function for generic TC execution
 * @{
 */

#ifndef TC_EXECUTION_H
#define TC_EXECUTION_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include "buffers.h"
#include "pus_tools/tc_management.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    tcProcessingStatus_t
 * @brief   TC execution functions specific returns 
 */
typedef enum
{
    TC_PROCESSING_SUCCESSFUL     = 0u,   /**< Function succeed */
    TC_PROCESSING_ERROR          = 1u,   /**< Function failed */
    TC_PROCESSING_INVALID_PARAM  = 2u,   /**< Function parameter is not valid */
    TC_PROCESSING_NOT_AVAILABLE  = 3u,   /**< Function is not available */
} tcProcessingStatus_t;

/** 
 * @struct  tcExecutionBasicBuffers_t
 * @brief   Struct type including basic buffers for TC execution
 */
typedef struct
{                            
    bufferRef_t tc_buffer;  /**< @brief TC input buffer */
    bufferRef_t tm_buffer;  /**< @brief TM output buffer */
    bufferRef_t ack_buffer; /**< @brief Acknowledge TM output buffer */
} tcExecutionBasicBuffers_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

tcProcessingStatus_t ProcessNewTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, bufferRef_t ack_buffer);
tcProcessingStatus_t ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, tcExecutionBasicBuffers_t basic_buffers);

#endif /* TC_EXECUTION_H */

/** 
 * @} 
 * @} 
 */