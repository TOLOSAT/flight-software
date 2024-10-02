/**
 * @file    buffers.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining buffers
 * @date    27/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup buffers Buffers
 * @brief Buffers handling interface.
 * @{
 */

#ifndef BUFFERS_H
#define BUFFERS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "conf/buffers_conf.h"
#include "core/tasks.h"
#include "utils/os.h"

/***************************** Macros Definitions ****************************/

#define IN_BUFFER_ARRAYS_SECTION    __attribute__((section(".buffer_arrays")))      /**< Buffer data go to .buffer_arrays section */
#define IN_BUFFER_ENTITIES_SECTION  __attribute__((section(".buffer_entities")))    /**< Buffer data go to .buffer_entities section */

#define NO_BUFFER   (-1u)   /**< Reference number to refer for no buffer */

/***************************** Types Definitions *****************************/

/** @brief Buffer Handle type */
typedef QueueHandle_t bufferHandle_t;

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
    bufferNo_t buffer;                  /**< @brief Buffer reference number as it is declared in BUFFERS_ENUM */
    taskNo_t sender;                    /**< @brief Task reference number of the sender */
    taskNo_t receiver;                  /**< @brief Task reference number of the receiver */
    length_t  max_size;                 /**< @brief Maximum message size the buffer can handle */
    length_t max_nb;                    /**< @brief Maximum number of message the buffer can handle */
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

/**
 * @var     g_buffers_conf
 * @brief   Configuration table where all buffers' static parameters are stored
 */
extern const bufferConf_t g_buffers_conf[NB_BUFFERS];

/**
 * @var     g_buffers_desc_table
 * @brief   Configuration table where all buffers' descriptors are stored
 */
extern bufferDesc_t g_buffers_desc_table[NB_BUFFERS];

/*************************** Functions Declarations **************************/

extern kernelStatus_t CreateBuffers(void);
extern kernelStatus_t BufferWrite(bufferNo_t buffer, data_t data, length_t length);
extern kernelStatus_t BufferRead(bufferNo_t buffer, data_t data, length_t length);
extern kernelStatus_t GetBufferCount(bufferNo_t buffer, length_t *count);

#endif /* BUFFERS_H */

/** 
 * @}
 * @}
 * @}
 */