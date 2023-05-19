/**
 * @file buffers.h
 * @author Merlin Kooshmanian
 * @brief Header file defining tasks
 * @date 27/04/2023
 * 
 * Last Update : 27/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup buffers Buffers Management
 * Functions that allows to manage buffers in TAPAS
 * @{
 */

#ifndef BUFFERS_H
#define BUFFERS_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "cmsis_os2.h"
#include "tasks.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/** 
 * @typedef bufferStatus_t
 * @brief   Buffer functions specific returns 
 */
typedef enum
{
    BUFFER_SUCCESSFUL      = 0u,    /**< Function succeed */
    BUFFER_ERROR           = 1u,    /**< Function failed */
    BUFFER_INVALID_PARAM   = 2u,    /**< Function parameter is not valid */
    BUFFER_TIMEOUT         = 3u,    /**< Function returned a timeout */
    BUFFER_EMPTY           = 4u,    /**< Buffer is empty */
    BUFFER_FULL            = 5u,    /**< Buffer is full */
} bufferStatus_t;

/** @brief Buffer ID type */
typedef osMessageQueueId_t bufferId_t;

/** @brief Buffer Reference number type */
typedef uint32_t bufferRef_t;

/** @brief Buffer Reference number type */
typedef uint32_t bufferSize_t;

/** @brief Buffer Reference number type */
typedef uint32_t bufferDepth_t;

/** 
 * @typedef bufferDef_t
 * @struct  bufferDef_t
 * @brief   Struct type definition of a buffer
 */
typedef struct
{
    bufferRef_t ref;            /**< @brief Buffer reference number as it is declared in BUFFERS_ENUM */
    taskRef_t sender;           /**< @brief Task reference number of the sender */
    taskRef_t receiver;         /**< @brief Task reference number of the receiver */
    bufferSize_t max_size;      /**< @brief Maximum message size the buffer can handle */
    bufferDepth_t max_nb;       /**< @brief Maximum number of message the buffer can handle */
} bufferDef_t;

/************************** Function Prototypes ******************************/

bufferStatus_t createBuffers(void);
bufferStatus_t WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length);
bufferStatus_t ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length);

#endif /* BUFFERS_H */

/** @} */