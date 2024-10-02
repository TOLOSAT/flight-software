/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tc_scheduler.h"
#include "kernel.h"
#include "pus.h"
#include "services/pus1.h"
#include "services/pus11.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS11_EXECUTION    4u  /**< Number of pus11 exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcSchedulerMain(void)
 * @brief           Main of the TC_SCHEDULER Task
 */
void TcSchedulerMain(void)
{
    // Initialisation
    uint32_t task_status;
    static pusExecutionTable_t sched_exec_tab[NB_PUS11_EXECUTION] =
    {
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 1u) , ExecuteS11SS1 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 2u) , ExecuteS11SS2 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 3u) , ExecuteS11SS3 , TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 4u) , ExecuteS11SS4 , TM_NOT_REQUESTED },
    };
    static pusExecutionContext_t sched_tc_context =
    {
        .execution_table = sched_exec_tab,
        .execution_table_size = NB_PUS11_EXECUTION,
        .buffer_tc = TC_PUS11,
        .buffer_tm = NO_BUFFER,
        .buffer_ack = TM_PUS1,
    };
    deviceNo_t dev_delayed_tc = 0u;

    task_status =  InitTCExecutionContext(&sched_tc_context);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPus11();
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_delayed_tc, DEVICE_TYPE_BUFFER, TC_DELAYED, DEVICE_NO_EXTRA_INFO);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC(&sched_tc_context);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Process delayed TC
        task_status = ProcessDelayedTC(dev_delayed_tc);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        SleepPeriodic();
    }
}
