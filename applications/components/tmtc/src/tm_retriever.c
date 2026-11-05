/**
 * @file    tm_retriever.c
 * @author  Matteo Planchet, Arthur Morain
 * @brief   Source file for TM_RETRIEVER Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "tm_retriever.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS15_EXECUTION 8u /**< Number of pus15 exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc TmRetrieverMain
 */
void TmRetrieverMain(void)
{
    static pus15PacketStoreIndexInfo_t pus15_packet_store_info = {
        .by_time_retrieval_status = PUS15_BY_TIME_RETRIEVAL_DISABLED,
        .id                       = "MAIN",
        .length                   = PUS15_MAXIMUM_PACKET_STORE_LENGTH,
        .nb_entries               = 0u,
        .oldest_entry             = NULL,
        .status                   = PUS15_PACKET_STORE_ENABLED,
        .type                     = PUS15_PACKET_STORE_TYPE_CIRCULAR,
        .write_cursor             = 0u,
    };

    static pus15PacketStore_t pus15_packet_store = {
        .packet_store_info = &pus15_packet_store_info,
        .data              = { 0 },
    };

    static pus15Env_t pus15_env = {
        .status          = PUS15_ENABLE,
        .packet_store    = &pus15_packet_store,
        .dev_pus15_index = NULL,
        .dev_pus15_data  = NULL,
        .fil_pus15_index = PUS15_INDEX_FILE,
        .fil_pus15_data  = PUS15_DATA_FILE,
    };

    static pusExecutionTableEntry_t storage_exec_entries[NB_PUS15_EXECUTION] = {
        // PUS Service 15 : On-board storage and retrieval
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 1u),  ExecuteS15SS1,  TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 2u),  ExecuteS15SS2,  TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 9u),  ExecuteS15SS9,  TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 11u), ExecuteS15SS11, TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 17u), ExecuteS15SS17, TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 18u), ExecuteS15SS18, TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 26u), ExecuteS15SS26, TM_NOT_REQUESTED, &pus15_env },
        { BUILD_ROUTING_KEY(OBC_APID, 15u, 27u), ExecuteS15SS27, TM_NOT_REQUESTED, &pus15_env },
    };

    static pusExecutionContext_t storage_tc_context = {
        .execution_table      = {
            .size = NB_PUS15_EXECUTION,
            .entries = storage_exec_entries,
        },
        .buffer_tc            = TC_PUS15,
        .buffer_tm            = TM_PUS15,
        .buffer_ack           = TM_PUS1,
    };

    // Initialisation
    CheckError(InitS15(&pus15_env), SEVERITY_MEDIUM);
    CheckError(InitTCExecutionContext(&storage_tc_context), SEVERITY_MEDIUM);

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&sched_tc_context), SEVERITY_MEDIUM);

        // Wait for timer end
        if (wait_for_signal)
        {
            CheckError(WaitSignal(SIGNAL_TIMER_ENDED | SIGNAL_TC), SEVERITY_MEDIUM);
        }
    }
}
