/**
 * @file    pus_init.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TM management
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus_init PUS Library Initialisation
 * Initialise every PUS dependancies. This allows
 * PUS library to remains independant from OS.
 * @{
 */

#ifndef PUS_INIT_H
#define PUS_INIT_H

/******************************* Include Files *******************************/

#include "pus_types.h"
#include "buffers.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  pusConf_t
 * @brief   Struct type for lib PUS configuration
 */
typedef struct
{
    bufferRef_t ack_buffer;          /**< @brief acknowledgment buffer */
} pusConf_t;

/*************************** Variables Declarations **************************/

extern pusConf_t g_pus_conf;

/*************************** Functions Declarations **************************/

extern pusStatus_t PusInitConf(pusConf_t *pus_init_conf);

#endif /* PUS_INIT_H */

/** 
 * @} 
 * @} 
 */