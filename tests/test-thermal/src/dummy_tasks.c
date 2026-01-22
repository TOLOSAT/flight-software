/**
 * @file    dummy_tasks.c
 * @author  Louis
 * @author  Aldo
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
#define DS18B20_ROM_CODE_2 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 }

// UNICAST PARAMETERS
#define UC_SENSOR_SEL      1

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

temSensorInfo_t tempSensors[BC_SENSOR_SZ] = {
    { /* DS18B20 ROM 1 */
      .temp_sensor_rom_code = DS18B20_ROM_CODE_1,
     .temp_sensor_model    = DS18B20_MODEL },
    { /* DS18S20 ROM 2 */
      .temp_sensor_rom_code = DS18B20_ROM_CODE_2,
     .temp_sensor_model    = DS18S20_MODEL }
};

/* --- Temperature sensor context instance --- */

temSensorContext_t sensorContext = {
    .temp_sensors         = tempSensors,  /* Pointer to sensor array */
    .temp_sensor_count    = BC_SENSOR_SZ, /* Number of sensors */
    .ow_device            = 0,            /* OneWire device */
    .peripheral           = PERIPH_OW1,   /* Example peripheral base address */
    .ow_device_init_state = INIT_NOT_DONE /* OneWire already initialized */
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
    int16_t raw_temperatures[BC_SENSOR_SZ];
    float temperatures[BC_SENSOR_SZ];

    // Initialisation of temperature sensors
    sensor_state_return = DS18B20Init(&sensorContext);
    if (sensor_state_return != RET_SUCCESSFUL)
    {
        LOG("RETURNED ERROR! \n");
    }

    // Task Core
    while (1)
    {
        LOG("Starting Thermal Test Task... \n");

#if THERMAL_TEST_MODE == 0 /* Broadcast */

        LOG("Test 0 Selected: \n");

        // Request broadcast temperature
        LOG("Broadcast Meas Started: \n");
        sensor_state_return = DS18B20StartMeasurementBroadcast(&sensorContext);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            LOG("RETURNED ERROR! \n");
        }

        // Wait 5s
        LOG("Waiting... \n");
        Sleep(5000);

        // Read broadcast temperature
        LOG("Broadcast Read Started: \n");
        sensor_state_return = DS18B20ReadTemperaturesBroadcast(&sensorContext, raw_temperatures, sensorContext.temp_sensor_count);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            LOG("RETURNED ERROR! \n");
        }

        // Loop to print broadcast temp values
        for (uint8_t i = 0; i < BC_SENSOR_SZ; i++)
        {
            LOG_DECIMAL("Temperature = %d C\n", (int)temperatures[i]);
        }

#elif THERMAL_TEST_MODE == 1 /* Unicast */

        LOG("Test 1 Selected: \n");

        // Ask for individual sensor temperature
        LOG("Unicast Meas Started: \n");
        sensor_state_return = DS18B20StartMeasurement(&sensorContext, UC_SENSOR_SEL);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            LOG("RETURNED ERROR! \n");
        }

        // Wait 1s
        LOG("Waiting... \n");
        Sleep(1000);

        // Read temperature of selected sensor
        LOG("Unicast Meas Started: \n");
        sensor_state_return = DS18B20ReadTemperature(&sensorContext, UC_SENSOR_SEL, &raw_temperatures[UC_SENSOR_SEL]);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            LOG("RETURNED ERROR! \n");
        }

        // Convert raw to float
        LOG("Unicast Raw Conv Started: \n");
        sensor_state_return = DS18B20ConvertRawToFloat(sensorContext.temp_sensors[UC_SENSOR_SEL].temp_sensor_model, raw_temperatures[UC_SENSOR_SEL],
                                                       &temperatures[UC_SENSOR_SEL]);
        if (sensor_state_return != RET_SUCCESSFUL)
        {
            LOG("RETURNED ERROR! \n");
        }

        // Print on screen obtained temperature
        LOG_DECIMAL("Temperature = %d C\n", (int)temperatures[UC_SENSOR_SEL]);

#else
        LOG("Test Selected Not Valid: \n");

#endif
        LOG("Tests Completed: \n");
        SleepPeriodic();
    }
}
