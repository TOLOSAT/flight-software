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

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              TmSenderMain(void)
 * @brief           Main of the TM_SENDER Task
 */
void TmSenderMain(void)
{
    // Initialisation
    returnCode_t status;
    returnCode_t buffer_status;
    static pusTM_t IN_DMABUFF_SECTION send_tm = {0};
    static bufferNo_t tm_sender_buffer_entry[NB_ENTRY_BUFFERS] =
    {
        TM_PUS1,
        TM_NORMAL,
    };
    deviceNo_t dev_uart_tmtc_tx = 0u;
    length_t buffer_count = 0;

    status = DeviceOpen(&dev_uart_tmtc_tx, DEVICE_TYPE_PERIPHERAL, UART_TMTC, DEVICE_NO_EXTRA_INFO);
    CheckErrors(status, FDIR_ERROR_HANDLER);
    status = DeviceIoctl(dev_uart_tmtc_tx, UART_IOCTL_START_TX, &send_tm, TM_MAX_SIZE);
    CheckErrors(status, FDIR_ERROR_HANDLER);

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
                if (buffer_status == RET_SUCCESSFUL)
                {
                    // Send TM
                    status = SendTM(&send_tm, dev_uart_tmtc_tx);
                    CheckErrors(status, FDIR_ERROR_HANDLER);

                    // Yield until DMA ended transaction
                    returnCode_t test_tx_end = DeviceIoctl(dev_uart_tmtc_tx, UART_IOCTL_CHECK_TX_ENDED, NULL, 0u);
                    while (test_tx_end == RET_BUSY)
                    {
                        Sleep(0);
                        test_tx_end = DeviceIoctl(dev_uart_tmtc_tx, UART_IOCTL_CHECK_TX_ENDED, NULL, 0u);
                    }
                }
            }
        }

        SleepPeriodic();
    }
}
