/**
 * @file    miso.c
 * @author  Merlin Kooshmanian
 * @author  Clement Cognard
 * @author  Yann Awbi
 * @brief   Source file for MISO Task
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "miso.h"
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS161_EXECUTION     3u                       /**< Number of pus161 exution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              MisoMain(void)
 * @brief           Main of the MISO Task
 */
void MisoMain(void)
{
    // Initialisation
    static pusExecutionTable_t miso_exec_tab[NB_PUS161_EXECUTION] =
    {
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 1u), ExecuteS161SS1, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 3u), ExecuteS161SS3, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 5u), ExecuteS161SS5, TM_REQUESTED},
    };
    static pusExecutionContext_t miso_tc_context =
    {
        .execution_table = miso_exec_tab,
        .execution_table_size = NB_PUS161_EXECUTION,
        .buffer_tc = TC_PUS161,
        .buffer_tm = TM_PUS161,
        .buffer_ack = TM_PUS1,
    };
    CheckError(InitTCExecutionContext(&miso_tc_context));
    CheckError(InitS161());

    // Function Core
    while (1)
    {
        // Executes a TC.
        CheckError(ExecuteTC(&miso_tc_context));

        // 1. Get some valuable data to generate an event
        // 2. Generate Event
        //
        // Idea :
        // - Add more events level (Medium Severity & High Severity ??)
        // - Generate event (message -> CARNE -> PUS)
        // - Generate message to SALAMI (if high severity event)
        // - Generate TM (if not severe only)

        SleepPeriodic();
    }
}
