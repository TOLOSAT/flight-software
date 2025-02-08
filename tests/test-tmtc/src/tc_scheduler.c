/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tc_scheduler.h"
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS11_EXECUTION 4u /**< Number of pus11 exution functions */

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
    time_t next_tc_release_date                                   = 0u;
    static pusExecutionTable_t sched_exec_tab[NB_PUS11_EXECUTION] =
    {
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 1u), ExecuteS11SS1, TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 2u), ExecuteS11SS2, TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 3u), ExecuteS11SS3, TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 4u), ExecuteS11SS4, TM_NOT_REQUESTED },
    };
    static pusExecutionContext_t sched_tc_context =
    {
        .execution_table      = sched_exec_tab,
        .execution_table_size = NB_PUS11_EXECUTION,
        .buffer_tc            = TC_PUS11,
        .buffer_tm            = NO_BUFFER,
        .buffer_ack           = TM_PUS1,
    };
    static pus11Context_t pus11_context =
    {
        .pus11_status       = PUS11_ENABLE,
        .buffer_delayed_tc  = TC_DELAYED,
        .fil_pus11_schedule = PUS11_SCHED_FILE,
        .fil_pus11_data     = PUS11_DATA_FILE,
    };

    CheckError(InitTCExecutionContext(&sched_tc_context));
    CheckError(InitPus11(&pus11_context));

    // Function Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&sched_tc_context));

        // Process delayed TC
        CheckError(ReleaseDelayedTC(&pus11_context, &next_tc_release_date));

        SleepPeriodic();
    }
}
