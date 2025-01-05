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

#include "kernel_types.h"
#include "utils/log.h"
#include "utils/endianness.h"

/********************************** SYSCALLS *********************************/

/**
 * @cond    IGNORE_DOXYGEN
 * @note    Syscall redefinition to make the user think they use the kernel function
 */
#define CheckError          sys_CheckError          /**< CheckError syscall redefinition */
#define Sleep               sys_Sleep               /**< Sleep syscall redefinition */
#define SleepPeriodic       sys_SleepPeriodic       /**< SleepPeriodic syscall redefinition */
#define GetTick             sys_GetTick             /**< GetTick syscall redefinition */
#define GetTime             sys_GetTime             /**< GetTime syscall redefinition */
#define SetTime             sys_SetTime             /**< SetTime syscall redefinition */
#define DeviceOpen          sys_DeviceOpen          /**< DeviceOpen syscall redefinition */
#define DeviceWrite         sys_DeviceWrite         /**< DeviceWrite syscall redefinition */
#define DeviceRead          sys_DeviceRead          /**< DeviceRead syscall redefinition */
#define DeviceIoctl         sys_DeviceIoctl         /**< DeviceIoctl syscall redefinition */
#define DeviceClose         sys_DeviceClose         /**< DeviceClose syscall redefinition */
#define GetCurrentTask      sys_GetCurrentTask      /**< GetCurrentTask syscall redefinition */
#define SuspendTask         sys_SuspendTask         /**< SuspendTask syscall redefinition */
#define ResumeTask          sys_ResumeTask          /**< ResumeTask syscall redefinition */
#define GetTaskPriority     sys_GetTaskPriority     /**< GetTaskPriority syscall redefinition */
#define SetTaskPriority     sys_SetTaskPriority     /**< SetTaskPriority syscall redefinition */
#define AcquireMutex        sys_AcquireMutex        /**< AcquireMutex syscall redefinition */
#define ReleaseMutex        sys_ReleaseMutex        /**< ReleaseMutex syscall redefinition */
#define SendSignal          sys_SendSignal          /**< SendSignal syscall redefinition */
#define WaitSignal          sys_WaitSignal          /**< WaitSignal syscall redefinition */
#define ConsolePrint        sys_ConsolePrint        /**< ConsolePrint syscall redefinition */
#define EnableHK            sys_EnableHK            /**< EnableHK syscall redefinition */
#define DisableHK           sys_DisableHK           /**< DisableHK syscall redefinition */
#define EmitHK              sys_EmitHK              /**< EmitHK syscall redefinition */
#define CollectHKs          sys_CollectHKs          /**< CollectHKs syscall redefinition */
/** @endcond */

/*
 * Syscall declarations
 */
extern void CheckError(returnCode_t retcode);
extern void Sleep(tick_t tick);
extern void SleepPeriodic(void);
extern tick_t GetTick(void);
extern returnCode_t GetTime(time_t *time);
extern returnCode_t SetTime(time_t time);
extern returnCode_t DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource);
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
extern returnCode_t SendSignal(taskNo_t task, signalMask_t mask);
extern returnCode_t WaitSignal(signalMask_t mask);
extern void ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision);
extern returnCode_t EnableHK(hkId_t hkid);
extern returnCode_t DisableHK(hkId_t hkid);
extern returnCode_t EmitHK(hk_t *hk);
extern returnCode_t CollectHKs(void);

#endif /* KERNEL_H */

/**
 * @}
 */