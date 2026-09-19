/**
 * @file    tmtc_hk.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TMTC HK Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "tmtc_hk.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

#define NB_HK             1u /**< Number of HKs */

#define NB_PUS3_EXECUTION 4u /**< Number of pus3 exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc TmTcHkMain
 */
void TmTcHkMain(void)
{
    static uint32_t test_hktm = 0u;

    static pus3HKParam_t hk_param_table[NB_HK] = {
        { .hkid = 0x01, .status = HK_REPORT_DISABLE, .collection_rate = 10u, .p_addr = (void *)&test_hktm, .size = sizeof(test_hktm) },
    };

    static pus3Env_t pus3_env = {
        .hk_table    = { .size = NB_HK, .entries = hk_param_table },
        .buffer_hktm = TM_PUS3,
    };

    static pusExecutionTableEntry_t hk_exec_entries[NB_PUS3_EXECUTION] = {
        // PUS Service 3 : Housekeeping
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 5u),  ExecuteS3SS5,  TM_NOT_REQUESTED, &pus3_env },
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 6u),  ExecuteS3SS6,  TM_NOT_REQUESTED, &pus3_env },
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 9u),  ExecuteS3SS9,  TM_REQUESTED,     &pus3_env },
        { BUILD_ROUTING_KEY(OBC_APID, 3u, 31u), ExecuteS3SS31, TM_NOT_REQUESTED, &pus3_env },
    };

    static pusExecutionContext_t hk_tc_context = {
        .execution_table      =
        {
            .size = NB_PUS3_EXECUTION,
            .entries = hk_exec_entries,
        },
        .buffer_tc            = TC_PUS3,
        .buffer_tm            = TM_PUS3,
        .buffer_ack           = TM_PUS1,
    };

    // Initialisation
    CheckError(InitS3(&pus3_env), SEVERITY_MEDIUM);
    CheckError(InitTCExecutionContext(&hk_tc_context), SEVERITY_MEDIUM);

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&hk_tc_context), SEVERITY_MEDIUM);

        // Emits HKs
        CheckError(EmitHKs(&pus3_env), SEVERITY_MEDIUM);

        // Increment test
        test_hktm++;

        SleepPeriodic();
    }
}
