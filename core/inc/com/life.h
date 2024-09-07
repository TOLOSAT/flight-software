/**
 * @file    life.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for life messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup life Life Messages
 * Functions that allows tasks to share their life status
 * @{
 */

#ifndef LIFE_H
#define LIFE_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "base/buffers.h"

/***************************** Macros Definitions ****************************/

#define LIFE_MESSAGE_SIZE  11u     /**< Size of a life message */

/***************************** Types Definitions *****************************/

/** 
 * @struct  lifeMessage_t
 * @brief   Struct type of a life message
 */
typedef struct
{                            
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t task_mode;      /**< @brief Task Current Mode */
    uint8_t status_type;   /**< @brief Task Current Status */
    time_t time;         /**< @brief Current Time */
}BYTE_ALIGNED lifeMessage_t;
ASSERT_SIZE(lifeMessage_t, LIFE_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t SendLifeMessage(lifeMessage_t *life_message, bufferNo_t buffer);
extern coreStatus_t ReceiveLifeMessage(lifeMessage_t *life_message, bufferNo_t buffer);

#endif /* LIFE_H */

/** 
 * @} 
 * @} 
 */