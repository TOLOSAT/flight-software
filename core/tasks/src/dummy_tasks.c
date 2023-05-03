/**
 * @file dummy_tasks.h
 * @author Merlin Kooshmanian
 * @brief Header file with dummy tasks
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tasks.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

#define BUFFER_MSG_SIZE     2U
#define UART_MSG_SIZE       1U
#define I2C_MSG_SIZE        1U
#define SLAVE_ADDR          0x05

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern gpioInst_t led_inst;
extern uartInst_t uart_cu_inst;
extern uartInst_t uart_tmtc_inst;
extern iicInst_t iic_avionic_inst;

/************************* Functions Definitions *****************************/

/**
 * @brief Function implementing the blink01 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink01(void *argument __attribute__((unused)))
{
    // Variable Initialisation
    uint32_t msg[BUFFER_MSG_SIZE] = {0};
    uint8_t msg_uart_rx[UART_MSG_SIZE] = {0x00};
    bufferStatus_t retval = 0;

    // Initialisation
    printf("[#1] Init\n");

    // Function Core
    while (1)
    {
        msg[0] = 0;
        msg[1] = 0;
        GpioToggle(&led_inst);
        UartRead(&uart_cu_inst, msg_uart_rx, UART_MSG_SIZE);
        printf("[#1] Msg Received : 0x%x\n", msg_uart_rx[0]);
        retval = ReadBuffer(BUFF01_BUFFER, msg, BUFFER_MSG_SIZE);
        switch (retval)
        {
            case BUFFERS_SUCCESSFUL:
                printf("[#1] Message received (Msg = (%ld,%ld))\n", msg[0], msg[1]);
                break;
            case BUFFERS_EMPTY:
                printf("[#1] Buffer Empty (Msg = (%ld,%ld))\n", msg[0], msg[1]);
                break;
            default:
                printf("[#1] Error\n");
                break;
        }
        osDelay(500);
    }
    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @brief Function implementing the blink02 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink02(void *argument __attribute__((unused)))
{
    // Variable Initialisation
    uint32_t msg[BUFFER_MSG_SIZE] = {1,2};
    uint8_t msg_i2c_tx[I2C_MSG_SIZE] = {0x55};
    bufferStatus_t retval = 0;

    // Initialisation
    printf("[#2] Init\n");

    // Function Core
    while (1)
    {
        GpioToggle(&led_inst);
        IicWrite(&iic_avionic_inst, SLAVE_ADDR, msg_i2c_tx, I2C_MSG_SIZE);
        retval = WriteBuffer(BUFF01_BUFFER, msg, BUFFER_MSG_SIZE);
        switch (retval)
        {
            case BUFFERS_SUCCESSFUL:
                printf("[#2] Message sended\n");
                break;
            case BUFFERS_FULL:
                printf("[#2] Buffer Full\n");
                break;
            default:
                printf("[#2] Error\n");
                break;
        }
        osDelay(1100);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @brief Function implementing the blink03 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink03(void *argument __attribute__((unused)))
{
    // Variable Initialisation
    uint8_t tmtc_msg[3] = {0x41, 0x0a, 0x0d};
    bufferStatus_t retval = 0;

    // Initialisation
    printf("[#3] Init\n");

    // Function Core
    while (1)
    {
        printf("[#3] Hello\n");
        UartWrite(&uart_tmtc_inst, tmtc_msg, 3);
        osDelay(800);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}