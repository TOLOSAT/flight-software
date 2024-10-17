/**
 * @file    kernel.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all kernel headers
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 */

#ifndef KERNEL_H
#define KERNEL_H

/******************************* Include Files *******************************/

#include <string.h>
#include "kernel_types.h"

// TO DO : replace with syscalls
#include "utils/housekeeping.h"

/********************************** SYSCALLS *********************************/

extern void CheckError(returnCode_t retcode);
extern tick_t GetTick(void);
extern void Sleep(tick_t tick);
extern void SleepPeriodic(void);
extern returnCode_t GetTime(time_t *time);
extern returnCode_t SetTime(time_t time);
extern returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info);
extern returnCode_t DeviceWrite(deviceNo_t device, data_t data, length_t length);
extern returnCode_t DeviceRead(deviceNo_t device, data_t data, length_t length);
extern returnCode_t DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t DeviceClose(deviceNo_t device);
extern returnCode_t GetCurrentTask(taskNo_t *task);
extern returnCode_t SuspendTask(taskNo_t task);
extern returnCode_t ResumeTask(taskNo_t task);
extern returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority);
extern returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority);
extern returnCode_t AcquireMutex(mutexNo_t mutex);
extern returnCode_t ReleaseMutex(mutexNo_t mutex);
extern void ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision);

#endif /* KERNEL_H */

/**
 * @}
 */