/**
 * @file tc_receiver.c
 * @author Merlin Kooshmanian
 * @brief Source file for TC_RECEIVER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tm_tc/tc_receiver.h"
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

#define TASK_NAME       "TC_RECEIVER"       /**< Current Task Name */
#define TASK_ID         (TC_RECEIVER_TASK)  /**< Current Task ID */
#define NB_ROUTES       1u                  /**< Number of routes */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

pusStatus_t ReceiveTC(pusTC_t *tc);
pusStatus_t SendTMToBuffer(pusTM_t *tm, bufferRef_t buffer_ref);

/*************************** Variables Definitions ***************************/

extern uartInst_t uart_tmtc_inst;

pusRoutingTable_t g_tc_routing_table[NB_ROUTES] = 
{
    {.key = BUILD_ROUTING_KEY(OBC_APID, 17u, 1u) , .route = TC_NORMAL  },
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
    pusStatus_t tc_handling_status;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    bufferRef_t route = 0u;
    uint32_t key = 0u;
    pusAcceptanceError_t acceptance_error = PUS_ACCEPTANCE_NO_ERROR;

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC.
        tc_handling_status = ReceiveTC(&tc);
        if(tc_handling_status == PUS_SUCCESSFUL)
        {
            // Then, we check the validity of the TC.
            tc_handling_status = CheckTCValidity(&tc, &acceptance_error);
            if(tc_handling_status ==  PUS_SUCCESSFUL)
            {
                // If TC is valid, we format the TC because of endianness.
                FormatTC(&tc);
                printf("Valid TC(%d,%d) arrived\n", tc.tc_header.service,tc.tc_header.subservice);
                // Then, we route the TC toward the task that will execute it.
                key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
                tc_handling_status = RouteSearch((pusRoutingTable_t *) &g_tc_routing_table, NB_ROUTES, key, &route);
                if(tc_handling_status ==  PUS_SUCCESSFUL)
                {
                    // Acknowledge TC
                    BuildS1SS1(&tc, &tm);
                    SendTMToBuffer(&tm, TM_PUS1);
                }
                else
                {
                    // Bad routing so TC non acknowleded
                    BuildS1SS2(&tc, &tm, PUS_ACCEPTANCE_INVALID_ROUTE);
                    SendTMToBuffer(&tm, TM_PUS1);
                }

            }
            else
            {
                // Invalid TC, TC will be non-acknowledged.
                printf("Invalid TC arrived\n");
                BuildS1SS2(&tc, &tm, acceptance_error);
                SendTMToBuffer(&tm, TM_PUS1);
            }
        }
        
        // We reset the TM & TC variables until next call;
        EraseTC(&tc);
        EraseTM(&tm);
        key = 0u;
        route = 0u;

        // Wait until next call of the task
        waitUntilNextPeriod(task_dyn_conf);
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
pusStatus_t ReceiveTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    halStatus_t read_status = FCT_SUCCESSFUL;

    // Function Core
    read_status = UartRead(&uart_tmtc_inst, (uartMsg_t *) tc, TC_MAX_SIZE);
    if(read_status != FCT_SUCCESSFUL)
    {
        return_value = PUS_NO_MSG;
    }

    return(return_value);
}

/**
 * @fn      SendTMToBuffer(pusTM_t *tm, bufferRef_t buffer_ref)
 * @brief   Function that send tm to its buffer
 * @param   tm Pointer to the TM we want to send
 * @param   buffer_ref Buffer where we want to put the TM.
 * @retval  #PUS_INVALID_PARAM if tm is invalid for buffer write
 * @retval  #PUS_ERROR buffer write has encountered an error, probably buffer is full.
 * @retval  #PUS_SUCCESSFUL else
 */
pusStatus_t SendTMToBuffer(pusTM_t *tm, bufferRef_t buffer_ref)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    bufferStatus_t buffer_status = BUFFER_SUCCESSFUL;

    // Function Core
    buffer_status = WriteBuffer(buffer_ref, (bufferMsgAddr_t) tm, TM_MAX_SIZE);
    if(buffer_status != BUFFER_SUCCESSFUL)
    {
        if(buffer_status == BUFFER_INVALID_PARAM)
        {
            return_value = PUS_INVALID_PARAM;
        }
        else
        {
            return_value = PUS_ERROR;
        }
    }

    return(return_value);
}