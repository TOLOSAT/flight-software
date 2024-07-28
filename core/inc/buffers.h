/**
 * @file    buffers.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining buffers
 * @date    27/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup buffers Buffers Management
 * Functions that allows to manage buffers in TAPAS
 * @{
 */

#ifndef BUFFERS_H
#define BUFFERS_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "tasks.h"
#include "os.h"

/***************************** Macros Definitions ****************************/

#define NO_BUFFER_REF        0xffffffffu /**< Reference number to refer for no buffer */

/***************************** Types Definitions *****************************/

/** @brief Buffer Handle type */
typedef QueueHandle_t bufferHandle_t;

/** @brief Buffer reference number type */
typedef uint32_t bufferRef_t;

/** @brief Buffer width type */
typedef uint32_t bufferSize_t;

/** @brief Buffer depth type */
typedef uint32_t bufferDepth_t;

/** @brief Buffer message address type */
typedef void *bufferMsgAddr_t;

/** @brief Buffer entity type */
typedef StaticQueue_t bufferEntity_t;

/** @brief Buffer data type */
typedef uint8_t bufferArray_t;

/** 
 * @struct  bufferConf_t
 * @struct  bufferConf_t
 * @brief   Struct type definition of a buffer
 */
typedef struct
{
    bufferRef_t ref;                    /**< @brief Buffer reference number as it is declared in BUFFERS_ENUM */
    taskRef_t sender;                   /**< @brief Task reference number of the sender */
    taskRef_t receiver;                 /**< @brief Task reference number of the receiver */
    bufferSize_t max_size;              /**< @brief Maximum message size the buffer can handle */
    bufferDepth_t max_nb;               /**< @brief Maximum number of message the buffer can handle */
    bufferEntity_t *p_buffer_entity;    /**< @brief Pointer to buffer entity */
    bufferArray_t *p_buffer_array;      /**< @brief Pointer to buffer data array */
} bufferConf_t;

/** 
 * @struct  bufferDesc_t
 * @brief   Struct type of a buffer dynamic parameters
 */
typedef struct
{                            
    bufferHandle_t handle;  /**< @brief Buffer handle */
    uint32_t nb_msg;        /**< @brief Current number of messages in buffer */
} bufferDesc_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t CreateBuffers(void);
extern coreStatus_t WriteBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length);
extern coreStatus_t ReadBuffer(bufferRef_t buffer, bufferMsgAddr_t msg, bufferSize_t length);
extern coreStatus_t GetBufferCount(bufferRef_t buffer, bufferDepth_t *count);

#endif /* BUFFERS_H */

/** 
 * @} 
 * @} 
 */