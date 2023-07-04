/**
 * @file tc_receiver.c
 * @author Merlin Kooshmanian
 * @brief Source file for TC_RECEIVER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tm_tc/tc_receiver.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "pus_tools/tc_management.h"

/************************** Constant Definitions *****************************/

#define TASK_NAME       "TC_RECEIVER"       /**< Current Task Name */
#define TASK_ID         (TC_RECEIVER_TASK)  /**< Current Task ID */

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

pusStatus_t ReceiveTC(pusTC_t *tc);

/************************** Variable Definitions *****************************/

extern uartInst_t uart_tmtc_inst;

/************************* Functions Definitions *****************************/

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
                // tc_handling_status = RouteTC(&tc, g_tc_receiver_routing_table, &acceptance_error);
                // if(tc_handling_status ==  PUS_SUCCESSFUL)
                // {
                //     // Acknowledge TC.
                //     SerializeS1SS1(&tc, TM_PUS1);
                // }
                // else
                // {
                //     // Bad routing so TC nin acknowleded
                //     SerializeS1SS2(&tc, TM_PUS1, acceptance_error);
                // }

            }
            else
            {
                // Invalid TC, TC will be non-acknowledged.
                printf("Invalid TC arrived\n");
                // SerializeS1SS2(&tc, TM_PUS1, acceptance_error);
            }
        }
        
        // We reset the TC variable until next call;
        EraseTC(&tc);

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
 * @retval  PUS_NO_MSG if there is no TC available
 * @retval  PUS_SUCCESSFUL else
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