/**
 * @file    syscalls.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining syscalls
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void sys_CheckError(returnCode_t retcode);
extern void sys_Sleep(tick_t tick);
extern void sys_SleepPeriodic(void);
extern tick_t sys_GetTick(void);
extern time_t sys_GetTime(void);
extern returnCode_t sys_SetTime(time_t time);
extern returnCode_t sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource);
extern returnCode_t sys_DeviceWrite(deviceNo_t device, data_t data, length_t length);
extern returnCode_t sys_DeviceRead(deviceNo_t device, data_t data, length_t length);
extern returnCode_t sys_DeviceIoctl(deviceNo_t device, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t sys_DeviceClose(deviceNo_t device);
extern taskNo_t sys_GetCurrentTask(void);
extern returnCode_t sys_SuspendTask(taskNo_t task);
extern returnCode_t sys_ResumeTask(taskNo_t task);
extern returnCode_t sys_GetTaskPriority(taskNo_t task, taskPriority_t *priority);
extern returnCode_t sys_SetTaskPriority(taskNo_t task, taskPriority_t priority);
extern returnCode_t sys_AcquireMutex(mutexNo_t mutex);
extern returnCode_t sys_ReleaseMutex(mutexNo_t mutex);
extern returnCode_t sys_SendSignal(taskNo_t task, signalMask_t mask);
extern returnCode_t sys_WaitSignal(signalMask_t mask);
extern returnCode_t sys_StartTimer(timerNo_t timer);
extern returnCode_t sys_PauseTimer(timerNo_t timer);
extern returnCode_t sys_ResumeTimer(timerNo_t timer);
extern returnCode_t sys_SetTimer(timerNo_t timer, tick_t period, timerMode_t mode);
extern void sys_ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision);
extern returnCode_t sys_EnableHK(hkId_t hkid);
extern returnCode_t sys_DisableHK(hkId_t hkid);
extern returnCode_t sys_GetLastHK(hkId_t hkid, hk_t *last_hk);
extern void sys_SVCExit(void);

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
    __asm volatile(" .extern CheckError                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne CheckError_unpriv             \n" //
                   " CheckError_priv :                 \n" // If privileged
                   "   b CheckError                    \n" // Directly execute the kernel function
                   " CheckError_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_CHECK_ERROR)    // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern Sleep                     \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne Sleep_unpriv                  \n" //
                   " Sleep_priv :                      \n" // If privileged
                   "   b Sleep                         \n" // Directly execute the kernel function
                   " Sleep_unpriv :                    \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_SLEEP)          // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_SleepPeriodic(void)
 * @brief   Syscall declaration for SleepPeriodic
 */
