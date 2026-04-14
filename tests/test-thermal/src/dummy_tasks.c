/**
 * @file    dummy_tasks.c
 * @author  Louis Remacle
 * @author  Aldo Lupio
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "dummy_tasks.h"
#include "kernel.h"
#include "system_conf.h"
#include "peripherals_conf.h"

/***************************** Macros Definitions ****************************/

// THERMAL TEST SELECTION ->  0: BROADCAST, 1: UNICAST
#define THERMAL_TEST_MODE  1

// BROADCAST PARAMETERS
#define BC_SENSOR_SZ       2u
#define DS18B20_ROM_CODE_1 { 0x10, 0x23, 0xC7, 0x6A, 0x03, 0x08, 0x00, 0x7E }
#define DS18B20_ROM_CODE_2 { 0x10, 0x33, 0xE8, 0x6A, 0x03, 0x08, 0x00, 0xB2 }

// UNICAST PARAMETERS
#define UC_SENSOR_SEL      1

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

ds18Info_t ds18_info[BC_SENSOR_SZ] = {
    { /* DS18B20 ROM 1 */
      .ds18_rom_code = DS18B20_ROM_CODE_1,
     .ds18_model    = DS18S20 },
    { /* DS18S20 ROM 2 */
      .ds18_rom_code = DS18B20_ROM_CODE_2,
     .ds18_model    = DS18S20 }
};

/* --- Temperature sensor context instance --- */

ds18Context_t ds18_context = {
    .ds18_info            = ds18_info,
    .ds18_count           = BC_SENSOR_SZ,
    .ow_device            = 0,
    .peripheral           = PERIPH_OW1,
    .ow_device_init_state = DS18_INIT_NOT_DONE,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Test tasks for the thermal library functions
 */
void DummyMainTask(void)
{
    // Return error state for DummyMainTask OW transactions
    returnCode_t sensor_state_return = RET_SUCCESSFUL;

    // Temperature variables for UniCast and BroadCast
    ds18RawTemperature_t raw_temp[BC_SENSOR_SZ] = { 0 };
    float float_temp[BC_SENSOR_SZ]              = { 0 };

    // Initialisation of temperature sensors
    sensor_state_return = DS18Init(&ds18_context);
    if (sensor_state_return != RET_SUCCESSFUL)
    {
        ConsolePrint("RETURNED ERROR! \n");
    }

    // Task Core
    while (1)
    {
        ConsolePrint("Starting Thermal Test Task... \n");

#if THERMAL_TEST_MODE == 0 /* Broadcast */

        ConsolePrint("Test 0 Selected: \n");

        // Request broadcast temperature
        ConsolePrint("Broadcast Meas Started: \n");
        sensor_state_return = DS18StartMeasurementBroadcast(&ds18_context);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            ConsolePrint("RETURNED ERROR! \n");
        }

        // Wait 5s
        ConsolePrint("Waiting... \n");
        Sleep(500);

        // Read broadcast temperature
        ConsolePrint("Broadcast Read Started: \n");
        sensor_state_return = DS18ReadTemperaturesBroadcast(&ds18_context, raw_temp, ds18_context.ds18_count);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            ConsolePrint("RETURNED ERROR! \n");
        }

        // Loop to print broadcast temp values
        for (uint8_t i = 0; i < BC_SENSOR_SZ; i++)
        {
            sensor_state_return = DS18ConvertRawToFloat((&ds18_context)->ds18_info[i].ds18_model, raw_temp[i], &float_temp[i]);
            ConsolePrint("Temperature = %d C\n", (int)float_temp[i]);
        }

#elif THERMAL_TEST_MODE == 1 /* Unicast */

        ConsolePrint("Test 1 Selected: \n");

        // Ask for individual sensor temperature
        ConsolePrint("Unicast Meas Started: \n");
        sensor_state_return = DS18StartMeasurement(&ds18_context, UC_SENSOR_SEL);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            ConsolePrint("RETURNED ERROR! \n");
        }

        // Wait 1s
        ConsolePrint("Waiting... \n");
        Sleep(500);

        // Read temperature of selected sensor
        ConsolePrint("Unicast Meas Started: \n");
        sensor_state_return = DS18ReadTemperature(&ds18_context, UC_SENSOR_SEL, &raw_temp[UC_SENSOR_SEL]);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            ConsolePrint("RETURNED ERROR! \n");
        }

        // Convert raw to float
        ConsolePrint("Unicast Raw Conv Started: \n");
        sensor_state_return =
            DS18ConvertRawToFloat(ds18_context.ds18_info[UC_SENSOR_SEL].ds18_model, raw_temp[UC_SENSOR_SEL], &float_temp[UC_SENSOR_SEL]);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            ConsolePrint("RETURNED ERROR! \n");
        }

        // Print on screen obtained temperature
        ConsolePrint("Temperature = %d C\n", (int)raw_temp[UC_SENSOR_SEL]);

#else
        ConsolePrint("Test Selected Not Valid: \n");

#endif
        ConsolePrint("Tests Completed: \n");
        SleepPeriodic();
    }
}
