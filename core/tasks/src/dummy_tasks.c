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

#define MSG_SIZE        2U

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern gpioInst_t led2_inst;

/************************* Functions Definitions *****************************/

/**
 * @brief Function implementing the blink01 thread.
 * @param argument: Not used
 * @retval None
 */
void StartBlink01(void *argument __attribute__((unused)))
{
    // Variable Initialisation
    uint32_t msg[MSG_SIZE] = {0};
    bufferStatus_t retval = 0;

    // Initialisation
    printf("[#1] Init\n");

    // Function Core
    while (1)
    {
        msg[0] = 0;
        msg[1] = 0;
        GpioToggle(&led2_inst);
        retval = ReadBuffer(BUFF01_BUFFER, msg, MSG_SIZE);
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
    uint32_t msg[MSG_SIZE] = {1,2};
    bufferStatus_t retval = 0;

    // Initialisation
    printf("[#2] Init\n");

    // Function Core
    while (1)
    {
        GpioToggle(&led2_inst);
        retval = WriteBuffer(BUFF01_BUFFER, msg, MSG_SIZE);
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