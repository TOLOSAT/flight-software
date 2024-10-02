/**
 * @file    tc_receiver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_RECEIVER Task
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tmtc/tc_receiver.h"
#include "kernel.h"
#include "pus.h"
#include "services/pus1.h"

/***************************** Macros Definitions ****************************/

#define NB_ROUTES   13u  /**< Number of routes */

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
    uint32_t task_status;
    static pusTC_t IN_DMABUFF_SECTION received_tc = {0};
    static pusRoutingTable_t tc_routing_table[NB_ROUTES] =
    {
        {.key = BUILD_ROUTING_KEY(OBC_APID,  3u,   5u) , .route = TC_PUS3   },
        {.key = BUILD_ROUTING_KEY(OBC_APID,  3u,   6u) , .route = TC_PUS3   },
        {.key = BUILD_ROUTING_KEY(OBC_APID,  6u,   1u) , .route = TC_NORMAL },
        {.key = BUILD_ROUTING_KEY(OBC_APID,  6u,   3u) , .route = TC_NORMAL },
        {.key = BUILD_ROUTING_KEY(OBC_APID,  9u, 128u) , .route = TC_NORMAL },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 11u,   1u) , .route = TC_PUS11  },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 11u,   2u) , .route = TC_PUS11  },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 11u,   3u) , .route = TC_PUS11  },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 11u,   4u) , .route = TC_PUS11  },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 17u,   1u) , .route = TC_NORMAL },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 161u,  1u) , .route = TC_PUS161 },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 161u,  3u) , .route = TC_PUS161 },
        {.key = BUILD_ROUTING_KEY(OBC_APID, 161u,  5u) , .route = TC_PUS161 },
    };
    deviceNo_t dev_uart_tmtc_rx = 0u;
    deviceNo_t dev_delayed_tc = 0u;
    deviceNo_t dev_ack_buffer = 0u;
    pusTC_t delayed_tc = {0};
    
    task_status = InitRoutingTable((pusRoutingTable_t *)&tc_routing_table, NB_ROUTES);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_uart_tmtc_rx, DEVICE_TYPE_PERIPHERAL, UART_TMTC, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_delayed_tc, DEVICE_TYPE_BUFFER, TC_DELAYED, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_ack_buffer, DEVICE_TYPE_BUFFER, TM_PUS1, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceIoctl(dev_uart_tmtc_rx, UART_IOCTL_START_RX, &received_tc, TC_MAX_SIZE);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC.
        pusStatus_t tc_handling_status = ReceiveTC(&received_tc, dev_uart_tmtc_rx);
        if (tc_handling_status == PUS_SUCCESSFUL)
        {
            // New TC available
            task_status = ProcessNewTC((pusRoutingTable_t *)&tc_routing_table, NB_ROUTES, &received_tc, dev_ack_buffer);
            CheckErrors(task_status, FDIR_NO_SANCTION);
        }

        // Second, we check if there is a delayed TC.
        tc_handling_status = ReceiveTC(&delayed_tc, dev_delayed_tc);
        if (tc_handling_status == PUS_SUCCESSFUL)
        {
            // New delayed TC available
            task_status = ProcessNewTC((pusRoutingTable_t *)&tc_routing_table, NB_ROUTES, &delayed_tc, dev_ack_buffer);
            CheckErrors(task_status, FDIR_NO_SANCTION);
        }

        SleepPeriodic();
    }
}
