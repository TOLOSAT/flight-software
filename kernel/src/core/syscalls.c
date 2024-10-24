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

static void InitializeFirstTaskContext(void);
static void sys_SVCExit(void);
static void SVCEntry(uint32_t *p_stack, uint32_t exc_return, uint32_t svc_no);
static void SVCExit(uint32_t *p_stack, uint32_t exc_return, uint32_t svc_no);
extern void SVC_Handler(void);

/*************************** Variables Definitions ***************************/

extern const uint32_t syscall_vector[NB_SYSCALLS];

/**
 * @brief Syscall Vector Table
 */
const uint32_t syscall_vector[NB_SYSCALLS] = {
    (uint32_t)ErrorHandler,     // RESERVED DO NOT USE
    (uint32_t)CheckError,       // SYSCALL_CHECK_ERROR
    (uint32_t)Sleep,            // SYSCALL_SLEEP
    (uint32_t)SleepPeriodic,    // SYSCALL_SLEEP_PERIODIC
    (uint32_t)GetTick,          // SYSCALL_GET_TICK
    (uint32_t)GetTime,          // SYSCALL_GET_TIME
    (uint32_t)SetTime,          // SYSCALL_SET_TIME
    (uint32_t)DeviceOpen,       // SYSCALL_DEVICE_OPEN
    (uint32_t)DeviceWrite,      // SYSCALL_DEVICE_WRITE
    (uint32_t)DeviceRead,       // SYSCALL_DEVICE_READ
    (uint32_t)DeviceIoctl,      // SYSCALL_DEVICE_IOCTL
    (uint32_t)DeviceClose,      // SYSCALL_DEVICE_CLOSE
    (uint32_t)GetCurrentTask,   // SYSCALL_GET_CURRENT_TASK
    (uint32_t)SuspendTask,      // SYSCALL_SUSPEND_TASK
    (uint32_t)ResumeTask,       // SYSCALL_RESUME_TASK
    (uint32_t)GetTaskPriority,  // SYSCALL_GET_TASK_PRIORITY
    (uint32_t)SetTaskPriority,  // SYSCALL_SET_TASK_PRIORITY
    (uint32_t)AcquireMutex,     // SYSCALL_ACQUIRE_MUTEX
    (uint32_t)ReleaseMutex,     // SYSCALL_RELEASE_MUTEX
    (uint32_t)ConsolePrint,     // SYSCALL_CONSOLE_PRINT
    (uint32_t)EnableHK,         // SYSCALL_ENABLE_HK
    (uint32_t)DisableHK,        // SYSCALL_DISABLE_HK
    (uint32_t)EmitHK,           // SYSCALL_EMIT_HK
    (uint32_t)CollectHKs        // SYSCALL_COLLECT_HKS
};

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

/*************************** System Calls Handling ***************************/

/**
 * @fn      InitializeFirstTaskContext(void)
 * @brief   Initialize the context for the first stack when scheduler starts
 * @note    Retrieved from FreeRTOS
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
 * @fn      sys_SVCExit(void)
 * @brief   Syscall declaration for SVCExit
 */
static void sys_SVCExit(void)
{
    // Call SVC exception
    __asm volatile ( "svc %0 \n" ::"i" (SYSCALL_EXIT) : "memory" );
}

/**
 * @fn          SVCEntry(uint32_t *p_stack, uint32_t exc_return, uint32_t svc_no)
 * @brief       Function that executes syscalls
 * @param[in]   p_stack     Pointer to the stack before interruption
 * @param[in]   exc_return  EXC_RETURN value (contains information about the processor state before the exception)
 * @param[in]   svc_no      SuperVisor Call numero
 * @note        Largely based on FreeRTOS syscall management for MPUs.
 */
static void SVCEntry(uint32_t *p_stack, uint32_t exc_return, uint32_t svc_no)
{
    // Unused for the moment
    (void)(p_stack);
    (void)(exc_return);
    (void)(svc_no);
    (void)(sys_SVCExit);
}

/**
 * @fn          SVCExit(uint32_t *p_stack, uint32_t exc_return, uint32_t svc_no)
 * @brief       Function that returns from syscalls
 * @param[in]   p_stack     Pointer to the stack before interruption
 * @param[in]   exc_return  EXC_RETURN value (contains information about the processor state before the exception)
 * @param[in]   svc_no      SuperVisor Call numero
 * @note        Largely based on FreeRTOS syscall management for MPUs.
 */
static void SVCExit(uint32_t *p_stack, uint32_t exc_return, uint32_t svc_no)
{
    // Unused for the moment
    (void)(p_stack);
    (void)(exc_return);
    (void)(svc_no);
}

/**
 * @fn      SVC_Handler(void)
 * @brief   SuperVisor Call exception handler  
 * @note    Largely based on FreeRTOS syscall management for MPUs.
 */
void __attribute__((naked)) SVC_Handler(void)
{
    __asm volatile
        (
            ".syntax unified                \n"
            ".extern ErrorHandler           \n"
            "                               \n"
            "tst lr, #4                     \n" // Get EXC_RETURN 3rd bit value 
            "ite eq                         \n" // Check if the bit is equal to 0
            "mrseq r0, msp                  \n" // If yes then store the msp to r0
            "mrsne r0, psp                  \n" // If bo then store the psp to r0
            "ldr r1, [r0, #24]              \n" // Get pc address that execute 'svc' instruction
            "ldrb r2, [r1, #-2]             \n" // Store the svc_no (immediate value) to r2
            "mov r1, lr                     \n" // Store the EXC_RETURN to r1
            "                               \n"
            "cmp r2, #0                     \n" // Compare svc_no to 0 (first stack initialisation called by the OS initialisation)
            "beq %0                         \n" // If equal go to InitializeFirstTaskContext
            "cmp r2, %1                     \n" // Compare svc_no to NB_SYSCALLS
            "blt syscall_execute            \n" // If inferior then execute the syscall
            "cmp r2, %2                     \n" // Else compare to the exit syscall numero
            "beq syscall_exit               \n" // If equal then go to the exit procedure
            "b ErrorHandler                 \n" // Else go to the error Handler
            "                               \n"
            "syscall_execute:               \n"
            "    mov r1, lr                 \n" // Get the lr
            "    b %3                       \n" // Go to the SVCEntry function
            "                               \n"
            "syscall_exit:                  \n"
            "    mov r1, lr                 \n" // Get the lr
            "    b %4                       \n" // Go to the SVCEntry function
            "                               \n"
            : /* No outputs. */
            : "i" (InitializeFirstTaskContext),"i" (NB_SYSCALLS), "i" (SYSCALL_EXIT), "i" (SVCEntry), "i" (SVCExit)
            : "r0", "r1", "r2", "memory"
        );
}
