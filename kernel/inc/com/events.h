/**
 * @file    events.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for events messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup intercom Task Intercommunication
 * @{
 * @defgroup events Events Messages
 * @brief Provide standard interface for task events
 * @{
 */

#ifndef EVENTS_H
#define EVENTS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/time.h"
#include "core/buffers.h"

/***************************** Macros Definitions ****************************/

#define EVENT_MESSAGE_SIZE   11u     /**< Size of an event message */

/***************************** Types Definitions *****************************/

/** 
 * @struct  eventMessage_t
 * @brief   Struct type of an event message
 */
typedef struct
{                            
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t event_type;     /**< @brief Event Type */
    uint8_t event_subtype;  /**< @brief Event SubType */
    time_t time;         /**< @brief Current Time */
}BYTE_ALIGNED eventMessage_t;
ASSERT_SIZE(eventMessage_t, EVENT_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern kernelStatus_t SendEventMessage(eventMessage_t *event_message, bufferNo_t buffer);
extern kernelStatus_t ReceiveEventMessage(eventMessage_t *event_message, bufferNo_t buffer);

#endif /* EVENTS_H */

/** 
 * @}
 * @}
 * @}
 */