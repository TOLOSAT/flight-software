/**
 * @file    housekeeping.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for housekeeping messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup intercom Task Intercommunication
 * @{
 * @defgroup housekeeping Housekeeping Messages
 * @brief Provide standard interface for task housekeeping
 * @{
 */

#ifndef HOUSEKEEPING_H
#define HOUSEKEEPING_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/time.h"
#include "core/buffers.h"

/***************************** Macros Definitions ****************************/

#define HOUSEKEEPING_MESSAGE_SIZE   14u     /**< Size of a housekeeping message */

/***************************** Types Definitions *****************************/

/** 
 * @struct  housekeepingMessage_t
 * @brief   Struct type of an housekeeping message
 */
typedef struct
{                            
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t observable;     /**< @brief Housekeeping Observable */
    uint32_t value;         /**< @brief Housekeeping Observable Value */
    time_t time;         /**< @brief Current Time */
}BYTE_ALIGNED housekeepingMessage_t;
ASSERT_SIZE(housekeepingMessage_t, HOUSEKEEPING_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern kernelStatus_t SendHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferNo_t buffer);
extern kernelStatus_t ReceiveHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferNo_t buffer);

#endif /* HOUSEKEEPING_H */

/** 
 * @}
 * @}
 * @}
 */