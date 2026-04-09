/**
 * @file    thermal.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for THERMAL Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "thermal.h"
#include "kernel.h"
#include "pus.h"
#include "service/pus178.h"
#include "drv/thermal_driver.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     DS18B20_ROM_CODE_1
 * @brief   Temperature sensor1 ROM code
 */
#define DS18B20_ROM_CODE_1  { 0x10, 0x23, 0xC7, 0x6A, 0x03, 0x08, 0x00, 0x7E }

/**
 * @def     DS18B20_ROM_CODE_2
 * @brief   Temperature sensor1 ROM code
 */
#define DS18B20_ROM_CODE_2  { 0x10, 0x33, 0xE8, 0x6A, 0x03, 0x08, 0x00, 0xB2 }

#define BC_SENSOR_SZ        2u /**< Number temperature sensor */
#define NB_PUS178_EXECUTION 2u /**< Number of execution functions */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              ThermalMain(void)
 * @brief           Main of the thermal Task
 */
void ThermalMain(void)
{
    static temSensorInfo_t temp_sensors[BC_SENSOR_SZ] = {
        { .temp_sensor_rom_code = DS18B20_ROM_CODE_1, .temp_sensor_model = DS18S20_MODEL },
        { .temp_sensor_rom_code = DS18B20_ROM_CODE_2, .temp_sensor_model = DS18S20_MODEL },
    };

    static temSensorContext_t thermal_context = {
        .temp_sensors         = temp_sensors,
        .temp_sensor_count    = BC_SENSOR_SZ,
        .ow_device            = 0,
        .peripheral           = PERIPH_OW1,
        .ow_device_init_state = INIT_NOT_DONE,
    };

    static pus178Env_t pus178_env = { .p_thermal_context = &thermal_context, .status = PUS_NOT_INITIALIZED };

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

    // Initialisation
    CheckError(InitS178(&pus178_env), SEVERITY_MEDIUM);
    CheckError(InitTCExecutionContext(&pus178_tc_context), SEVERITY_MEDIUM);

    // Task Core
    while (1)
    {
        // Execute incoming TC
        CheckError(ExecuteTC(&pus178_tc_context), SEVERITY_MEDIUM);

        SleepPeriodic();
    }
}