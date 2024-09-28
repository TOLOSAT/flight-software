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

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern pusStatus_t ReceiveTC(pusTC_t *tc, deviceNo_t dev_tc);
extern pusStatus_t ProcessNewTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, deviceNo_t dev_ack);
extern pusStatus_t ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, deviceNo_t dev_tc, deviceNo_t dev_tm, deviceNo_t dev_ack);
extern pusStatus_t CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error);
extern pusStatus_t FormatTC(pusTC_t *tc);
extern void EraseTC(pusTC_t *tc);

#endif /* TC_MANAGEMENT_H */

/** 
 * @}
 * @}
 * @}
 */