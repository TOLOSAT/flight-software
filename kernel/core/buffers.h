/**
 * @file    buffers.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining buffers
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

#include "autoconf.h"
#include "kernel_types.h"
#include "core/tasks.h"
#include "core/os.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     BUFFER_CONF(buffer_no)
 * @brief   Get buffer conf from g_buffers_conf_table
 */
#define BUFFER_CONF(buffer_no) (g_buffers_conf_table[(buffer_no) - 1u])

/**
 * @def     BUFFER_DESC(buffer_no)
 * @brief   Get buffer conf from g_buffers_desc_table
 */
#define BUFFER_DESC(buffer_no) (g_buffers_desc_table[(buffer_no) - 1u])

/**
 * @def     IS_A_VALID_BUFFER(buffer_no)
 * @brief   Indicates if the buffer_no is valid
 */
#define IS_A_VALID_BUFFER(buffer_no) \
    (((buffer_no) != (bufferNo_t)NO_BUFFER) && ((buffer_no) < (bufferNo_t)CONFIG_MAX_NB_BUFFERS) && (BUFFER_DESC(buffer_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/** @brief Buffer Handle type */
typedef QueueHandle_t bufferHandle_t;

/** @brief Buffer entity type */
typedef StaticQueue_t bufferQueue_t;

/** @brief Buffer data type */
typedef uint8_t bufferArray_t;

/**
 * @struct  bufferDesc_t
 * @brief   Struct type of a buffer dynamic parameters
 */
typedef struct
{
    descStatus_t status;   /**< @brief Indicates if the descriptor is free or used */
    bufferHandle_t handle; /**< @brief Buffer handle */
    uint32_t nb_msg;       /**< @brief Current number of messages in buffer */
} bufferDesc_t;

/*************************** Variables Declarations **************************/

extern const bufferConf_t g_buffers_conf_table[CONFIG_MAX_NB_BUFFERS];
extern bufferDesc_t g_buffers_desc_table[CONFIG_MAX_NB_BUFFERS];

/*************************** Functions Declarations **************************/

extern void CreateBuffers(void);
extern returnCode_t BufferWrite(bufferNo_t buffer, data_t data, length_t length);
extern returnCode_t BufferRead(bufferNo_t buffer, data_t data, length_t length);
extern returnCode_t BufferIoctl(bufferNo_t buffer, uint32_t cmd, void *data, uint32_t data_size);

#endif /* BUFFERS_H */

/**
 * @}
 * @}
 * @}
 */