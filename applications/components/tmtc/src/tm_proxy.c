/**
 * @file    tm_proxy.c
 * @author  Matteo Planchet, Arthur Morain
 * @brief   Source file for TM_PROXY Task
 *
 * @copyright Copyright (c) TOLOSAT 2026
 * SPDX-License-Identifier: Apache-2.0
 */

/******************************* Include Files *******************************/

#include "tm_proxy.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

#define NB_SEND_ENTRY 7u /**< Maximum number of input buffers */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

void AddTmToPacketStore(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    deviceNo_t dev_pus15_index = NULL;
    deviceNo_t dev_pus15_data  = NULL;

    // Open PUS15 index and data files
    return_value = DeviceOpen(&dev_pus15_index, DEVICE_TYPE_FILE, PUS15_INDEX_FILE);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = DeviceOpen(&dev_pus15_data, DEVICE_TYPE_FILE, PUS15_DATA_FILE);
        if (return_value == RET_SUCCESSFUL)
        {
            // Set the read/write pointer to the beginning of the file
            length_t offset = 0u;
            return_value    = DeviceIoctl(pus15_env->dev_pus15_index, IOCTL_FS_SEEK, &offset, sizeof(length_t));

            if (return_value == RET_SUCCESSFUL)
            {
                // Read packet store info from index file header
                pus15PacketStoreIndexInfo_t packet_store_info = { 0 };
                return_value = DeviceRead(dev_pus15_index, (data_t)&packet_store_info, sizeof(pus15PacketStoreIndexInfo_t));
                if (return_value == RET_SUCCESSFUL)
                {
                    // TODO: add TM to packet store data file
                    // TODO: update write cursor and nb entries
                    // TODO: write packet store info header back
                }
            }
        }
    }
}

/**
 * @copydoc TmProxyMain
 */
void TmProxyMain(void)
{
    // Initialisation
    static pusSendTable_t tm_send_table[NB_SEND_ENTRY] = {
        { .buffer = TM_PUS1 },   { .buffer = TM_PUS3 },   { .buffer = TM_PUS5 },   { .buffer = TM_PUS15 },
        { .buffer = TM_NORMAL }, { .buffer = TM_PUS178 }, { .buffer = TM_PUS193 },
    };

    // Task Core
    while (1)
    {
        // Write TMs into his specified buffer
        CheckError(SendTM(&send_tm_context), SEVERITY_MEDIUM);

        CheckError(WaitSignal(SIGNAL_TM), SEVERITY_MEDIUM);
    }
}