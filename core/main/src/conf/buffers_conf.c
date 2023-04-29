/**
 * @file buffers_conf.c
 * @author Merlin Kooshmanian
 * @brief Source file stocking configuration table for buffers
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "conf/buffers_conf.h"

/***************************** Configuration *********************************/

#define EMPTY_STRUCT    {NULL}

const bufferDef_t g_buffers_conf[NB_BUFFERS] = {
    {BUFF01_BUFFER, 12 ,1, EMPTY_STRUCT},
};

bufferId_t g_buffers_ids[NB_BUFFERS];