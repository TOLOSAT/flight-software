/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tmtc/tc_scheduler.h"
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

    CheckError(InitTCExecutionContext(&sched_tc_context));
    CheckError(InitPus11());
    CheckError(DeviceOpen(&dev_delayed_tc, DEVICE_TYPE_BUFFER, TC_DELAYED, DEVICE_NO_EXTRA_INFO)); // TO DO : init with PUS11

    // Function Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&sched_tc_context));

        // Process delayed TC
        CheckError(ProcessDelayedTC(dev_delayed_tc));

        SleepPeriodic();
    }
}
