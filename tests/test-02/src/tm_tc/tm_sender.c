/**
 * @file tm_sender.c
 * @author Merlin Kooshmanian
 * @brief Source file for TM_SENDER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tm_tc/tm_sender.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "pus_tools/tm_management.h"

/************************** Constant Definitions *****************************/

#define TASK_NAME       "TM_SENDER"         /**< Current Task Name */
#define TASK_ID         (TM_SENDER_TASK)    /**< Current Task ID */

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

pusStatus_t SendTM(pusTM_t *tm);

/************************** Variable Definitions *****************************/

extern uartInst_t uart_tmtc_inst;

/************************* Functions Definitions *****************************/

/**
 * @fn      TmSenderMain(void *task_dyn_conf)
 * @brief   Main of the TM_SENDER Task
 * @param   task_dyn_conf Status of the current task
 */
void TmSenderMain(void *task_dyn_conf)
{
    // Variable Initialisation
    pusTM_t tm = {0};
    bufferStatus_t buffer_status;

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        buffer_status = ReadBuffer(TM_PUS1,(uint32_t) &tm, TM_MAX_SIZE);
        if(buffer_status == BUFFER_SUCCESSFUL)
        {
            SendTM(&tm);
        }
        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @fn      SendTM(pusTM_t *tm)
 * @brief   Function that send TM toward the DMA for sending
 * @param   tm Pointer to the TM we want to send
 * @retval  PUS_INVALID_PARAM if TM is invalid for UART Write
 * @retval  PUS_ERROR if UART_Write has encountered an error
 * @retval  PUS_SUCCESSFUL else
 */
pusStatus_t SendTM(pusTM_t *tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    halStatus_t read_status = FCT_SUCCESSFUL;
    uartMsg_t tm_size = 0;

    // Function Core
    tm_size = tm->spp_header.packet_data_length + SPP_HEADER_SIZE + 1u;
    FormatTM(tm);
    read_status = UartWrite(&uart_tmtc_inst, (uartMsg_t *) tm, tm_size);
    if(read_status != FCT_SUCCESSFUL)
    {
        if(read_status == FCT_INVALID_PARAM)
        {
            return_value = PUS_INVALID_PARAM;
        }
        else
        {
            return_value = PUS_ERROR;
        }
    }

    return(return_value);
}