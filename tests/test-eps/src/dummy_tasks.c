/**
 * @file    dummy_tasks.c
 * @author  Matteo Planchet
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "kernel.h"
#include "eps.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_eps_inst
 * @brief   EPS instance declaration
 */
static epsInst_t g_eps_inst = {
    .dev_i2c     = 1,     /**< I2C device that will be used for the EPS (need to be initialise outside the driver) */
    .i2c_address = 0x10u, /**< I2C address of the EPS */
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Function that runs a dummy task.
 */
void DummyMainTask(void)
{
    hkparam_t hk = { 0 };

    // Initialisation
    (void)DeviceOpen(&g_eps_inst.dev_i2c, DEVICE_TYPE_PERIPHERAL, I2C_AVIONIC);
    (void)EPS_Start(&g_eps_inst);

    // Wait Next Periode
    SleepPeriodic();

    // Task Core
    while (1)
    {
        // Get Iridium Network
        (void)GetHk1(&g_eps_inst, &hk);

        // Print HK
        LOG_DECIMAL("EPS Temperature 1: ", hk.temp[0]);
        SleepPeriodic();
    }
}