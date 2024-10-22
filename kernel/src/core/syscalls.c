/**
 * @file    syscalls.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining syscalls
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/os.h"
#include "core/time.h"
#include "core/tasks.h"
#include "core/buffers.h"
#include "core/mutex.h"
#include "core/devices.h"
#include "system/console.h"
#include "system/housekeeping.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define SYSTEM_CALL      __attribute__((section(".syscalls"))) __attribute__((naked))   /**< Macro setting function attributes for a syscall */

/*************************** Functions Declarations **************************/

extern void InitializeFirstTaskContext(void);

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

extern void SVC_Handler(void);

/*************************** Variables Definitions ***************************/

extern const uint32_t syscall_vector[NB_SYSCALLS];

/**
 * @brief Syscall Vector Table
 */
const uint32_t syscall_vector[NB_SYSCALLS] = {
    (uint32_t)InitializeFirstTaskContext,  // SVC 0
    (uint32_t)CheckError,                  // SYSCALL_CHECK_ERROR
    (uint32_t)Sleep,                       // SYSCALL_SLEEP
    (uint32_t)SleepPeriodic,               // SYSCALL_SLEEP_PERIODIC
    (uint32_t)GetTick,                     // SYSCALL_GET_TICK
    (uint32_t)GetTime,                     // SYSCALL_GET_TIME
    (uint32_t)SetTime,                     // SYSCALL_SET_TIME
    (uint32_t)DeviceOpen,                  // SYSCALL_DEVICE_OPEN
    (uint32_t)DeviceWrite,                 // SYSCALL_DEVICE_WRITE
    (uint32_t)DeviceRead,                  // SYSCALL_DEVICE_READ
    (uint32_t)DeviceIoctl,                 // SYSCALL_DEVICE_IOCTL
    (uint32_t)DeviceClose,                 // SYSCALL_DEVICE_CLOSE
    (uint32_t)GetCurrentTask,              // SYSCALL_GET_CURRENT_TASK
    (uint32_t)SuspendTask,                 // SYSCALL_SUSPEND_TASK
    (uint32_t)ResumeTask,                  // SYSCALL_RESUME_TASK
    (uint32_t)GetTaskPriority,             // SYSCALL_GET_TASK_PRIORITY
    (uint32_t)SetTaskPriority,             // SYSCALL_SET_TASK_PRIORITY
    (uint32_t)AcquireMutex,                // SYSCALL_ACQUIRE_MUTEX
    (uint32_t)ReleaseMutex,                // SYSCALL_RELEASE_MUTEX
    (uint32_t)ConsolePrint,                // SYSCALL_CONSOLE_PRINT
    (uint32_t)EnableHK,                    // SYSCALL_ENABLE_HK
    (uint32_t)DisableHK,                   // SYSCALL_DISABLE_HK
    (uint32_t)EmitHK,                      // SYSCALL_EMIT_HK
    (uint32_t)CollectHKs                   // SYSCALL_COLLECT_HKS
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitializeFirstTaskContext(void)
 * @brief   Initialize the context for the first stack when scheduler starts
 */
void __attribute__((naked)) InitializeFirstTaskContext(void)
{
    __asm volatile (
        "   ldr r3, pxCurrentTCBConst2      \n" /* Restore the context. */
        "   ldr r1, [r3]                    \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
        "   ldr r0, [r1]                    \n" /* The first item in pxCurrentTCB is the task top of stack. */
        "   ldmia r0!, {r4-r11, r14}        \n" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
        "   msr psp, r0                     \n" /* Restore the task stack pointer. */
        "   isb                             \n"
        "   mov r0, #0                      \n"
        "   msr basepri, r0                 \n"
        "   bx lr                          \n"
        "                                   \n"
        "   .align 4                        \n"
        "pxCurrentTCBConst2: .word pxCurrentTCB \n"
    );
}

/**
 * @fn      sys_CheckError(returnCode_t retcode)
 * @brief   Syscall declaration for CheckError
 */
void SYSTEM_CALL sys_CheckError(returnCode_t retcode)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_CHECK_ERROR) : "memory"
    );

    // Ignore unused parameters
    (void)(retcode);
}

