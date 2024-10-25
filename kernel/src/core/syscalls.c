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

#define SYSTEM_CALL      __attribute__((section(".syscalls"))) __attribute__((naked))   /**< Macro setting function attributes for a syscall */

/*************************** Functions Declarations **************************/

extern void sys_CheckError(returnCode_t retcode);
extern void sys_Sleep(tick_t tick);
extern void sys_SleepPeriodic(void);
extern tick_t sys_GetTick(void);
extern returnCode_t sys_GetTime(time_t *time);
extern returnCode_t sys_SetTime(time_t time);
extern returnCode_t sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info);
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
void SYSTEM_CALL sys_CheckError(returnCode_t retcode)
{
    // Ignore unused parameters
    (void)(retcode);
    
    // Call SVC exception
    __asm volatile ("svc %0 \n" : : "i" (SYSCALL_CHECK_ERROR) : "memory");
}

/**
 * @fn      sys_Sleep(tick_t tick)
 * @brief   Syscall declaration for Sleep
 */
void SYSTEM_CALL sys_Sleep(tick_t tick)
{
    // Ignore unused parameters
    (void)(tick);

    // Call SVC exception
    __asm volatile ("svc %0 \n" : : "i" (SYSCALL_SLEEP) : "memory");
}

/**
 * @fn      sys_SleepPeriodic(void)
 * @brief   Syscall declaration for SleepPeriodic
 */
void SYSTEM_CALL sys_SleepPeriodic(void)
{
    // Call SVC exception
    __asm volatile ("svc %0 \n" :: "i" (SYSCALL_SLEEP_PERIODIC) : "memory" );
}

/**
 * @fn      sys_GetTick(void)
 * @brief   Syscall declaration for GetTick
 */
tick_t SYSTEM_CALL sys_GetTick(void)
{
    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_GET_TICK) : "memory" );
}

/**
 * @fn      sys_GetTime(time_t *time)
 * @brief   Syscall declaration for GetTime
 */
returnCode_t SYSTEM_CALL sys_GetTime(time_t *time)
{
    // Ignore unused parameters
    (void)(time);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_GET_TIME) : "memory" );
}

/**
 * @fn      sys_SetTime(time_t time)
 * @brief   Syscall declaration for SetTime
 */
returnCode_t SYSTEM_CALL sys_SetTime(time_t time)
{
    // Ignore unused parameters
    (void)(time);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_SET_TIME) : "memory" );
}

/**
 * @fn      sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
 * @brief   Syscall declaration for DeviceOpen
 */
returnCode_t SYSTEM_CALL sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
{
    // Ignore unused parameters
    (void)(device);
    (void)(type);
    (void)(ressource);
    (void)(extra_info);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_DEVICE_OPEN) : "memory" );
}

/**
 * @fn      sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceWrite
 */
returnCode_t SYSTEM_CALL sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    // Ignore unused parameters
    (void)(device);
    (void)(data);
    (void)(length);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_DEVICE_WRITE) : "memory" );
}

/**
 * @fn      sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceRead
 */
returnCode_t SYSTEM_CALL sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    // Ignore unused parameters
    (void)(device);
    (void)(data);
    (void)(length);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_DEVICE_READ) : "memory" );
}

/**
 * @fn      sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief   Syscall declaration for DeviceIoctl
 */
returnCode_t SYSTEM_CALL sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    // Ignore unused parameters
    (void)(device);
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_DEVICE_IOCTL) : "memory" );
}

/**
 * @fn      sys_DeviceClose(deviceNo_t device)
 * @brief   Syscall declaration for DeviceClose
 */
returnCode_t SYSTEM_CALL sys_DeviceClose(deviceNo_t device)
{
    // Ignore unused parameters
    (void)(device);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_DEVICE_CLOSE) : "memory" );
}

/**
 * @fn      sys_GetCurrentTask(taskNo_t *task)
 * @brief   Syscall declaration for GetCurrentTask
 */
returnCode_t SYSTEM_CALL sys_GetCurrentTask(taskNo_t *task)
{
    // Ignore unused parameters
    (void)(task);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_GET_CURRENT_TASK) : "memory" );    
}

/**
 * @fn      sys_SuspendTask(taskNo_t task)
 * @brief   Syscall declaration for SuspendTask
 */
returnCode_t SYSTEM_CALL sys_SuspendTask(taskNo_t task)
{
    // Ignore unused parameters
    (void)(task);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_SUSPEND_TASK) : "memory" );
}

/**
 * @fn      sys_ResumeTask(taskNo_t task)
 * @brief   Syscall declaration for ResumeTask
 */
returnCode_t SYSTEM_CALL sys_ResumeTask(taskNo_t task)
{
    // Ignore unused parameters
    (void)(task);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_RESUME_TASK) : "memory" );
}

/**
 * @fn      sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief   Syscall declaration for GetTaskPriority
 */
returnCode_t SYSTEM_CALL sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    // Ignore unused parameters
    (void)(task);
    (void)(priority);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_GET_TASK_PRIORITY) : "memory" );
}

/**
 * @fn      sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief   Syscall declaration for SetTaskPriority
 */
returnCode_t SYSTEM_CALL sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    // Ignore unused parameters
    (void)(task);
    (void)(priority);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_SET_TASK_PRIORITY) : "memory" );    
}

/**
 * @fn      sys_AcquireMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for AcquireMutex
 */
returnCode_t SYSTEM_CALL sys_AcquireMutex(mutexNo_t mutex)
{
    // Ignore unused parameters
    (void)(mutex);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_ACQUIRE_MUTEX) : "memory" );
}

/**
 * @fn      sys_ReleaseMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for ReleaseMutex
 */
returnCode_t SYSTEM_CALL sys_ReleaseMutex(mutexNo_t mutex)
{
    // Ignore unused parameters
    (void)(mutex);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_RELEASE_MUTEX) : "memory" );    
}

/**
 * @fn      sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
 * @brief   Syscall declaration for ConsolePrint
 */
void SYSTEM_CALL sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
{
    // Ignore unused parameters
    (void)(msg);
    (void)(dnumber);
    (void)(hnumber);
    (void)(fnumber);
    (void)(fprecision);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_CONSOLE_PRINT) : "memory" );
}

/**
 * @fn      sys_EnableHK(hkId_t hkid)
 * @brief   Syscall declaration for EnableHK
 */
returnCode_t SYSTEM_CALL sys_EnableHK(hkId_t hkid)
{
    // Ignore unused parameters
    (void)(hkid);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_ENABLE_HK) : "memory" );
}

/**
 * @fn      sys_DisableHK(hkId_t hkid)
 * @brief   Syscall declaration for DisableHK
 */
returnCode_t SYSTEM_CALL sys_DisableHK(hkId_t hkid)
{
    // Ignore unused parameters
    (void)(hkid);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_DISABLE_HK) : "memory" );
}

/**
 * @fn      sys_EmitHK(hk_t *hk)
 * @brief   Syscall declaration for EmitHK
 */
returnCode_t SYSTEM_CALL sys_EmitHK(hk_t *hk)
{
    // Ignore unused parameters
    (void)(hk);

    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_EMIT_HK) : "memory" );
}

/**
 * @fn      sys_CollectHKs(void)
 * @brief   Syscall declaration for CollectHKs
 */
returnCode_t SYSTEM_CALL sys_CollectHKs(void)
{
    // Call SVC exception
    __asm volatile ( "svc %0 \n" : : "i" (SYSCALL_COLLECT_HKS) : "memory" );
}
