/**
 * @file    housekeeping.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for housekeeping messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup housekeeping Housekeeping Messages
 * Functions that allows tasks to share their housekeeping data
 * @{
 */

#ifndef HOUSEKEEPING_H
#define HOUSEKEEPING_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "buffers.h"
#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

#define HOUSEKEEPING_MESSAGE_SIZE   12u     /**< Size of a housekeeping message */

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
    cucTime_t time;         /**< @brief Current Time */
}BYTE_ALIGNED housekeepingMessage_t;
ASSERT_SIZE(housekeepingMessage_t, HOUSEKEEPING_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t SendHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferRef_t buffer_ref);
extern coreStatus_t ReceiveHousekeepingMessage(housekeepingMessage_t *housekeeping_message, bufferRef_t buffer_ref);
extern coreStatus_t ConvertHousekeepingMessageIntoReport(housekeepingMessage_t *event_message, housekeepingReport_t *event_report);

#endif /* HOUSEKEEPING_H */

/** 
 * @} 
 * @} 
 */