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

/** 
 * @typedef bufferDef_t
 * @struct  bufferDef_t
 * @brief   Struct type definition of a buffer
 */
typedef struct
{
    uint32_t buffer_ref;                    /**< @brief Buffer reference number as it is declared in BUFFERS_ENUM */
    uint32_t buffer_max_size;               /**< @brief Maximum message size the buffer can handle */
    uint32_t buffer_max_nb;                 /**< @brief Maximum number of message the buffer can handle */
    osMessageQueueAttr_t buffer_attribute;  /**< @brief Attribute that store information on the buffer for the OS */
} bufferDef_t;

/** @brief Buffer ID type */
typedef osMessageQueueId_t bufferId_t;

/** @brief Buffer Reference number type */
typedef uint32_t bufferRef_t;

/************************** Function Prototypes ******************************/

bufferStatus_t createBuffers(void);
bufferStatus_t WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length);
bufferStatus_t ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length);

#endif /* BUFFERS_H */

/** @} */