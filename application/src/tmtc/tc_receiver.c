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
#include "core.h"
#include "pus.h"
#include "services/pus1.h"

/***************************** Macros Definitions ****************************/

#define NB_ROUTES   13u  /**< Number of routes */

/*************************** Functions Declarations **************************/

static pusStatus_t ReceiveTC(pusTC_t *tc);
static pusStatus_t ReceiveDelayedTC(pusTC_t *delayed_tc);

/*************************** Variables Definitions ***************************/

/**
 * @var     dev_uart_tmtc_rx
 * @brief   UART TMTC RX device
 */
static deviceNo_t IN_TMTC_DATA_SECTION dev_uart_tmtc_rx;

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcReceiverMain(void *task_desc)
 * @brief           Main of the TC_RECEIVER Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_TMTC_TEXT_SECTION TcReceiverMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    static pusRoutingTable_t IN_TMTC_DATA_SECTION tc_routing_table[NB_ROUTES] =
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

    static pusTC_t IN_DMABUFF_SECTION received_tc = {0};
    pusTC_t delayed_tc = {0};

    // Initialisation
    task_status = CheckRoutingTable((pusRoutingTable_t *)&tc_routing_table, NB_ROUTES);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_uart_tmtc_rx, UART_TMTC, 0u);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceIoctl(dev_uart_tmtc_rx, UART_IOCTL_START_RX, &received_tc, TC_MAX_SIZE);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC.
        pusStatus_t tc_handling_status = ReceiveTC(&received_tc);
        if (tc_handling_status == PUS_SUCCESSFUL)
        {
            // New TC available
            task_status = ProcessNewTC((pusRoutingTable_t *)&tc_routing_table, NB_ROUTES, &received_tc, TM_PUS1);
            CheckErrors(task_status, FDIR_NO_SANCTION);
        }

        // Second, we check if there is a delayed TC.
        tc_handling_status = ReceiveDelayedTC(&delayed_tc);
        if (tc_handling_status == PUS_SUCCESSFUL)
        {
            // New delayed TC available
            task_status = ProcessNewTC((pusRoutingTable_t *)&tc_routing_table, NB_ROUTES, &delayed_tc, TM_PUS1);
            CheckErrors(task_status, FDIR_NO_SANCTION);
        }

        // Wait until next call of the task
        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn          ReceiveTC(pusTC_t *tc)
 * @brief       Function that get a TC if there is any read by the DMA
 * @param[out]  tc Pointer to the TC variable where we want to store it
 * @retval      #PUS_NOT_AVAILABLE if there is no TC available
 * @retval      #PUS_ERROR if UartRead() encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_TMTC_TEXT_SECTION ReceiveTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tc != NULL)
    {
        coreStatus_t test_rx = DeviceRead(dev_uart_tmtc_rx, (uartMsg_t *)tc, TC_MAX_SIZE);
        if (test_rx != CORE_SUCCESSFUL)
        {
            if (test_rx == CORE_BUSY)
            {
                return_value = PUS_NOT_AVAILABLE;
            }
            else
            {
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ReceiveDelayedTC(pusTC_t *delayed_tc)
 * @brief       Function that get a delayed TC if there is any in delayed tc buffer
 * @param[out]  delayed_tc Pointer to the TC variable where we want to store it
 * @retval      #PUS_NOT_AVAILABLE if there is no TC available
 * @retval      #PUS_ERROR if ReadBuffer() encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_TMTC_TEXT_SECTION ReceiveDelayedTC(pusTC_t *delayed_tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (delayed_tc != NULL)
    {
        coreStatus_t buffer_status = ReadBuffer(TC_DELAYED, (bufferMsgAddr_t)delayed_tc, TC_MAX_SIZE);
        if (buffer_status != CORE_SUCCESSFUL)
        {
            if (buffer_status == CORE_TIMEOUT)
            {
                return_value = PUS_NOT_AVAILABLE;
            }
            else
            {
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}