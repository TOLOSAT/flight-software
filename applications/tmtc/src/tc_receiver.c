/**
 * @file    tc_receiver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_RECEIVER Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "tc_receiver.h"
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_ROUTES 25u /**< Number of routes */

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
    static pusRoutingTable_t tc_routing_table[NB_ROUTES] = {
        // PUS Service 3 : Housekeeping
        { .key = BUILD_ROUTING_KEY(OBC_APID, 3u,   5u),   .route = TC_PUS3   },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 3u,   6u),   .route = TC_PUS3   },
        // PUS Service 6 : Memory management
        { .key = BUILD_ROUTING_KEY(OBC_APID, 6u,   1u),   .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 6u,   3u),   .route = TC_NORMAL },
        // PUS Service 9 : Time Management
        { .key = BUILD_ROUTING_KEY(OBC_APID, 9u,   128u), .route = TC_NORMAL },
        // PUS Service 11 : Time Based Scheduling
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  1u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  2u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  3u),   .route = TC_PUS11  },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 11u,  4u),   .route = TC_PUS11  },
        // PUS Service 17 : Test
        { .key = BUILD_ROUTING_KEY(OBC_APID, 17u,  1u),   .route = TC_NORMAL },
        // PUS Service 160 : System PUS Service
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 1u),   .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 2u),   .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 17u),  .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 19u),  .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 21u),  .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 23u),  .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 33u),  .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 35u),  .route = TC_NORMAL },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 160u, 37u),  .route = TC_NORMAL },
        // PUS Service 193 : Iridium PUS Service
        { .key = BUILD_ROUTING_KEY(OBC_APID, 193u, 1u),   .route = TC_PUS193 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 193u, 2u),   .route = TC_PUS193 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 193u, 3u),   .route = TC_PUS193 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 193u, 5u),   .route = TC_PUS193 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 193u, 7u),   .route = TC_PUS193 },
        { .key = BUILD_ROUTING_KEY(OBC_APID, 193u, 8u),   .route = TC_PUS193 },
    };
    static pusReceiveContext_t receive_tc_context = {
        .routing_table      = tc_routing_table,
        .routing_table_size = NB_ROUTES,
        .ref_rx             = UART_TMTC,
        .rx_type            = DEVICE_TYPE_PERIPHERAL,
        .buffer_ack         = TM_PUS1,
        .tc                 = &received_tc,
    };
    static pusReceiveContext_t receive_delayed_tc_context = {
        .routing_table      = tc_routing_table,
        .routing_table_size = NB_ROUTES,
        .ref_rx             = TC_DELAYED,
        .rx_type            = DEVICE_TYPE_BUFFER,
        .buffer_ack         = TM_PUS1,
        .tc                 = &delayed_tc,
    };

    CheckError(InitTCReceiveContext(&receive_tc_context));
    CheckError(InitTCReceiveContext(&receive_delayed_tc_context));

    // Task Core
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
