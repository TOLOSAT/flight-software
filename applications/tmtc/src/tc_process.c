/**
 * @file    tc_process.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_PROCESS Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "tc_process.h"
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_NORMAL_EXECUTION 12u /**< Number of execution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcProcessMain(void)
 * @brief           Main of the TC_PROCESS Task
 */
void TcProcessMain(void)
{
    // Initialisation
    static pusExecutionTable_t normal_exec_tab[NB_NORMAL_EXECUTION] = {
        { BUILD_ROUTING_KEY(OBC_APID, 6u,   1u),   ExecuteS6SS1,    TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 6u,   3u),   ExecuteS6SS3,    TM_REQUESTED     },
        { BUILD_ROUTING_KEY(OBC_APID, 9u,   128u), ExecuteS9SS128,  TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 17u,  1u),   ExecuteS17SS1,   TM_REQUESTED     },

        { BUILD_ROUTING_KEY(OBC_APID, 160u, 1u),   ExecuteS160SS1,  TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 2u),   ExecuteS160SS2,  TM_NOT_REQUESTED },

        { BUILD_ROUTING_KEY(OBC_APID, 160u, 17u),  ExecuteS160SS17, TM_REQUESTED     },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 19u),  ExecuteS160SS19, TM_REQUESTED     },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 21u),  ExecuteS160SS21, TM_REQUESTED     },

        { BUILD_ROUTING_KEY(OBC_APID, 160u, 33u),  ExecuteS160SS33, TM_REQUESTED     },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 35u),  ExecuteS160SS35, TM_REQUESTED     },
        { BUILD_ROUTING_KEY(OBC_APID, 160u, 37u),  ExecuteS160SS37, TM_REQUESTED     },
    };
    static pusExecutionContext_t normal_tc_context = {
        .execution_table      = normal_exec_tab,
        .execution_table_size = NB_NORMAL_EXECUTION,
        .buffer_tc            = TC_NORMAL,
        .buffer_tm            = TM_NORMAL,
        .buffer_ack           = TM_PUS1,
    };
    CheckError(InitTCExecutionContext(&normal_tc_context));
    CheckError(InitS160());

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&normal_tc_context));

        CheckError(WaitSignal(SIGNAL_TC));
    }
}
