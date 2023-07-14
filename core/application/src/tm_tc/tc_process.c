/**
 * @file tc_process.c
 * @author Merlin Kooshmanian
 * @brief Source file for TC_PROCESS Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tm_tc/tc_process.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "pus_tools/tc_management.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/tables_management.h"
#include "services/pus1.h"
#include "services/pus17.h"

/***************************** Macros Definitions ****************************/

#define TASK_NAME       "TC_PROCESS"        /**< Current Task Name */
#define TASK_ID         (TC_PROCESS_TASK)   /**< Current Task ID */
#define NB_EXECUTION    1u                  /**< Number of exution functions */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

pusExecutionTable_t g_tc_execution_table[NB_EXECUTION] = 
{
    {.key = BUILD_ROUTING_KEY(OBC_APID, 17u, 1u) , ExecuteS17SS1},
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      TcProcessMain(void *task_dyn_conf)
 * @brief   Main of the TC_PROCESS Task
 * @param   task_dyn_conf Status of the current task
 */
void TcProcessMain(void *task_dyn_conf)
{
    // Variable Initialisation
    pusStatus_t tc_handling_status;
    bufferStatus_t buffer_status;
    uint32_t key;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    pusTM_t execution_tm = {0};
    pusExecutionFunctionPtr_t ExecutionFunction;

    // Initialisation
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC.
        buffer_status = ReadBuffer(TC_NORMAL, (bufferMsgAddr_t) &tc, TC_MAX_SIZE);
        if(buffer_status == BUFFER_SUCCESSFUL)
        {
            // Then, we find which TC we have to execute
            key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
            tc_handling_status = ExecutionSearch((pusExecutionTable_t *) &g_tc_execution_table, NB_EXECUTION, key, &ExecutionFunction);
            if(tc_handling_status ==  PUS_SUCCESSFUL)
            {
                // Now we execute the TC
                tc_handling_status = ExecutionFunction(&tc, &tm);
                if(tc_handling_status == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC execution
                    BuildS1SS7(&tc, &execution_tm);
                    WriteBuffer(TM_PUS1, (bufferMsgAddr_t) &execution_tm, TM_MAX_SIZE);
                    WriteBuffer(TM_NORMAL, (bufferMsgAddr_t) &tm, TM_MAX_SIZE);
                }
                else
                {
                    // TC Failed to be executed
                    BuildS1SS8(&tc, &execution_tm, PUS_EXECUTION_FAILED);
                    WriteBuffer(TM_PUS1, (bufferMsgAddr_t) &execution_tm, TM_MAX_SIZE);
                }
            }
            else
            {
                // TC does not have execution procedure
                BuildS1SS8(&tc, &execution_tm, PUS_EXECUTION_UNAVAILABLE);
                WriteBuffer(TM_PUS1, (bufferMsgAddr_t) &execution_tm, TM_MAX_SIZE);
            }
        }
        // We reset the TM & TC variables until next call;
        EraseTC(&tc);
        EraseTM(&execution_tm);

        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}