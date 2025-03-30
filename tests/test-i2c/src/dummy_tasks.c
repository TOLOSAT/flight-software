/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "dummy_tasks.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

#define I2C_MAX_MSG_SIZE   2u    /**< I2C max message size */
#define LM75_ADDR          0x90u /**< LM75 slave address */
#define LM75_REG_ADDR_SIZE 1u    /**< LM75 reguster address size */
#define LM75_REG_TEMP      0x00u /**< LM75 temperature register address */
#define LM75_TEMP_SIZE     2u    /**< LM75 temperature size */

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
    deviceNo_t dev_i2c_lm75;
    const uint16_t lm75_addr          = LM75_ADDR;
    uint32_t temperature              = 0u;
    uint8_t i2c_msg[I2C_MAX_MSG_SIZE] = { 0 };
    (void)DeviceOpen(&dev_i2c_lm75, DEVICE_TYPE_PERIPHERAL, I2C_AVIONIC);
    (void)DeviceIoctl(dev_i2c_lm75, IOCTL_I2C_SET_SLAVE_ADDR, (void *)&lm75_addr, sizeof(lm75_addr));
    LOG("Init dummy task\n");

    // Task Core
    while (1)
    {
        // Send hello periodically
        LOG("Hello\n");

        // Get LM75 temperature
        i2c_msg[0] = LM75_REG_TEMP;
        (void)DeviceWrite(dev_i2c_lm75, i2c_msg, LM75_REG_ADDR_SIZE);

        (void)memset(&i2c_msg, 0u, I2C_MAX_MSG_SIZE);
        (void)DeviceRead(dev_i2c_lm75, i2c_msg, LM75_TEMP_SIZE);
        temperature = i2c_msg[0];
        LOG_DECIMAL("Temperature : %d", temperature);

        SleepPeriodic();
    }
}