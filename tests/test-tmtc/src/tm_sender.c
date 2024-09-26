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
#include "kernel.h"
#include "pus.h"

/***************************** Macros Definitions ****************************/

#define NB_ENTRY_BUFFERS    2u  /**< Maximum number of input buffers */

/*************************** Functions Declarations **************************/

static pusStatus_t SendTM(pusTM_t *tm);

/*************************** Variables Definitions ***************************/

/**
 * @var     dev_uart_tmtc_tx
 * @brief   UART TMTC TX device
 */
static deviceNo_t IN_TMTC_DATA_SECTION IN_TMTC_DATA_SECTION dev_uart_tmtc_tx;

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
    kernelStatus_t buffer_status;
    static pusTM_t IN_DMABUFF_SECTION send_tm = {0};
    length_t buffer_count = 0;
    static bufferNo_t IN_TMTC_DATA_SECTION tm_sender_buffer_entry[NB_ENTRY_BUFFERS] =
    {
        TM_PUS1,
        TM_NORMAL,
    };

    // Initialisation
    task_status = DeviceOpen(&dev_uart_tmtc_tx, DEVICE_TYPE_PERIPHERAL, UART_TMTC, 0u);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceIoctl(dev_uart_tmtc_tx, UART_IOCTL_START_TX, &send_tm, TM_MAX_SIZE);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // We will read each buffer in tm_sender_buffer_entry
        for (uint32_t i = 0; i < NB_ENTRY_BUFFERS; i++)
        {
            // Get how many message there is in buffer
            (void)GetBufferCount(tm_sender_buffer_entry[i], &buffer_count);
            // Now we read the buffer until it is empty
            for (uint32_t k = 0; k < buffer_count; k++)
            {
                buffer_status = BufferRead(tm_sender_buffer_entry[i], (data_t)&send_tm, TM_MAX_SIZE);
                if (buffer_status == KERNEL_SUCCESSFUL)
                {
                    // Send TM
                    task_status = SendTM(&send_tm);
                    CheckErrors(task_status, FDIR_ERROR_HANDLER);

                    // Yield until DMA ended transaction
                    kernelStatus_t test_tx_end = DeviceIoctl(dev_uart_tmtc_tx, UART_IOCTL_CHECK_TX_ENDED, NULL, 0u);
                    while (test_tx_end == KERNEL_BUSY)
                    {
                        task_status = TaskYield(task_desc);
                        CheckErrors(task_status, FDIR_ERROR_HANDLER);
                        test_tx_end = DeviceIoctl(dev_uart_tmtc_tx, UART_IOCTL_CHECK_TX_ENDED, NULL, 0u);
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
        length_t tm_size = tm->spp_header.packet_data_length + SPP_HEADER_SIZE + 1u;
        (void)FormatTM(tm);

        kernelStatus_t test_tx = DeviceWrite(dev_uart_tmtc_tx, (data_t)tm, tm_size);
        if(test_tx != KERNEL_SUCCESSFUL)
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