/**
 * @fn      sys_Sleep(tick_t tick)
 * @brief   Syscall declaration for Sleep
 */
void SYSTEM_CALL sys_Sleep(tick_t tick)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_SLEEP) : "memory"
    );

    // Ignore unused parameters
    (void)(tick);
}

/**
 * @fn      sys_SleepPeriodic(void)
 * @brief   Syscall declaration for SleepPeriodic
 */
void SYSTEM_CALL sys_SleepPeriodic(void)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_SLEEP_PERIODIC) : "memory"
    );
}

/**
 * @fn      sys_GetTick(void)
 * @brief   Syscall declaration for GetTick
 */
tick_t SYSTEM_CALL sys_GetTick(void)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_GET_TICK) : "memory"
    );
}

/**
 * @fn      sys_GetTime(time_t *time)
 * @brief   Syscall declaration for GetTime
 */
returnCode_t SYSTEM_CALL sys_GetTime(time_t *time)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_GET_TIME) : "memory"
    );

    // Ignore unused parameters
    (void)(time);
}

/**
 * @fn      sys_SetTime(time_t time)
 * @brief   Syscall declaration for SetTime
 */
returnCode_t SYSTEM_CALL sys_SetTime(time_t time)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_SET_TIME) : "memory"
    );

    // Ignore unused parameters
    (void)(time);
}

/**
 * @fn      sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
 * @brief   Syscall declaration for DeviceOpen
 */
returnCode_t SYSTEM_CALL sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t ressource, uint32_t extra_info)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_DEVICE_OPEN) : "memory"
    );

    // Ignore unused parameters
    (void)(device);
    (void)(type);
    (void)(ressource);
    (void)(extra_info);
}

/**
 * @fn      sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceWrite
 */
returnCode_t SYSTEM_CALL sys_DeviceWrite(deviceNo_t device, data_t data, length_t length)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_DEVICE_WRITE) : "memory"
    );

    // Ignore unused parameters
    (void)(device);
    (void)(data);
    (void)(length);
}

/**
 * @fn      sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
 * @brief   Syscall declaration for DeviceRead
 */
returnCode_t SYSTEM_CALL sys_DeviceRead(deviceNo_t device, data_t data, length_t length)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_DEVICE_READ) : "memory"
    );

    // Ignore unused parameters
    (void)(device);
    (void)(data);
    (void)(length);
}

/**
 * @fn      sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
 * @brief   Syscall declaration for DeviceIoctl
 */
returnCode_t SYSTEM_CALL sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_DEVICE_IOCTL) : "memory"
    );

    // Ignore unused parameters
    (void)(device);
    (void)(cmd);
    (void)(data);
    (void)(data_size);
}

/**
 * @fn      sys_DeviceClose(deviceNo_t device)
 * @brief   Syscall declaration for DeviceClose
 */
returnCode_t SYSTEM_CALL sys_DeviceClose(deviceNo_t device)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_DEVICE_CLOSE) : "memory"
    );

    // Ignore unused parameters
    (void)(device);
}

/**
 * @fn      sys_GetCurrentTask(taskNo_t *task)
 * @brief   Syscall declaration for GetCurrentTask
 */
returnCode_t SYSTEM_CALL sys_GetCurrentTask(taskNo_t *task)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_GET_CURRENT_TASK) : "memory"
    );

    // Ignore unused parameters
    (void)(task);
}

/**
 * @fn      sys_SuspendTask(taskNo_t task)
 * @brief   Syscall declaration for SuspendTask
 */
returnCode_t SYSTEM_CALL sys_SuspendTask(taskNo_t task)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_SUSPEND_TASK) : "memory"
    );

    // Ignore unused parameters
    (void)(task);
}

/**
 * @fn      sys_ResumeTask(taskNo_t task)
 * @brief   Syscall declaration for ResumeTask
 */
returnCode_t SYSTEM_CALL sys_ResumeTask(taskNo_t task)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_RESUME_TASK) : "memory"
    );

    // Ignore unused parameters
    (void)(task);
}

