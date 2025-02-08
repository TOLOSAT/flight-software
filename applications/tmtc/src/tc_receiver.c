/**
 * @file    tc_receiver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_RECEIVER Task
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tc_receiver.h"
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_ROUTES 13u /**< Number of routes */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcReceiverMain(void)
 * @brief           Main of the TC_RECEIVER Task
 */
void TcReceiverMain(void)
{
    // Initialisation
    static pusTC_t IN_DMABUFF_SECTION received_tc        = { 0 };
    static pusTC_t delayed_tc                            = { 0 };
    static pusRoutingTable_t tc_routing_table[NB_ROUTES] =
    {
        { .key = BUILD_ROUTING_KEY(OBC_APID, 3u,   5u),   .route = TC_PUS3   },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 3u,   6u),   .route = TC_PUS3   },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 6u,   1u),   .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 6u,   3u),   .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 9u,   128u), .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  1u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  2u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  3u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  4u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 17u,  1u),   .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 161u, 1u),   .route = TC_PUS161 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 161u, 3u),   .route = TC_PUS161 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 161u, 5u),   .route = TC_PUS161 },
    };
    static pusReceiveContext_t receive_tc_context =
    {
        .routing_table      = tc_routing_table,
        .routing_table_size = NB_ROUTES,
        .ref_rx             = UART_TMTC,
        .rx_type            = DEVICE_TYPE_PERIPHERAL,
        .buffer_ack         = TM_PUS1,
        .tc                 = &received_tc,
    };
    static pusReceiveContext_t receive_delayed_tc_context =
    {
        .routing_table      = tc_routing_table,
        .routing_table_size = NB_ROUTES,
        .ref_rx             = TC_DELAYED,
        .rx_type            = DEVICE_TYPE_BUFFER,
        .buffer_ack         = TM_PUS1,
        .tc                 = &delayed_tc,
    };

    CheckError(InitTCReceiveContext(&receive_tc_context));
    CheckError(InitTCReceiveContext(&receive_delayed_tc_context));

    // Function Core
    while (1)
    {
        // Check if there was a normal TC.
        CheckError(ReceiveTC(&receive_tc_context));

        // Check if there was a delayed TC.
        CheckError(ReceiveTC(&receive_delayed_tc_context));

        // Yield
        CheckError(WaitSignal(SIGNAL_NEW_TC));
    }
}
