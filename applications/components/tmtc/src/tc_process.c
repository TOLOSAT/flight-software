/**
 * @file    tc_process.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_PROCESS Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "tc_process.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

#define NB_NORMAL_EXECUTION 13u /**< Number of execution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc TcProcessMain
 */
void TcProcessMain(void)
{
    static pus160Env_t pus160_env = {
        .nb_tasks = NB_TASKS,
    };

    static pusExecutionTableEntry_t normal_exec_entries[NB_NORMAL_EXECUTION] = {
        // PUS Service 6 : Memory management
        { BUILD_ROUTING_KEY(OBC_APID, 6u,   1u),   ExecuteS6SS1,    TM_NOT_REQUESTED, NULL        },
        { BUILD_ROUTING_KEY(OBC_APID, 6u,   3u),   ExecuteS6SS3,    TM_REQUESTED,     NULL        },
        // PUS Service 9 : Time Management
        { BUILD_ROUTING_KEY(OBC_APID, 9u,   128u), ExecuteS9SS128,  TM_NOT_REQUESTED, NULL        },
        // PUS Service 17 : Test
        { BUILD_ROUTING_KEY(OBC_APID, 17u,  1u),   ExecuteS17SS1,   TM_REQUESTED,     NULL        },
        // PUS Service 160 : System PUS Service
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 1u),   ExecuteS160SS1,  TM_NOT_REQUESTED, &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 2u),   ExecuteS160SS2,  TM_NOT_REQUESTED, &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 17u),  ExecuteS160SS17, TM_REQUESTED,     &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 19u),  ExecuteS160SS19, TM_REQUESTED,     &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 21u),  ExecuteS160SS21, TM_REQUESTED,     &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 23u),  ExecuteS160SS23, TM_NOT_REQUESTED, &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 33u),  ExecuteS160SS33, TM_REQUESTED,     &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 35u),  ExecuteS160SS35, TM_REQUESTED,     &pus160_env },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 37u),  ExecuteS160SS37, TM_REQUESTED,     &pus160_env },
    };

    static pusExecutionContext_t normal_tc_context = {
        .execution_table      =
        {
            .size = NB_NORMAL_EXECUTION,
            .entries = normal_exec_entries,
        },
        .buffer_tc            = TC_NORMAL,
        .buffer_tm            = TM_NORMAL,
        .buffer_ack           = TM_PUS1,
    };

    // Initialisation
    CheckError(InitS160(&pus160_env), SEVERITY_MEDIUM);
    CheckError(InitTCExecutionContext(&normal_tc_context), SEVERITY_MEDIUM);

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&normal_tc_context), SEVERITY_MEDIUM);

        CheckError(WaitSignal(SIGNAL_TC), SEVERITY_MEDIUM);
    }
}