/**
 * @fn      sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief   Syscall declaration for GetTaskPriority
 */
returnCode_t SYSTEM_CALL sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_GET_TASK_PRIORITY) : "memory"
    );

    // Ignore unused parameters
    (void)(task);
    (void)(priority);
}

/**
 * @fn      sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief   Syscall declaration for SetTaskPriority
 */
returnCode_t SYSTEM_CALL sys_SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_SET_TASK_PRIORITY) : "memory"
    );

    // Ignore unused parameters
    (void)(task);
    (void)(priority);
}

/**
 * @fn      sys_AcquireMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for AcquireMutex
 */
returnCode_t SYSTEM_CALL sys_AcquireMutex(mutexNo_t mutex)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_ACQUIRE_MUTEX) : "memory"
    );

    // Ignore unused parameters
    (void)(mutex);
}

/**
 * @fn      sys_ReleaseMutex(mutexNo_t mutex)
 * @brief   Syscall declaration for ReleaseMutex
 */
returnCode_t SYSTEM_CALL sys_ReleaseMutex(mutexNo_t mutex)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_RELEASE_MUTEX) : "memory"
    );

    // Ignore unused parameters
    (void)(mutex);
}

/**
 * @fn      sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
 * @brief   Syscall declaration for ConsolePrint
 */
void SYSTEM_CALL sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_CONSOLE_PRINT) : "memory"
    );

    // Ignore unused parameters
    (void)(msg);
    (void)(dnumber);
    (void)(hnumber);
    (void)(fnumber);
    (void)(fprecision);
}

/**
 * @fn      sys_EnableHK(hkId_t hkid)
 * @brief   Syscall declaration for EnableHK
 */
returnCode_t SYSTEM_CALL sys_EnableHK(hkId_t hkid)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_ENABLE_HK) : "memory"
    );

    // Ignore unused parameters
    (void)(hkid);
}

/**
 * @fn      sys_DisableHK(hkId_t hkid)
 * @brief   Syscall declaration for DisableHK
 */
returnCode_t SYSTEM_CALL sys_DisableHK(hkId_t hkid)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_DISABLE_HK) : "memory"
    );

    // Ignore unused parameters
    (void)(hkid);
}

/**
 * @fn      sys_EmitHK(hk_t *hk)
 * @brief   Syscall declaration for EmitHK
 */
returnCode_t SYSTEM_CALL sys_EmitHK(hk_t *hk)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_EMIT_HK) : "memory"
    );

    // Ignore unused parameters
    (void)(hk);
}

/**
 * @fn      sys_CollectHKs(void)
 * @brief   Syscall declaration for CollectHKs
 */
returnCode_t SYSTEM_CALL sys_CollectHKs(void)
{
    __asm volatile
    (
        "svc %0 \n"
        : : "i" (SYSCALL_COLLECT_HKS) : "memory"
    );
}

/************************ Exception Handler Definition ***********************/

/**
 * @fn      SVC_Handler(void)
 * @brief   SuperVisor Call exception handler  
 */
void __attribute__((naked)) SVC_Handler(void)
{
    __asm volatile
    (
        "   tst lr, #4                       \n" // Test EXC_RETURN to see which stack pointer we are using (MSP or PSP).
        "   ite eq                          \n"
        "   mrseq r0, msp                   \n" // If equal, MSP was used, move it to R0.
        "   mrsne r0, psp                   \n" // Otherwise, PSP was used, move it to R0.
        "   ldr r1, [r0, #24]               \n" // R1 = stacked PC
        "   ldrb r1, [r1, #-2]              \n" // R1 = stacked PC - 2, now R1 contains the SVC number.

        "   ldr r2, =syscall_vector         \n" // Load the base address of syscall_vector into R2.
        "   ldr r3, [r2, r1, lsl #2]        \n" // Get the address of the syscall function using R1 as an index.
        "   blx r3                          \n" // Branch to the function pointed to by R3.

        "   bx lr                          \n"
    );
}
