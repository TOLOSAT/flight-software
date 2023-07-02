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
    halStatus_t tc_handling_status;
    pusTC_t tc = {0};

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC, if yes ReceiveTC will format it the right way.
        tc_handling_status = ReceiveTC(&tc);
        printf("Status : %d\n", tc_handling_status);
        if(tc_handling_status == PUS_SUCCESSFUL)
        {
        //     // Second, we check the validity of the TC.
        //     tc_handling_status = CheckTCValidity(&tc);
        //     // Depending on the validity of the TC, its acceptance or rejection is acknowledged.
        //     if(tc_handling_status == PUS_SUCCESSFUL)
        //     {
        //         SerializeS1SS1(&tc, TM_PUS1);
        //         // Finally we route the TC to the right task.
        //         RouteTC(&tc, g_tc_receiver_routing_table);
        //     }
        //     else
        //     {
        //         SerializeS1SS2(&tc, TM_PUS1);
        //     }
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
    if(read_status == FCT_SUCCESSFUL)
    {
        FormatTC(tc);
    }
    else
    {
        return_value = PUS_NO_MSG;
    }

    return(return_value);
}