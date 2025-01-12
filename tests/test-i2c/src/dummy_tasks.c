/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "dummy_tasks.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

#define I2C_MAX_MSG_SIZE    2u      /**< I2C max message size */
#define LM75_ADDR           0x90u   /**< LM75 slave address */
#define LM75_TMP_ADDR       0x00u   /**< LM75 temperature register address */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask(void)
{
    // Initialisation
    uint8_t i2c_msg[I2C_MAX_MSG_SIZE] = {0};
    deviceNo_t dev_i2c_lm75;
    const uint16_t lm75_addr = LM75_ADDR;
    uint32_t temperature = 0u;
    (void)DeviceOpen(&dev_i2c_lm75, DEVICE_TYPE_PERIPHERAL, I2C_AVIONIC);
    (void)DeviceIoctl(dev_i2c_lm75, IOCTL_I2C_SET_SLAVE_ADDRESS, (void *)&lm75_addr, sizeof(lm75_addr));
    LOG("Init dummy task\n");

    // Function Core
    while (1)
    {
        // Send hello periodically
        LOG("Hello\n");

        // Get LM75 temperature
        // First ask for temperature register
        i2c_msg[0] = LM75_TMP_ADDR;
        (void)DeviceIoctl(dev_i2c_lm75, IOCTL_PERIPHERAL_START_TX, i2c_msg, 1u);
        (void)DeviceIoctl(dev_i2c_lm75, IOCTL_PERIPHERAL_CHECK_TX_COMPLETED, NULL, 0u);

        (void)memset(&i2c_msg, 0u, I2C_MAX_MSG_SIZE);
        (void)DeviceIoctl(dev_i2c_lm75, IOCTL_PERIPHERAL_START_RX, i2c_msg, 2u);
        (void)DeviceIoctl(dev_i2c_lm75, IOCTL_PERIPHERAL_CHECK_RX_COMPLETED, NULL, 0u);
        temperature = i2c_msg[0];
        LOG_DECIMAL("Temperature : %d", temperature);

        SleepPeriodic();
    }
}