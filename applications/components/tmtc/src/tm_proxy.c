/**
 * @file    tm_proxy.c
 * @author  Matteo Planchet, Arthur Morain
 * @brief   Source file for TM_PROXY Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "tm_proxy.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

#define NB_SEND_ENTRY 7u /**< Maximum number of input buffers */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc TmProxyMain
 */
void TmProxyMain(void)
{
    // Initialisation
    static pusSendTable_t tm_send_table[NB_SEND_ENTRY] = {
        { .buffer = TM_PUS1 },   { .buffer = TM_PUS3 },   { .buffer = TM_PUS5 },   { .buffer = TM_PUS15 },
        { .buffer = TM_NORMAL }, { .buffer = TM_PUS178 }, { .buffer = TM_PUS193 },
    };

    // Task Core
    while (1)
    {
        // Write TMs into his specified buffer
        CheckError(SendTM(&send_tm_context), SEVERITY_MEDIUM);

        CheckError(WaitSignal(SIGNAL_TM), SEVERITY_MEDIUM);
    }
}