/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
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
static iridiumInst_t g_iridium_inst = {
    .hw_ctrl_reg = IRIDIUM_ECHO_OFF | IRIDIUM_MSG_RX_ALERT_OFF |      // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_VERBOSE_OFF | IRIDIUM_SBD_TIMEOUT_2S |     // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_QUIET_OFF | IRIDIUM_HW_CTRL_FLOW_DISABLE | // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_DTR_OFF | IRIDIUM_19200_BPS,
    .minimum_availability = IRIDIUM_NETWORK_POOR,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Function that runs a dummy task.
 */
void DummyMainTask(void)
{
    iridiumSDBTxMsg_t message = { 0 };
    // Set message
    message[0]  = 'h';
    message[1]  = 'e';
    message[2]  = 'l';
    message[3]  = 'l';
    message[4]  = 'o';
    message[5]  = ' ';
    message[6]  = 'w';
    message[7]  = 'o';
    message[8]  = 'r';
    message[9]  = 'l';
    message[10] = 'd';
    message[11] = '\n';
    message[12] = '\r';

    // Initialisation
    (void)DeviceOpen(&g_iridium_inst.dev_uart, DEVICE_TYPE_PERIPHERAL, UART_PL);
    (void)IridiumStart(&g_iridium_inst);

    // Wait 1s
    Sleep(1000);

    // Task Core
    while (1)
    {
        // Get Iridium Network
        (void)IridiumSendSDB(&g_iridium_inst, message);

        SleepPeriodic();
    }
}