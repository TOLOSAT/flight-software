/**
 * @file    thermal.c
 * @author  Merlin Kooshmanian
 * @author  Aldo Lupio
 * @brief   Source file for THERMAL Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "thermal.h"
#include "kernel.h"
#include "pus.h"
#include "service/pus178.h"
#include "drv/ds18_drv.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     DS18_ROM_CODE_1
 * @brief   Temperature sensor #1 (DS18S20-0x10) ROM code
 */
#define DS18_ROM_CODE_1     { 0x10, 0x23, 0xC7, 0x6A, 0x03, 0x08, 0x00, 0x7E }

/**
 * @def     DS18_ROM_CODE_2
 * @brief   Temperature sensor #2 (DS18S20-0x10) ROM code
 */
#define DS18_ROM_CODE_2     { 0x10, 0x33, 0xE8, 0x6A, 0x03, 0x08, 0x00, 0xB2 }

#define NB_DS18_ONBOARD     2u /**< Number of (ds18) temperature sensor onboard */
#define NB_PUS178_EXECUTION 2u /**< Number of PUS178 execution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              ThermalMain(void)
 * @brief           Main of the thermal Task
 */
void ThermalMain(void)
{
    static ds18Info_t ds18_info[NB_DS18_ONBOARD] = {
        { .ds18_rom_code = DS18_ROM_CODE_1, .ds18_model = DS18S20 },
        { .ds18_rom_code = DS18_ROM_CODE_2, .ds18_model = DS18S20 },
    };

    static ds18Context_t ds18_context = {
        .ds18_info            = ds18_info,
        .ds18_count           = NB_DS18_ONBOARD,
        .ow_device            = 0,
        .peripheral           = PERIPH_OW1,
        .ow_device_init_state = DS18_INIT_NOT_DONE,
    };

    static pus178Env_t pus178_env = { .p_ds18_context = &ds18_context, .status = PUS_NOT_INITIALIZED };

    static pusExecutionTableEntry_t pus178_exec_entries[NB_PUS178_EXECUTION] = {
        { BUILD_ROUTING_KEY(OBC_APID, 178u, 1u), ExecuteS178SS1, TM_REQUESTED, &pus178_env },
        { BUILD_ROUTING_KEY(OBC_APID, 178u, 3u), ExecuteS178SS3, TM_REQUESTED, &pus178_env },
    };

    static pusExecutionContext_t pus178_tc_context = {
        .execution_table      =
        {
            .size = NB_PUS178_EXECUTION,
            .entries = pus178_exec_entries,
        },
        .buffer_tc            = TC_PUS178,
        .buffer_tm            = TM_PUS178,
        .buffer_ack           = TM_PUS1,
    };

    // Sequential initialisation of ds18 driver and pus178
    CheckError(DS18Init(&ds18_context), SEVERITY_MEDIUM);
    CheckError(InitS178(&pus178_env), SEVERITY_MEDIUM);
    CheckError(InitTCExecutionContext(&pus178_tc_context), SEVERITY_MEDIUM);

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&pus178_tc_context), SEVERITY_MEDIUM);

        CheckError(WaitSignal(SIGNAL_TC), SEVERITY_MEDIUM);
    }
}