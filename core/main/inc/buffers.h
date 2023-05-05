/**
 * @file buffers.h
 * @author Merlin Kooshmanian
 * @brief Header file defining tasks
 * @date 27/04/2023
 * 
 * Last Update : 27/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef BUFFERS_H
#define BUFFERS_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "cmsis_os2.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum {
    BUFFERS_SUCCESSFUL      = 0u,    /**< Function succeed */
    BUFFERS_ERROR           = 1u,    /**< Function failed */
    BUFFERS_INVALID_PARAM   = 2u,    /**< Function parameter is not valid */
    BUFFERS_TIMEOUT         = 3u,    /**< Function returned a timeout */
    BUFFERS_EMPTY           = 4u,    /**< Buffer is empty */
    BUFFERS_FULL            = 5u,    /**< Buffer is full */
} bufferStatus_t;

typedef struct {
    uint32_t buffer_ref;
    uint32_t buffer_max_size;
    uint32_t buffer_max_nb;
    osMessageQueueAttr_t buffer_attribute;
} bufferDef_t;

typedef osMessageQueueId_t bufferId_t;

typedef uint32_t bufferRef_t;

/************************** Function Prototypes ******************************/

bufferStatus_t createBuffers(void);
bufferStatus_t WriteBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length);
bufferStatus_t ReadBuffer(bufferRef_t buffer, uint32_t *msg, uint32_t length);

#endif /* BUFFERS_H */