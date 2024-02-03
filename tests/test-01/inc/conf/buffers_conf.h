/**
 * @file    buffers_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration table for buffers
 * @date    28/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BUFFERS_CONF_H
#define BUFFERS_CONF_H

/******************************* Include Files *******************************/

#include "buffers.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    BUFFERS_ENUM
 * @brief   Enum defining buffers reference numbers
 */
enum BUFFERS_ENUM {
    BUFF01_BUFFER = 0,
    NB_BUFFERS,
};

/*************************** Variables Declarations **************************/

extern const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS];
extern bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS];

#endif /* BUFFERS_CONF_H */