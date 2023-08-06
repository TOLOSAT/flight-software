/**
 * @file    tc_receiver.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_RECEIVER Task
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "tm_tc/tc_receiver.h"
#include "fdir.h"
#include "conf/io_conf.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "pus_tools/tc_management.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/tables_management.h"
#include "services/pus1.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t ReceiveTC(pusTC_t *tc);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_tc_routing_table
 * @brief   Routing table for incomming TC
 * @warning Keys must be ordered from smallest to largest
 */
pusRoutingTable_t g_tc_routing_table[NB_ROUTES] =
    {
    {.key = BUILD_ROUTING_KEY(OBC_APID,  9u, 128u) , .route = TC_NORMAL },
    {.key = BUILD_ROUTING_KEY(OBC_APID, 17u,   1u) , .route = TC_NORMAL },
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      TcReceiverMain(void *task_dyn_conf)
 * @brief   Main of the TC_RECEIVER Task
 * @param   task_dyn_conf Status of the current task
 */
void TcReceiverMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    uint32_t key;
    bufferRef_t route = 0u;
    pusTC_t tc = {0};
    pusTM_t acceptance_tm = {0};
    pusAcceptanceError_t acceptance_error = PUS_ACCEPTANCE_NO_ERROR;

    // Initialisation
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckRoutingTable((pusRoutingTable_t *)&g_tc_routing_table, NB_ROUTES);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC.
        pusStatus_t tc_handling_status = ReceiveTC(&tc);
        if (tc_handling_status == PUS_SUCCESSFUL)
        {
            // Then, we check the validity of the TC.
            tc_handling_status = CheckTCValidity(&tc, &acceptance_error);
            if (tc_handling_status == PUS_SUCCESSFUL)
            {
                // If TC is valid, we format the TC because of endianness.
                (void)FormatTC(&tc);
                // Then, we route the TC toward the task that will execute it.
                key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
                tc_handling_status = RouteSearch((pusRoutingTable_t *)&g_tc_routing_table, NB_ROUTES, key, &route);
                if (tc_handling_status == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC
                    task_status = BuildS1SS1(&tc, &acceptance_tm);
                    CheckErrors(task_status, FDIR_NO_SANCTION);
                    task_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)&acceptance_tm, TM_MAX_SIZE);
                    CheckErrors(task_status, FDIR_NO_SANCTION);

                    // Send TC to the task that will execute it
                    task_status = WriteBuffer(route, (bufferMsgAddr_t)&tc, TC_MAX_SIZE);
                    CheckErrors(task_status, FDIR_NO_SANCTION);
                }
                else
                {
                    // Bad routing so TC non acknowleded
                    task_status = BuildS1SS2(&tc, &acceptance_tm, PUS_ACCEPTANCE_INVALID_ROUTE);
                    CheckErrors(task_status, FDIR_NO_SANCTION);
                    task_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)&acceptance_tm, TM_MAX_SIZE);
                    CheckErrors(task_status, FDIR_NO_SANCTION);
                }
            }
            else
            {
                // Invalid TC, TC will be non-acknowledged.
                task_status = BuildS1SS2(&tc, &acceptance_tm, acceptance_error);
                CheckErrors(task_status, FDIR_NO_SANCTION);
                task_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)&acceptance_tm, TM_MAX_SIZE);
                CheckErrors(task_status, FDIR_NO_SANCTION);
            }
        }

        // We reset the TM & TC variables until next call;
        EraseTC(&tc);
        EraseTM(&acceptance_tm);

        // Wait until next call of the task
        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @fn      ReceiveTC(pusTC_t *tc)
 * @brief   Function that get a TC if there is any read by the DMA
 * @param   tc Pointer to the TC variable where we want to store it
 * @retval  #PUS_NO_MSG if there is no TC available
 * @retval  #PUS_SUCCESSFUL else
 */
static pusStatus_t ReceiveTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    halStatus_t read_status = FCT_SUCCESSFUL;

    // Function Core
    read_status = UartRead(&uart_tmtc_inst, (uartMsg_t *)tc, TC_MAX_SIZE);
    if (read_status != FCT_SUCCESSFUL)
    {
        return_value = PUS_NO_MSG;
    }

    return return_value;
}
