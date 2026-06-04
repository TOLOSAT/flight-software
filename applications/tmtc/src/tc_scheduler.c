/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "tc_scheduler.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

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
    time_t next_tc_release_date = INVALID_TIME;

    static pus11Env_t pus11_env = {
        .pus11_status       = PUS11_ENABLE,
        .buffer_delayed_tc  = TC_DELAYED,
        .fil_pus11_schedule = PUS11_SCHED_FILE,
        .fil_pus11_data     = PUS11_DATA_FILE,
    };

    static pusExecutionTableEntry_t sched_exec_entries[NB_PUS11_EXECUTION] = {
        // PUS Service 11 : Time Based Scheduling
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 1u), ExecuteS11SS1, TM_NOT_REQUESTED, &pus11_env },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 2u), ExecuteS11SS2, TM_NOT_REQUESTED, &pus11_env },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 3u), ExecuteS11SS3, TM_NOT_REQUESTED, &pus11_env },
        { BUILD_ROUTING_KEY(OBC_APID, 11u, 4u), ExecuteS11SS4, TM_NOT_REQUESTED, &pus11_env },
    };

    static pusExecutionContext_t sched_tc_context = {
        .execution_table      =
        {
            .size = NB_PUS11_EXECUTION,
            .entries = sched_exec_entries,
        },
        .buffer_tc            = TC_PUS11,
        .buffer_tm            = NO_BUFFER,
        .buffer_ack           = TM_PUS1,
    };

    uint32_t kernel_clock_freq_hz = GetTickFreq();

    // Initialisation
    CheckError(InitS11(&pus11_env), SEVERITY_MEDIUM);
    CheckError(InitTCExecutionContext(&sched_tc_context), SEVERITY_MEDIUM);

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&sched_tc_context), SEVERITY_MEDIUM);

        // Process delayed TC
        CheckError(TryReleaseDelayedTC(&pus11_env, &next_tc_release_date), SEVERITY_MEDIUM);

        // If delayed TC is available
        if (next_tc_release_date != INVALID_TIME)
        {
            // Get current time
            time_t current_time = GetTime();

            // Set timer until next TC release date
            if (current_time < next_tc_release_date)
            {
                tick_t delay = CUC_TO_TICK(next_tc_release_date - current_time, kernel_clock_freq_hz);
                CheckError(SetTimer(PUS11_TIMER, delay, TIMER_ONESHOT), SEVERITY_MEDIUM);
            }
            else
            {
                CheckError(SetTimer(PUS11_TIMER, 0u, TIMER_ONESHOT), SEVERITY_MEDIUM);
            }

            // Then start the timer
            CheckError(StartTimer(PUS11_TIMER), SEVERITY_MEDIUM);
        }

        // Wait for timer end
        CheckError(WaitSignal(SIGNAL_TIMER_ENDED | SIGNAL_TC), SEVERITY_MEDIUM);
    }
}
