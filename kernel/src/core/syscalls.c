/**
 * @file    syscalls.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining syscalls
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void sys_CheckError(returnCode_t retcode);
extern void sys_Sleep(tick_t tick);
extern void sys_SleepPeriodic(void);
extern tick_t sys_GetTick(void);
extern returnCode_t sys_GetTime(time_t *time);
extern returnCode_t sys_SetTime(time_t time);
extern returnCode_t sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource, uint32_t extra_info);
extern returnCode_t sys_DeviceWrite(deviceNo_t device, data_t data, length_t length);
extern returnCode_t sys_DeviceRead(deviceNo_t device, data_t data, length_t length);
extern returnCode_t sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t sys_DeviceClose(deviceNo_t device);
extern returnCode_t sys_GetCurrentTask(taskNo_t *task);
extern returnCode_t sys_SuspendTask(taskNo_t task);
extern returnCode_t sys_ResumeTask(taskNo_t task);
extern returnCode_t sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority);
extern returnCode_t sys_SetTaskPriority(taskNo_t task, taskPriority_t priority);
extern returnCode_t sys_AcquireMutex(mutexNo_t mutex);
extern returnCode_t sys_ReleaseMutex(mutexNo_t mutex);
extern returnCode_t sys_SendSignal(taskNo_t task, signalMask_t mask);
extern returnCode_t sys_WaitSignal(signalMask_t mask);
extern void sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision);
extern returnCode_t sys_EnableHK(hkId_t hkid);
extern returnCode_t sys_DisableHK(hkId_t hkid);
extern returnCode_t sys_EmitHK(hk_t *hk);
extern returnCode_t sys_CollectHKs(void);

/*************************** Variables Definitions ***************************/

/************************** System Calls Definitions *************************/

/**
 * @fn      sys_CheckError(returnCode_t retcode)
 * @brief   Syscall declaration for CheckError
 */
