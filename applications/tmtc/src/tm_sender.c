/**
 * @file    tm_sender.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TM_SENDER Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "tm_sender.h"
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_SEND_ENTRY 2u /**< Maximum number of input buffers */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TmSenderMain(void)
 * @brief           Main of the TM_SENDER Task
 */
void TmSenderMain(void)
{
    // Initialisation
    static pusTM_t IN_DMABUFF_SECTION send_tm          = { 0 };
    static pusSendTable_t tm_send_table[NB_SEND_ENTRY] = {
        { .buffer = TM_PUS1 },
        { .buffer = TM_NORMAL },
    };
    static pusSendContext_t send_tm_context = {
        .send_table      = tm_send_table,
        .send_table_size = NB_SEND_ENTRY,
        .ref_tx          = UART_TMTC,
        .tx_type         = DEVICE_TYPE_PERIPHERAL,
        .tm              = &send_tm,
    };

    CheckError(InitTMSendContext(&send_tm_context));

    // Task Core
    while (1)
    {
        // Send TMs if any available
        CheckError(SendTM(&send_tm_context));

        CheckError(WaitSignal(SIGNAL_TM));
    }
}
