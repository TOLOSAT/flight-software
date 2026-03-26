/**
 * @file    tm_event.c
 * @author  Arthur Morain, Matteo Planchet
 * @brief   Source file for building event-related TMs
 *
 * @copyright Copyright (c) TOLOSAT 2026
 */

/******************************* Include Files *******************************/

#include "tm_sender.h"
#include "kernel.h"
#include "pus.h"
#include "system_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn    ReportEvent(severityLevel_t level)
 * @brief Event reporting callback.
 * @param level Severity level of the error.
 */
void ReportEvent(severityLevel_t level)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    eventReport_t report = {
        .EID = 0u,
        // Matteo's Birthday (data not used yet)
        .data = { 10u, 12u, 2u, 0u, 0u, 4u }
    };
    pusTM_t tm = { 0 };

    // Populate report with relevant data
    return_value = BuildS5SS1234(&tm, level, &report);
    if (return_value == RET_SUCCESSFUL)
    {
        // Open PUS 5 buffer
        deviceNo_t dev_pus5_buffer;
        return_value = DeviceOpen(&dev_pus5_buffer, DEVICE_TYPE_BUFFER, TM_PUS5);

        if (return_value == RET_SUCCESSFUL)
        {
            // Write the TM to the buffer
            return_value = DeviceWrite(dev_pus5_buffer, (data_t)&tm, TM_MAX_SIZE);

            if (return_value == RET_SUCCESSFUL)
            {
                taskNo_t tm_sender = NO_TASK;
                ConsolePrint("TM(%d,%d) has been sent\n", tm.tm_header.service, tm.tm_header.subservice);
                return_value = DeviceIoctl(dev_pus5_buffer, IOCTL_BUFFER_GET_RECEIVER, &tm_sender, sizeof(taskNo_t));
                // Check if a receiver is registered and send signal to the TM sender
                if ((return_value == RET_SUCCESSFUL) && (tm_sender != NO_TASK))
                {
                    return_value = SendSignal(tm_sender, SIGNAL_TM);
                }
            }
        }
    }

    CheckError(return_value, SEVERITY_MEDIUM);
}
