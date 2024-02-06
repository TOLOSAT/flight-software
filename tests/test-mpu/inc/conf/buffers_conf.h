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
    BUFF01 = 0,
    BUFF02,
    NB_BUFFERS,
};

/*************************** Variables Declarations **************************/

extern const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS];
extern bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS];
extern bufferData_t g_buff01_data[12u*1u];
extern bufferData_t g_buff02_data[12u*1u];

#endif /* BUFFERS_CONF_H */