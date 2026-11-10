/**
 * @file    aocs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for AOCS Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "aocs.h"
#include "kernel.h"
#include "system_conf.h"
#include "drivers/mlx90399.h"

/***************************** Macros Definitions ****************************/

#define CS_LOW DeviceWrite(mag_cs, (data_t)&pin_reset, 1u)
#define CS_HIGH DeviceWrite(mag_cs, (data_t)&pin_set, 1u)

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @copydoc AocsMain
 */
void AocsMain(void)
{
    deviceNo_t mag_cs = 0u;
    deviceNo_t mag_spi = 0u;

    uint8_t mag_tx_data_r[2u] = { 0b01010000, 0x0A << 2 };
    uint8_t mag_tx_data_w[4u] = { 0b01100000, 0xCA, 0xFE, 0x0A << 2 };
    uint8_t mag_rx_data[3u] = { 0x00, 0x00, 0x00 };
    uint8_t pin_reset = 0u;
    uint8_t pin_set = 1u;

    // Initialisation
    DeviceOpen(&mag_cs, DEVICE_TYPE_PERIPHERAL, MAGNETOMETER_CS);
    DeviceOpen(&mag_spi, DEVICE_TYPE_PERIPHERAL, PERIPH_SPI1);

    // Task Core
    while (1)
    {
        // ConsolePrint("AOCS Task is running...\n");
        CS_LOW;
            DeviceWrite(mag_spi, (data_t)mag_tx_data_w, 4u);
        CS_HIGH;

        CS_LOW;
            DeviceWrite(mag_spi, (data_t)mag_tx_data_r, 2u);
            DeviceRead(mag_spi, (data_t)mag_rx_data, 3u);
        CS_HIGH;

        ConsolePrint("AOCS Task received: %X %X\n", mag_rx_data[1], mag_rx_data[2]);

        SleepPeriodic();
    }
    while (1)
    {
        ConsolePrint("AOCS Task is running...\n");
        SleepPeriodic();
    }
}