void ATTR_SYSCALL sys_CheckError(returnCode_t retcode)
{
    // Ignore unused parameters
    (void)(retcode);

    // Call SVC exception
    __asm volatile(
        " .extern CheckError                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne CheckError_unpriv             \n"
        " CheckError_priv :                 \n"
        "   b CheckError                    \n"
        " CheckError_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_CHECK_ERROR) : "memory");
}

/**
 * @fn      sys_Sleep(tick_t tick)
 * @brief   Syscall declaration for Sleep
 */
void ATTR_SYSCALL sys_Sleep(tick_t tick)
{
    // Ignore unused parameters
    (void)(tick);

    // Call SVC exception
    __asm volatile(
        " .extern Sleep                     \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne Sleep_unpriv                  \n"
        " Sleep_priv :                      \n"
        "   b Sleep                         \n"
        " Sleep_unpriv :                    \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_SLEEP) : "memory");
}

/**
 * @fn      sys_SleepPeriodic(void)
 * @brief   Syscall declaration for SleepPeriodic
 */
void ATTR_SYSCALL sys_SleepPeriodic(void)
{
    // Call SVC exception
    __asm volatile(
        " .extern SleepPeriodic             \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne SleepPeriodic_unpriv          \n"
        " SleepPeriodic_priv :              \n"
        "   b SleepPeriodic                 \n"
        " SleepPeriodic_unpriv :            \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_SLEEP_PERIODIC) : "memory");
}

/**
 * @fn      sys_GetTick(void)
 * @brief   Syscall declaration for GetTick
 */
tick_t ATTR_SYSCALL sys_GetTick(void)
{
    // Call SVC exception
    __asm volatile(
        " .extern GetTick                   \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne GetTick_unpriv                \n"
        " GetTick_priv :                    \n"
        "   b GetTick                       \n"
        " GetTick_unpriv :                  \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_GET_TICK) : "memory");
}

/**
 * @fn      sys_GetTime(time_t *time)
 * @brief   Syscall declaration for GetTime
 */
returnCode_t ATTR_SYSCALL sys_GetTime(time_t *time)
{
    // Ignore unused parameters
    (void)(time);

    // Call SVC exception
    __asm volatile(
        " .extern GetTime                   \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne GetTime_unpriv                \n"
        " GetTime_priv :                    \n"
        "   b GetTime                       \n"
        " GetTime_unpriv :                  \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_GET_TIME) : "memory");
}

/**
 * @fn      sys_SetTime(time_t time)
 * @brief   Syscall declaration for SetTime
 */
returnCode_t ATTR_SYSCALL sys_SetTime(time_t time)
{
    // Ignore unused parameters
    (void)(time);

    // Call SVC exception
    __asm volatile(
        " .extern SetTime                   \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne SetTime_unpriv                \n"
        " SetTime_priv :                    \n"
        "   b SetTime                       \n"
        " SetTime_unpriv :                  \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_SET_TIME) : "memory");
}

/**
 * @fn      sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource, uint32_t extra_info)
 * @brief   Syscall declaration for DeviceOpen
 */
returnCode_t ATTR_SYSCALL sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource, uint32_t extra_info)
{
    // Ignore unused parameters
    (void)(device);
    (void)(type);
    (void)(resource);
    (void)(extra_info);

    // Call SVC exception
    __asm volatile(
        " .extern DeviceOpen                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne DeviceOpen_unpriv             \n"
        " DeviceOpen_priv :                 \n"
        "   b DeviceOpen                    \n"
        " DeviceOpen_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_DEVICE_OPEN) : "memory");
}

/**
 * @fn      sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceWrite
 */
returnCode_t ATTR_SYSCALL sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    // Ignore unused parameters
    (void)(device);
    (void)(data);
    (void)(length);

    // Call SVC exception
    __asm volatile(
        " .extern DeviceWrite               \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne DeviceWrite_unpriv            \n"
        " DeviceWrite_priv :                \n"
        "   b DeviceWrite                   \n"
        " DeviceWrite_unpriv :              \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_DEVICE_WRITE) : "memory");
}

/**
 * @fn      sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceRead
 */
returnCode_t ATTR_SYSCALL sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    // Ignore unused parameters
    (void)(device);
    (void)(data);
    (void)(length);

    // Call SVC exception
    __asm volatile(
        " .extern DeviceRead                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne DeviceRead_unpriv             \n"
        " DeviceRead_priv :                 \n"
        "   b DeviceRead                    \n"
        " DeviceRead_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_DEVICE_READ) : "memory");
}

/**
 * @fn      sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief   Syscall declaration for DeviceIoctl
 */
returnCode_t ATTR_SYSCALL sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    // Ignore unused parameters
    (void)(device);
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Call SVC exception
    __asm volatile(
        " .extern DeviceIoctl               \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne DeviceIoctl_unpriv            \n"
        " DeviceIoctl_priv :                \n"
        "   b DeviceIoctl                   \n"
        " DeviceIoctl_unpriv :              \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_DEVICE_IOCTL) : "memory");
}

/**
 * @fn      sys_DeviceClose(deviceNo_t device)
 * @brief   Syscall declaration for DeviceClose
 */
returnCode_t ATTR_SYSCALL sys_DeviceClose(deviceNo_t device)
{
    // Ignore unused parameters
    (void)(device);

    // Call SVC exception
    __asm volatile(
        " .extern DeviceClose               \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne DeviceClose_unpriv            \n"
        " DeviceClose_priv :                \n"
        "   b DeviceClose                   \n"
        " DeviceClose_unpriv :              \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_DEVICE_CLOSE) : "memory");
}

/**
 * @fn      sys_GetCurrentTask(taskNo_t *task)
 * @brief   Syscall declaration for GetCurrentTask
 */
returnCode_t ATTR_SYSCALL sys_GetCurrentTask(taskNo_t *task)
{
    // Ignore unused parameters
    (void)(task);

    // Call SVC exception
    __asm volatile(
        " .extern GetCurrentTask            \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne GetCurrentTask_unpriv         \n"
        " GetCurrentTask_priv :             \n"
        "   b GetCurrentTask                \n"
        " GetCurrentTask_unpriv :           \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_GET_CURRENT_TASK) : "memory");
}

/**
 * @fn      sys_SuspendTask(taskNo_t task)
 * @brief   Syscall declaration for SuspendTask
 */
returnCode_t ATTR_SYSCALL sys_SuspendTask(taskNo_t task)
{
    // Ignore unused parameters
    (void)(task);

    // Call SVC exception
    __asm volatile(
        " .extern SuspendTask               \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne SuspendTask_unpriv            \n"
        " SuspendTask_priv :                \n"
        "   b SuspendTask                   \n"
        " SuspendTask_unpriv :              \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_SUSPEND_TASK) : "memory");
}

/**
 * @fn      sys_ResumeTask(taskNo_t task)
 * @brief   Syscall declaration for ResumeTask
 */
returnCode_t ATTR_SYSCALL sys_ResumeTask(taskNo_t task)
{
    // Ignore unused parameters
    (void)(task);

    // Call SVC exception
    __asm volatile(
        " .extern ResumeTask                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne ResumeTask_unpriv             \n"
        " ResumeTask_priv :                 \n"
        "   b ResumeTask                    \n"
        " ResumeTask_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_RESUME_TASK) : "memory");
}

/**
 * @fn      sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief   Syscall declaration for GetTaskPriority
 */
returnCode_t ATTR_SYSCALL sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    // Ignore unused parameters
    (void)(task);
    (void)(priority);

    // Call SVC exception
    __asm volatile(
        " .extern GetTaskPriority           \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne GetTaskPriority_unpriv        \n"
        " GetTaskPriority_priv :            \n"
        "   b GetTaskPriority               \n"
        " GetTaskPriority_unpriv :          \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_GET_TASK_PRIORITY) : "memory");
}

/**
 * @fn      sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief   Syscall declaration for SetTaskPriority
 */
returnCode_t ATTR_SYSCALL sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    // Ignore unused parameters
    (void)(task);
    (void)(priority);

    // Call SVC exception
    __asm volatile(
        " .extern SetTaskPriority           \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne SetTaskPriority_unpriv        \n"
        " SetTaskPriority_priv :            \n"
        "   b SetTaskPriority               \n"
        " SetTaskPriority_unpriv :          \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_SET_TASK_PRIORITY) : "memory");
}

/**
 * @fn      sys_AcquireMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for AcquireMutex
 */
returnCode_t ATTR_SYSCALL sys_AcquireMutex(mutexNo_t mutex)
{
    // Ignore unused parameters
    (void)(mutex);

    // Call SVC exception
    __asm volatile(
        " .extern AcquireMutex              \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne AcquireMutex_unpriv           \n"
        " AcquireMutex_priv :               \n"
        "   b AcquireMutex                  \n"
        " AcquireMutex_unpriv :             \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_ACQUIRE_MUTEX) : "memory");
}

/**
 * @fn      sys_ReleaseMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for ReleaseMutex
 */
returnCode_t ATTR_SYSCALL sys_ReleaseMutex(mutexNo_t mutex)
{
    // Ignore unused parameters
    (void)(mutex);

    // Call SVC exception
    __asm volatile(
        " .extern ReleaseMutex              \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne ReleaseMutex_unpriv           \n"
        " ReleaseMutex_priv :               \n"
        "   b ReleaseMutex                  \n"
        " ReleaseMutex_unpriv :             \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_RELEASE_MUTEX) : "memory");
}

/**
 * @fn      sys_SendSignal(taskNo_t task, signalMask_t mask)
 * @brief   Syscall declaration for SendSignal
 */
returnCode_t ATTR_SYSCALL sys_SendSignal(taskNo_t task, signalMask_t mask)
{
    // Ignore unused parameters
    (void)(task);
    (void)(mask);

    // Call SVC exception
    __asm volatile(
        " .extern SendSignal                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne SendSignal_unpriv             \n"
        " SendSignal_priv :                 \n"
        "   b SendSignal                    \n"
        " SendSignal_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_SEND_SIGNAL) : "memory");
}

/**
 * @fn      sys_WaitSignal(signalMask_t mask)
 * @brief   Syscall declaration for WaitSignal
 */
returnCode_t ATTR_SYSCALL sys_WaitSignal(signalMask_t mask)
{
    // Ignore unused parameters
    (void)(mask);

    // Call SVC exception
    __asm volatile(
        " .extern WaitSignal                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne WaitSignal_unpriv             \n"
        " WaitSignal_priv :                 \n"
        "   b WaitSignal                    \n"
        " WaitSignal_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_WAIT_SIGNAL) : "memory");
}

/**
 * @fn      sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
 * @brief   Syscall declaration for ConsolePrint
 */
void ATTR_SYSCALL sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
{
    // Ignore unused parameters
    (void)(msg);
    (void)(dnumber);
    (void)(hnumber);
    (void)(fnumber);
    (void)(fprecision);

    // Call SVC exception
    __asm volatile(
        " .extern ConsolePrint              \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne ConsolePrint_unpriv           \n"
        " ConsolePrint_priv :               \n"
        "   b ConsolePrint                  \n"
        " ConsolePrint_unpriv :             \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_CONSOLE_PRINT) : "memory");
}

/**
 * @fn      sys_EnableHK(hkId_t hkid)
 * @brief   Syscall declaration for EnableHK
 */
returnCode_t ATTR_SYSCALL sys_EnableHK(hkId_t hkid)
{
    // Ignore unused parameters
    (void)(hkid);

    // Call SVC exception
    __asm volatile(
        " .extern EnableHK                  \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne EnableHK_unpriv               \n"
        " EnableHK_priv :                   \n"
        "   b EnableHK                      \n"
        " EnableHK_unpriv :                 \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_ENABLE_HK) : "memory");
}

/**
 * @fn      sys_DisableHK(hkId_t hkid)
 * @brief   Syscall declaration for DisableHK
 */
returnCode_t ATTR_SYSCALL sys_DisableHK(hkId_t hkid)
{
    // Ignore unused parameters
    (void)(hkid);

    // Call SVC exception
    __asm volatile(
        " .extern DisableHK                 \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne DisableHK_unpriv              \n"
        " DisableHK_priv :                  \n"
        "   b DisableHK                     \n"
        " DisableHK_unpriv :                \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_DISABLE_HK) : "memory");
}

/**
 * @fn      sys_EmitHK(hk_t *hk)
 * @brief   Syscall declaration for EmitHK
 */
returnCode_t ATTR_SYSCALL sys_EmitHK(hk_t *hk)
{
    // Ignore unused parameters
    (void)(hk);

    // Call SVC exception
    __asm volatile(
        " .extern EmitHK                    \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne EmitHK_unpriv                 \n"
        " EmitHK_priv :                     \n"
        "   b EmitHK                        \n"
        " EmitHK_unpriv :                   \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_EMIT_HK) : "memory");
}

/**
 * @fn      sys_CollectHKs(void)
 * @brief   Syscall declaration for CollectHKs
 */
returnCode_t ATTR_SYSCALL sys_CollectHKs(void)
{
    // Call SVC exception
    __asm volatile(
        " .extern CollectHKs                \n"
        "                                   \n"
        " push {r0}                         \n"
        " mrs r0, control                   \n"
        " tst r0, #1                        \n"
        " pop {r0}                          \n"
        " bne CollectHKs_unpriv             \n"
        " CollectHKs_priv :                 \n"
        "   b CollectHKs                    \n"
        " CollectHKs_unpriv :               \n"
        "   svc %0                          \n"
        "                                   \n"
        : : "i"(SYSCALL_COLLECT_HKS) : "memory");
}
