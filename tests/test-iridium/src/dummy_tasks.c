/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "kernel.h"
#include "iridium_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_iridium_inst
 * @brief   Iridium instance declaration
 */
static iridiumInst_t g_iridium_inst =
{
    .hw_ctrl_reg = IRIDIUM_ECHO_OFF | IRIDIUM_MSG_RX_ALERT_OFF |        // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_VERBOSE_OFF | IRIDIUM_SBD_TIMEOUT_2S |       // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_QUIET_OFF | IRIDIUM_HW_CTRL_FLOW_DISABLE |   // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_DTR_OFF | IRIDIUM_115200_BPS,
    .minimum_availability = IRIDIUM_NETWORK_POOR,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyTask01(void)
 * @brief   Function that runs the dummy task 01.
 */
void DummyTask01(void)
{
    // Variable Initialisation
    deviceNo_t dev_user_led;

    // Initialisation
    ConsolePrint("[#1] Init\n");
    (void)DeviceOpen(&dev_user_led, DEVICE_TYPE_PERIPHERAL, USER_LED, DEVICE_NO_EXTRA_INFO);

    // Function Core
    while (1)
    {
        // Toggle LED
        (void)DeviceIoctl(dev_user_led, GPIO_IOCTL_TOGGLE, NULL, 0u);
        
        SleepPeriodic();
    }
}

/**
 * @fn      DummyTask02(void)
 * @brief   Function that runs the dummy task 02.
 */
void DummyTask02(void)
{
    // Variable Initialisation
    iridiumSDBTxMsg_t message = {0};

    // Initialisation
    ConsolePrint("[#2] Init\n");
    (void)DeviceOpen(&g_iridium_inst.dev_uart, DEVICE_TYPE_PERIPHERAL, UART_PL, DEVICE_NO_EXTRA_INFO);
    (void)IridiumStart(&g_iridium_inst);

    // Wait Next Periode
    SleepPeriodic();

    // Function Core
    while (1)
    {
        // Get Iridium Network
        (void)IridiumSendSDB(&g_iridium_inst, message);

        SleepPeriodic();
    }
}