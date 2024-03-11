/**
 * @file    tm_sender.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TM_SENDER Task
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tm_sender.h"
#include "core_basics.h"
#include "io_instances.h"
#include "pus_tools/tm_management.h"

/***************************** Macros Definitions ****************************/

#define IN_DMABUFF_SECTION  __attribute__((section(".dmabuff")))    /**< Temporary file goes to .dmabuff section */

/*************************** Functions Declarations **************************/

static pusStatus_t SendTM(pusTM_t *tm);

/*************************** Variables Definitions ***************************/

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
    uint32_t task_status;
    bufferStatus_t buffer_status;
    static pusTM_t IN_DMABUFF_SECTION send_tm = {0};
    bufferDepth_t buffer_count = 0;
    halIoCtlCmd_t start_tx_transfer = {UART_IOCTL_START_TX, TM_MAX_SIZE, &send_tm};

    // Initialisation
    task_status = UartIoctl(&uart_tmtc_inst, start_tx_transfer);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // We will read each buffer in g_tm_sender_buffer_entry
        for (uint32_t i = 0; i < NB_ENTRY_BUFFERS; i++)
        {
            // Get how many message there is in buffer
            (void)GetBufferCount(g_tm_sender_buffer_entry[i], &buffer_count);
            // Now we read the buffer until it is empty
            for (uint32_t k = 0; k < buffer_count; k++)
            {
                buffer_status = ReadBuffer(g_tm_sender_buffer_entry[i], (bufferMsgAddr_t)&send_tm, TM_MAX_SIZE);
                if (buffer_status == BUFFER_SUCCESSFUL)
                {
                    // Send TM
                    task_status = SendTM(&send_tm);
                    CheckErrors(task_status, FDIR_ERROR_HANDLER);

                    // Yield until DMA ended transaction
                    halIoCtlCmd_t check_tx_transfer = {UART_IOCTL_CHECK_TX_ENDED, 0u, NULL};
                    halStatus_t test_hal = UartIoctl(&uart_tmtc_inst, check_tx_transfer);
                    while (test_hal == GEN_HAL_BUSY)
                    {
                        task_status = TaskYield(task_dyn_conf);
                        CheckErrors(task_status, FDIR_ERROR_HANDLER);
                        test_hal = UartIoctl(&uart_tmtc_inst, check_tx_transfer);
                    }
                }
            }
        }

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn          SendTM(pusTM_t *tm)
 * @brief       Function that send TM toward the DMA for sending
 * @param[in]   tm Pointer to the TM we want to send
 * @retval      #PUS_INVALID_PARAM if tm is a null pointer
 * @retval      #PUS_ERROR if UART_Write has encountered an error
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t SendTM(pusTM_t *tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tm != NULL)
    {
        // Get size of TM then format it
        uartMsg_t tm_size = tm->spp_header.packet_data_length + SPP_HEADER_SIZE + 1u;
        (void)FormatTM(tm);

        halStatus_t test_hal = UartWrite(&uart_tmtc_inst, (uartMsg_t *)tm, tm_size);
        if(test_hal != GEN_HAL_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}