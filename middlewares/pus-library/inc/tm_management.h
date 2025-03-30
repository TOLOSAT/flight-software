/**
 * @file    tm_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TM management
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup tm_management TM management
 * @brief PUS telemetry (TM) management layer.
 * @{
 */

#ifndef TM_MANAGEMENT_H
#define TM_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/**
 * @struct  pusSendTable_t
 * @brief   Struct type for send table
 */
typedef struct
{
    bufferNo_t buffer;     /**< @brief Buffer reference number from where the TM arrives */
    deviceNo_t dev_buffer; /**< @brief Device bound to the TM buffer */
} pusSendTable_t;

/**
 * @struct  pusSendContext_t
 * @brief   Struct type for execution context
 */
typedef struct
{
    pusContextStatus_t status;      /**< @brief Execution context status */
    pusSendTable_t *send_table;     /**< @brief Pointer to the send table */
    pusTableSize_t send_table_size; /**< @brief Size of the execution table */
    uint32_t ref_tx;                /**< @brief resource where the TM will be sent */
    deviceType_t tx_type;           /**< @brief Type of resource used to send the TM (e.g. buffer or peripheral) */
    deviceNo_t dev_tx;              /**< @brief Device bound to the TX resource */
    pusTM_t *tm;                    /**< @brief Pointer to a TM data field in the case it needs to be allocated in the DMA section */
} pusSendContext_t;

/*************************** Functions Declarations **************************/

extern returnCode_t InitTMSendContext(pusSendContext_t *send_context);
extern returnCode_t SendTM(pusSendContext_t *send_context);
extern returnCode_t BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size);
extern returnCode_t FormatTM(pusTM_t *tm);
extern void EraseTM(pusTM_t *tm);

#endif /* TM_MANAGEMENT_H */

/**
 * @}
 * @}
 * @}
 */