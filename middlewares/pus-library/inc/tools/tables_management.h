/**
 * @file    tables_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for execution or routing tables
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup tables_management Tables Management
 * @brief PUS specific tables management layer.
 * @{
 */

#ifndef TABLES_MANAGEMENT_H
#define TABLES_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/**
 * @def  BUILD_ROUTING_KEY(apid, service, subservice)
 * @brief Preprocessor function that build routing key with APID, sevice and subservice
 */
#define BUILD_ROUTING_KEY(apid, service, subservice) ((uint32_t)(((uint32_t)(apid) << 16) | ((uint32_t)(service) << 8) | (uint32_t)(subservice)))

/***************************** Types Definitions *****************************/

/** @brief Pointer to execution function type */
typedef returnCode_t (*pusExecutionFunctionPtr_t)(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

/**
 * @enum    pusTMRequested_t
 * @brief   Type enum use to indicates if this TC needs a specific TM to be send
 */
typedef enum
{
    TM_NOT_REQUESTED = 0u, /**< No specific TM has to be send for this TC */
    TM_REQUESTED     = 1u, /**< A TM has to be send for this TC */
} pusTMRequested_t;

/**
 * @struct  pusExecutionTable_t
 * @brief   Struct type for execution table
 */
typedef struct
{
    uint32_t key;                                 /**< @brief Key allowing to link to the execution function */
    pusExecutionFunctionPtr_t execution_function; /**< @brief Execution function */
    pusTMRequested_t tm_requested;                /**< @brief Indicates if this TC needs a specific TM to be send */
} pusExecutionTable_t;

/**
 * @struct  pusRoutingTable_t
 * @brief   Struct type for routing table
 */
typedef struct
{
    uint32_t key;         /**< @brief Key allowing to route */
    uint32_t route;       /**< @brief Route reference number */
    deviceNo_t dev_route; /**< @brief Route device (i.e. a buffer device) */
} pusRoutingTable_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitRoutingTable(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size);
extern returnCode_t InitExecutionTable(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size);
extern returnCode_t RouteSearch(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size, uint32_t key, deviceNo_t *dev_route);
extern returnCode_t ExecutionSearch(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size, uint32_t key, pusTMRequested_t *tm_requested,
                                    pusExecutionFunctionPtr_t *execution_function_ptr);

#endif /* TABLES_MANAGEMENT_H */

/**
 * @}
 * @}
 * @}
 */