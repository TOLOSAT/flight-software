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

/***************************** Macros Definitions ****************************/

#define IRIDIUM_TEST_MODE 1 /**< 1: TX, 0: RX */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_iridium_inst
 * @brief   Iridium instance declaration
 */
iridiumInst_t g_iridium_inst = {
    .uart_ref    = UART_PL,
    .hw_ctrl_reg = IRIDIUM_ECHO_OFF | IRIDIUM_MSG_RX_ALERT_OFF |      // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_VERBOSE_OFF | IRIDIUM_SBD_TIMEOUT_INF |    // cppcheck-suppress misra-c2012-12.2; False positive
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
#if IRIDIUM_TEST_MODE == 1
    // Tx Message
    iridiumSBDTxMsg_t message = { 0 };

    // Set message value
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
#else
    // Rx Message
    iridiumSBDRxMsg_t message = { 0 };
#endif

    // Initialisation
    (void)IridiumStart(&g_iridium_inst);

    // Task Core
    while (1)
    {
#if IRIDIUM_TEST_MODE == 1
        // Send Message
        (void)IridiumSendSBD(&g_iridium_inst, message);
#else
        // Receive Message
        (void)IridiumReceiveSBD(&g_iridium_inst, message);
#endif

        SleepPeriodic();
    }
}