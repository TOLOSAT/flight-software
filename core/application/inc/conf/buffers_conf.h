/**
 * @file buffers_conf.h
 * @author Merlin Kooshmanian
 * @brief Header file stocking configuration table for buffers
 * @date 28/04/2023
 * 
 * Last Update : 28/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef BUFFERS_CONF_H
#define BUFFERS_CONF_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "buffers.h"

/************************** Constant Definitions *****************************/

/**
 * @enum    BUFFERS_ENUM
 * @brief   Enum defining buffers reference numbers
 */
enum BUFFERS_ENUM {
    /* Life Messages */
    MISO_LIFE = 0,
    CARNE_LIFE,
    TC_RECEIVER_LIFE,
    TM_SENDER_LIFE,
    TC_SCHEDULER_LIFE,
    TC_PROCESS_LIFE,
    HK_LIFE,
    AOCS_LIFE,
    POWER_LIFE,
    THERMAL_LIFE,
    GRAVIMETRY_LIFE,
    IRIDIUM_LIFE,
    /* Mode Change Requests */
    MISO_MODE_CHANGE,
    CARNE_MODE_CHANGE,
    /* Events Messages */
    SALAMI_EVENTS,
    MISO_EVENTS,
    AOCS_EVENTS,
    POWER_EVENTS,
    THERMAL_EVENTS,
    GRAVIMETRY_EVENTS,
    IRIDIUM_EVENTS,
    /* HK Messages */
    AOCS_HK,
    POWER_HK,
    THERMAL_HK,
    GRAVIMETRY_HK,
    IRIDIUM_HK,
    /* PUS Buffers */
    TC_NORMAL,
    TC_PUS3,
    TC_PUS5,
    TC_PUS11,
    TC_PUS160,
    TC_PUS161,
    TC_PUS176,
    TC_PUS177,
    TC_PUS178,
    TC_PUS192,
    TC_PUS193,
    TC_DELAYED,
    TM_NORMAL,
    TM_PUS1,
    TM_PUS3,
    TM_PUS5,
    TM_PUS160,
    TM_PUS161,
    TM_PUS176,
    TM_PUS177,
    TM_PUS178,
    TM_PUS192,
    TM_PUS193,
    NB_BUFFERS,
};

#endif /* BUFFERS_CONF_H */