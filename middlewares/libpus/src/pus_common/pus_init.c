/**
 * @file    pus_init.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for lib PUS init
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "pus_common.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus_conf
 * @brief   Configuration table for buffer management for pus library
 */
pusConf_t IN_PUS_DATA_SECTION g_pus_conf = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          PusInitConf(pusConf_t *pus_init_conf)
 * @brief       Function that initialises buffer configuration for lib pus
 * @param[in]   pus_init_conf Pointer to the buffer configuration initialisation struct
 * @retval      #PUS_INVALID_PARAM if buffer_conf is a null pointer
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION PusInitConf(pusConf_t *pus_init_conf)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (pus_init_conf != NULL)
    {
        g_pus_conf.ack_buffer = pus_init_conf->ack_buffer;
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}
