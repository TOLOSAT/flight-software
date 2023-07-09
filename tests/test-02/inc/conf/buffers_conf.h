/**
 * @file buffers_conf.h
 * @author Merlin Kooshmanian
 * @brief Header file stocking configuration table for buffers
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef BUFFERS_CONF_H
#define BUFFERS_CONF_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "buffers.h"

/*************************** Macros Definitions ******************************/

/**
 * @enum    BUFFERS_ENUM
 * @brief   Enum defining buffers reference numbers
 */
enum BUFFERS_ENUM {
    /* PUS Buffers */
    TC_NORMAL,
    TC_PUS11,
    TC_DELAYED,
    TM_NORMAL,
    TM_PUS1,
    NB_BUFFERS,
};

#endif /* BUFFERS_CONF_H */