void ATTR_SYSCALL sys_SleepPeriodic(void)
{
    // Call SVC exception
    __asm volatile(" .extern SleepPeriodic             \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne SleepPeriodic_unpriv          \n" //
                   " SleepPeriodic_priv :              \n" // If privileged
                   "   b SleepPeriodic                 \n" // Directly execute the kernel function
                   " SleepPeriodic_unpriv :            \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_SLEEP_PERIODIC) // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_GetTick(void)
 * @brief   Syscall declaration for GetTick
 */
tick_t ATTR_SYSCALL sys_GetTick(void)
{
    // Call SVC exception
    __asm volatile(" .extern GetTick                   \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne GetTick_unpriv                \n" //
                   " GetTick_priv :                    \n" // If privileged
                   "   b GetTick                       \n" // Directly execute the kernel function
                   " GetTick_unpriv :                  \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_GET_TICK)       // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_GetTime(void)
 * @brief   Syscall declaration for GetTime
 */
time_t ATTR_SYSCALL sys_GetTime(void)
{
    // Call SVC exception
    __asm volatile(" .extern GetTime                   \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne GetTime_unpriv                \n" //
                   " GetTime_priv :                    \n" // If privileged
                   "   b GetTime                       \n" // Directly execute the kernel function
                   " GetTime_unpriv :                  \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_GET_TIME)       // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern SetTime                   \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne SetTime_unpriv                \n" //
                   " SetTime_priv :                    \n" // If privileged
                   "   b SetTime                       \n" // Directly execute the kernel function
                   " SetTime_unpriv :                  \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_SET_TIME)       // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
 * @brief   Syscall declaration for DeviceOpen
 */
returnCode_t ATTR_SYSCALL sys_DeviceOpen(deviceNo_t *device, deviceType_t type, uint32_t resource)
{
    // Ignore unused parameters
    (void)(device);
    (void)(type);
    (void)(resource);

    // Call SVC exception
    __asm volatile(" .extern DeviceOpen                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne DeviceOpen_unpriv             \n" //
                   " DeviceOpen_priv :                 \n" // If privileged
                   "   b DeviceOpen                    \n" // Directly execute the kernel function
                   " DeviceOpen_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_DEVICE_OPEN)    // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern DeviceWrite               \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne DeviceWrite_unpriv            \n" //
                   " DeviceWrite_priv :                \n" // If privileged
                   "   b DeviceWrite                   \n" // Directly execute the kernel function
                   " DeviceWrite_unpriv :              \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_DEVICE_WRITE)   // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern DeviceRead                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne DeviceRead_unpriv             \n" //
                   " DeviceRead_priv :                 \n" // If privileged
                   "   b DeviceRead                    \n" // Directly execute the kernel function
                   " DeviceRead_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_DEVICE_READ)    // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern DeviceIoctl               \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne DeviceIoctl_unpriv            \n" //
                   " DeviceIoctl_priv :                \n" // If privileged
                   "   b DeviceIoctl                   \n" // Directly execute the kernel function
                   " DeviceIoctl_unpriv :              \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_DEVICE_IOCTL)   // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern DeviceClose               \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne DeviceClose_unpriv            \n" //
                   " DeviceClose_priv :                \n" // If privileged
                   "   b DeviceClose                   \n" // Directly execute the kernel function
                   " DeviceClose_unpriv :              \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_DEVICE_CLOSE)   // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_GetCurrentTask(void)
 * @brief   Syscall declaration for GetCurrentTask
 */
taskNo_t ATTR_SYSCALL sys_GetCurrentTask(void)
{
    // Call SVC exception
    __asm volatile(" .extern GetCurrentTask            \n"   // Declare kernel function
                   "                                   \n"   //
                   " push {r0}                         \n"   // Save r0 on the stack
                   " mrs r0, control                   \n"   // Get control register
                   " tst r0, #1                        \n"   // Test privilege bit from the control register
                   " pop {r0}                          \n"   // Retrieve r0 from the stack
                   " bne GetCurrentTask_unpriv         \n"   //
                   " GetCurrentTask_priv :             \n"   // If privileged
                   "   b GetCurrentTask                \n"   // Directly execute the kernel function
                   " GetCurrentTask_unpriv :           \n"   // If not privileged
                   "   svc %[syscall]                  \n"   // Call the supervisor
                   "                                   \n"   //
                   :                                         // Output operands
                   : [syscall] "i"(SYSCALL_GET_CURRENT_TASK) // Input operands
                   : "memory");                              // Clobbered register
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
    __asm volatile(" .extern SuspendTask               \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne SuspendTask_unpriv            \n" //
                   " SuspendTask_priv :                \n" // If privileged
                   "   b SuspendTask                   \n" // Directly execute the kernel function
                   " SuspendTask_unpriv :              \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_SUSPEND_TASK)   // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern ResumeTask                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne ResumeTask_unpriv             \n" //
                   " ResumeTask_priv :                 \n" // If privileged
                   "   b ResumeTask                    \n" // Directly execute the kernel function
                   " ResumeTask_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_RESUME_TASK)    // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern GetTaskPriority           \n"    // Declare kernel function
                   "                                   \n"    //
                   " push {r0}                         \n"    // Save r0 on the stack
                   " mrs r0, control                   \n"    // Get control register
                   " tst r0, #1                        \n"    // Test privilege bit from the control register
                   " pop {r0}                          \n"    // Retrieve r0 from the stack
                   " bne GetTaskPriority_unpriv        \n"    //
                   " GetTaskPriority_priv :            \n"    // If privileged
                   "   b GetTaskPriority               \n"    // Directly execute the kernel function
                   " GetTaskPriority_unpriv :          \n"    // If not privileged
                   "   svc %[syscall]                  \n"    // Call the supervisor
                   "                                   \n"    //
                   :                                          // Output operands
                   : [syscall] "i"(SYSCALL_GET_TASK_PRIORITY) // Input operands
                   : "memory");                               // Clobbered register
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
    __asm volatile(" .extern SetTaskPriority           \n"    // Declare kernel function
                   "                                   \n"    //
                   " push {r0}                         \n"    // Save r0 on the stack
                   " mrs r0, control                   \n"    // Get control register
                   " tst r0, #1                        \n"    // Test privilege bit from the control register
                   " pop {r0}                          \n"    // Retrieve r0 from the stack
                   " bne SetTaskPriority_unpriv        \n"    //
                   " SetTaskPriority_priv :            \n"    // If privileged
                   "   b SetTaskPriority               \n"    // Directly execute the kernel function
                   " SetTaskPriority_unpriv :          \n"    // If not privileged
                   "   svc %[syscall]                  \n"    // Call the supervisor
                   "                                   \n"    //
                   :                                          // Output operands
                   : [syscall] "i"(SYSCALL_SET_TASK_PRIORITY) // Input operands
                   : "memory");                               // Clobbered register
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
    __asm volatile(" .extern AcquireMutex              \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne AcquireMutex_unpriv           \n" //
                   " AcquireMutex_priv :               \n" // If privileged
                   "   b AcquireMutex                  \n" // Directly execute the kernel function
                   " AcquireMutex_unpriv :             \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_ACQUIRE_MUTEX)  // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern ReleaseMutex              \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne ReleaseMutex_unpriv           \n" //
                   " ReleaseMutex_priv :               \n" // If privileged
                   "   b ReleaseMutex                  \n" // Directly execute the kernel function
                   " ReleaseMutex_unpriv :             \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_RELEASE_MUTEX)  // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern SendSignal                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne SendSignal_unpriv             \n" //
                   " SendSignal_priv :                 \n" // If privileged
                   "   b SendSignal                    \n" // Directly execute the kernel function
                   " SendSignal_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_SEND_SIGNAL)    // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern WaitSignal                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne WaitSignal_unpriv             \n" //
                   " WaitSignal_priv :                 \n" // If privileged
                   "   b WaitSignal                    \n" // Directly execute the kernel function
                   " WaitSignal_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_WAIT_SIGNAL)    // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_StartTimer(timerNo_t timer)
 * @brief   Syscall declaration for StartTimer
 */
returnCode_t ATTR_SYSCALL sys_StartTimer(timerNo_t timer)
{
    // Ignore unused parameters
    (void)(timer);
    // Call SVC exception
    __asm volatile(" .extern StartTimer                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne StartTimer_unpriv             \n" //
                   " StartTimer_priv :                 \n" // If privileged
                   "   b StartTimer                    \n" // Directly execute the kernel function
                   " StartTimer_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_START_TIMER)    // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_PauseTimer(timerNo_t timer)
 * @brief   Syscall declaration for PauseTimer
 */
returnCode_t ATTR_SYSCALL sys_PauseTimer(timerNo_t timer)
{
    // Ignore unused parameters
    (void)(timer);
    // Call SVC exception
    __asm volatile(" .extern PauseTimer                \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne PauseTimer_unpriv             \n" //
                   " PauseTimer_priv :                 \n" // If privileged
                   "   b PauseTimer                    \n" // Directly execute the kernel function
                   " PauseTimer_unpriv :               \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_PAUSE_TIMER)    // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_ResumeTimer(timerNo_t timer)
 * @brief   Syscall declaration for ResumeTimer
 */
returnCode_t ATTR_SYSCALL sys_ResumeTimer(timerNo_t timer)
{
    // Ignore unused parameters
    (void)(timer);
    // Call SVC exception
    __asm volatile(" .extern ResumeTimer               \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne ResumeTimer_unpriv            \n" //
                   " ResumeTimer_priv :                \n" // If privileged
                   "   b ResumeTimer                   \n" // Directly execute the kernel function
                   " ResumeTimer_unpriv :              \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_RESUME_TIMER)   // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
 * @brief   Syscall declaration for SetTimer
 */
returnCode_t ATTR_SYSCALL sys_SetTimer(timerNo_t timer, tick_t period, timerMode_t mode)
{
    // Ignore unused parameters
    (void)(timer);
    (void)(period);
    (void)(mode);
    // Call SVC exception
    __asm volatile(" .extern SetTimer                  \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne SetTimer_unpriv               \n" //
                   " SetTimer_priv :                   \n" // If privileged
                   "   b SetTimer                      \n" // Directly execute the kernel function
                   " SetTimer_unpriv :                 \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_SET_TIMER)      // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern ConsolePrint              \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne ConsolePrint_unpriv           \n" //
                   " ConsolePrint_priv :               \n" // If privileged
                   "   b ConsolePrint                  \n" // Directly execute the kernel function
                   " ConsolePrint_unpriv :             \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_CONSOLE_PRINT)  // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern EnableHK                  \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne EnableHK_unpriv               \n" //
                   " EnableHK_priv :                   \n" // If privileged
                   "   b EnableHK                      \n" // Directly execute the kernel function
                   " EnableHK_unpriv :                 \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_ENABLE_HK)      // Input operands
                   : "memory");                            // Clobbered register
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
    __asm volatile(" .extern DisableHK                 \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne DisableHK_unpriv              \n" //
                   " DisableHK_priv :                  \n" // If privileged
                   "   b DisableHK                     \n" // Directly execute the kernel function
                   " DisableHK_unpriv :                \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_DISABLE_HK)     // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_GetLastHK(hkId_t hkid, hk_t *last_hk)
 * @brief   Syscall declaration for GetLastHK
 */
returnCode_t ATTR_SYSCALL sys_GetLastHK(hkId_t hkid, hk_t *last_hk)
{
    // Ignore unused parameters
    (void)(hkid);
    (void)(last_hk);

    // Call SVC exception
    __asm volatile(" .extern GetLastHK                 \n" // Declare kernel function
                   "                                   \n" //
                   " push {r0}                         \n" // Save r0 on the stack
                   " mrs r0, control                   \n" // Get control register
                   " tst r0, #1                        \n" // Test privilege bit from the control register
                   " pop {r0}                          \n" // Retrieve r0 from the stack
                   " bne EmitHK_unpriv                 \n" //
                   " EmitHK_priv :                     \n" // If privileged
                   "   b GetLastHK                     \n" // Directly execute the kernel function
                   " EmitHK_unpriv :                   \n" // If not privileged
                   "   svc %[syscall]                  \n" // Call the supervisor
                   "                                   \n" //
                   :                                       // Output operands
                   : [syscall] "i"(SYSCALL_GET_LAST_HK)    // Input operands
                   : "memory");                            // Clobbered register
}

/**
 * @fn      sys_SVCExit(void)
 * @brief   Call SVCExit using a system call (internally)
 *
 * This is the only syscall that should be called from the kernel and not the user space,
 * it needs to stay in the kernel memory space, that's why it does not have the attribute
 * ATTR_SYSCALL.
 */
void ATTR_NAKED sys_SVCExit(void)
{
    // Call SVC exception
    __asm volatile("svc %0 \n"         // Call exit supervisor call
                   :                   // Output operands
                   : "i"(SYSCALL_EXIT) // Input operands
                   : "memory");        // Clobbered register
}
