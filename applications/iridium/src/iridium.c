/**
 * @file    iridium.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for IRIDIUM Task
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "iridium.h"
#include "kernel.h"
#include "pus.h"
#include "service/pus193.h"
#include "drv/iridium_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

#define NB_PUS193_EXECUTION 4u /**< Number of execution functions */

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumMain(void)
 * @brief           Main of the IRIDIUM Task
 */
void IridiumMain(void)
{
    // Initialisation
    static pusExecutionTable_t pus193_exec_tab[NB_PUS193_EXECUTION] = {
        { BUILD_ROUTING_KEY(OBC_APID, 193u, 1u), ExecuteS193SS1, TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 193u, 2u), ExecuteS193SS2, TM_NOT_REQUESTED },
        { BUILD_ROUTING_KEY(OBC_APID, 193u, 3u), ExecuteS193SS3, TM_REQUESTED     },
        { BUILD_ROUTING_KEY(OBC_APID, 193u, 5u), ExecuteS193SS5, TM_NOT_REQUESTED },
    };
    static pusExecutionContext_t pus193_tc_context = {
        .execution_table      = pus193_exec_tab,
        .execution_table_size = NB_PUS193_EXECUTION,
        .buffer_tc            = TC_PUS193,
        .buffer_tm            = TM_PUS193,
        .buffer_ack           = TM_PUS193,
    };
    CheckError(InitTCExecutionContext(&pus193_tc_context));

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&pus193_tc_context));

        SleepPeriodic();
    }
}