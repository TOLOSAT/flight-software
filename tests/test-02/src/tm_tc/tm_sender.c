/**
 * @file tm_sender.c
 * @author Merlin Kooshmanian
 * @brief Source file for TM_SENDER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "tm_tc/tm_sender.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "pus_tools/tm_management.h"

/***************************** Macros Definitions ****************************/

#define NB_ENTRY_BUFFERS    2u                  /**< Maximum number of input buffers */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

static pusStatus_t SendTM(pusTM_t *tm);

/*************************** Variables Definitions ***************************/

extern uartInst_t uart_tmtc_inst;

/**
 * @var     g_tm_sender_buffer_entry
 * @brief   Entry buffer list for TM sender
 * @warning Order of buffers is important
 */
const bufferRef_t g_tm_sender_buffer_entry[NB_ENTRY_BUFFERS] =
{
    TM_PUS1,
    TM_NORMAL,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              TmSenderMain(void *task_dyn_conf)
 * @brief           Main of the TM_SENDER Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void TmSenderMain(void *task_dyn_conf)
{
    // Variable Initialisation
    bufferStatus_t buffer_status;
    pusTM_t tm = {0};
    bufferDepth_t buffer_count = 0;

    // Initialisation
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        // We will read each buffer in g_tm_sender_buffer_entry
        for(uint32_t i = 0; i < NB_ENTRY_BUFFERS; i++)
        {
            // Get how many message there is in buffer
            GetBufferCount(g_tm_sender_buffer_entry[i], &buffer_count);
            // Now we read the buffer until it is empty
            for(uint32_t k = 0; k < buffer_count; k++)
            {
                buffer_status = ReadBuffer(g_tm_sender_buffer_entry[i], (bufferMsgAddr_t) &tm, TM_MAX_SIZE);
                if(buffer_status == BUFFER_SUCCESSFUL)
                {
                    SendTM(&tm);
                    waitUntilNextPeriod(task_dyn_conf);
                }
            }
        }
        
        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @fn          SendTM(pusTM_t *tm)
 * @brief       Function that send TM toward the DMA for sending
 * @param[in]   tm Pointer to the TM we want to send
 * @retval      #PUS_ERROR if UART_Write has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t SendTM(pusTM_t *tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    halStatus_t read_status = FCT_BUSY;
    uartMsg_t tm_size = 0;

    // Function Core
    tm_size = tm->spp_header.packet_data_length + SPP_HEADER_SIZE + 1u;
    FormatTM(tm);

    while(read_status == FCT_BUSY)
    {
        read_status = UartWrite(&uart_tmtc_inst, (uartMsg_t *) tm, tm_size);
    }

    if(read_status != FCT_SUCCESSFUL)
    {
        return_value = PUS_ERROR;
    }

    return return_value;
}