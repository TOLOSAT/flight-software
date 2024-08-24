/**
 * @file    tm_sender.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TM_SENDER Task
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tmtc/tm_sender.h"
#include "core.h"
#include "pus.h"

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
const bufferNo_t IN_TMTC_DATA_SECTION g_tm_sender_buffer_entry[NB_ENTRY_BUFFERS] =
{
    TM_PUS1,
    TM_NORMAL,
    TM_PUS161,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              TmSenderMain(void *task_desc)
 * @brief           Main of the TM_SENDER Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_TMTC_TEXT_SECTION TmSenderMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    coreStatus_t buffer_status;
    static pusTM_t IN_DMABUFF_SECTION send_tm = {0};
    bufferDepth_t buffer_count = 0;

    // Initialisation
    task_status = UartIoctl(&g_uart_tmtc_inst, UART_IOCTL_START_TX, &send_tm, TM_MAX_SIZE);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
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
                if (buffer_status == CORE_SUCCESSFUL)
                {
                    // Send TM
                    task_status = SendTM(&send_tm);
                    CheckErrors(task_status, FDIR_ERROR_HANDLER);

                    // Yield until DMA ended transaction
                    halStatus_t test_hal = UartIoctl(&g_uart_tmtc_inst, UART_IOCTL_CHECK_TX_ENDED, NULL, 0u);
                    while (test_hal == GEN_HAL_BUSY)
                    {
                        task_status = TaskYield(task_desc);
                        CheckErrors(task_status, FDIR_ERROR_HANDLER);
                        test_hal = UartIoctl(&g_uart_tmtc_inst, UART_IOCTL_CHECK_TX_ENDED, NULL, 0);
                    }
                }
            }
        }

        task_status = WaitUntilNextPeriod(task_desc);
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
static pusStatus_t IN_TMTC_TEXT_SECTION SendTM(pusTM_t *tm)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (tm != NULL)
    {
        // Get size of TM then format it
        uartMsg_t tm_size = tm->spp_header.packet_data_length + SPP_HEADER_SIZE + 1u;
        (void)FormatTM(tm);

        halStatus_t test_hal = UartWrite(&g_uart_tmtc_inst, (uartMsg_t *)tm, tm_size);
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