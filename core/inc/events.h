/**
 * @file    events.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for events messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup events Events Messages
 * Functions that allows tasks to raise attention for events
 * @{
 */

#ifndef EVENTS_H
#define EVENTS_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include "buffers.h"
#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

#define EVENT_MESSAGE_SIZE   9u     /**< Size of an event message */

/***************************** Types Definitions *****************************/

/** 
 * @enum    eventStatus_t
 * @brief   Events functions specific returns 
 */
typedef enum
{
    EVENT_SUCCESSFUL            = 0u,    /**< Function succeed */
    EVENT_ERROR                 = 1u,    /**< Function failed */
    EVENT_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    EVENT_TIMEOUT               = 3u,    /**< Function returned a timeout */
} eventStatus_t;

/** 
 * @struct  eventMessage_t
 * @brief   Struct type of an event message
 */
typedef struct
{                            
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t event_type;     /**< @brief Event Type */
    uint8_t event_subtype;  /**< @brief Event SubType */
    cucTime_t time;         /**< @brief Current Time */
}BYTE_ALIGNED eventMessage_t;
ASSERT_SIZE(eventMessage_t, EVENT_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

eventStatus_t SendEventMessage(eventMessage_t *event_message, bufferRef_t buffer_ref);
eventStatus_t ReceiveEventMessage(eventMessage_t *event_message, bufferRef_t buffer_ref);
eventStatus_t ConvertEventMessageIntoReport(eventMessage_t *event_message, eventReport_t *event_report);

#endif /* EVENTS_H */

/** 
 * @} 
 * @} 
 */