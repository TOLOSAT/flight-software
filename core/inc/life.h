/**
 * @file    life.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for life messages
 * @date    22/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
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

#include <stdint.h>

#include "buffers.h"
#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

#define LIFE_MESSAGE_SIZE   9u     /**< Size of a life message */

/***************************** Types Definitions *****************************/

/** 
 * @enum    lifeStatus_t
 * @brief   Life functions specific returns 
 */
typedef enum
{
    LIFE_SUCCESSFUL            = 0u,    /**< Function succeed */
    LIFE_ERROR                 = 1u,    /**< Function failed */
    LIFE_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    LIFE_TIMEOUT               = 3u,    /**< Function returned a timeout */
} lifeStatus_t;

/** 
 * @struct  lifeMessage_t
 * @brief   Struct type of a life message
 */
typedef struct
{                            
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t task_mode;      /**< @brief Task Current Mode */
    uint8_t status_type;   /**< @brief Task Current Status */
    cucTime_t time;         /**< @brief Current Time */
}BYTE_ALIGNED lifeMessage_t;
ASSERT_SIZE(lifeMessage_t, LIFE_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

lifeStatus_t SendLifeMessage(lifeMessage_t *life_message, bufferRef_t buffer_ref);
lifeStatus_t ReceiveLifeMessage(lifeMessage_t *life_message, bufferRef_t buffer_ref);

#endif /* LIFE_H */

/** 
 * @} 
 * @} 
 */