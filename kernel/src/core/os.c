/**
 * @file    os.c
 * @author  Merlin Kooshmanian
 * @brief   OS API source file
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
#include "bsp.h"

/***************************** Macros Definitions ****************************/

#define OFFSET_TO_PC                    6u      /**< Offset in the stack frame to get the PC */
#define OFFSET_TO_LR                    5u      /**< Offset in the stack frame to get the LR */

#define INITIAL_CONTROL_IF_UNPRIVILEGED 0x03u   /**< Default value for 'control' register value when task is unprivileged */
#define INITIAL_CONTROL_IF_PRIVILEGED   0x02u   /**< Default value for 'control' register value when task is privileged */

/*************************** Functions Declarations **************************/

extern void vInitTaskPrivilege(TaskHandle_t xTask, BaseType_t xRunPrivileged);

extern void vApplicationIdleHook(void);
extern void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

extern void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
extern void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

extern StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters);

static void InitializeFirstTaskContext(void);
static void CallSVCExit(void);
static void SVCEntry(uint32_t *p_stack, uint32_t svc_no);
static void SVCExit(uint32_t *p_stack);

extern void SVC_Handler(void);
extern void PendSV_Handler(void);

/*************************** Variables Definitions ***************************/

extern const uint32_t syscall_vector[NB_SYSCALLS];

/**
 * @brief Syscall Vector Table
 */
const uint32_t syscall_vector[NB_SYSCALLS] = {
    (uint32_t)ErrorHandler,    // RESERVED DO NOT USE
    (uint32_t)CheckError,      // SYSCALL_CHECK_ERROR
    (uint32_t)Sleep,           // SYSCALL_SLEEP
    (uint32_t)SleepPeriodic,   // SYSCALL_SLEEP_PERIODIC
    (uint32_t)GetTick,         // SYSCALL_GET_TICK
    (uint32_t)GetTime,         // SYSCALL_GET_TIME
    (uint32_t)SetTime,         // SYSCALL_SET_TIME
    (uint32_t)DeviceOpen,      // SYSCALL_DEVICE_OPEN
    (uint32_t)DeviceWrite,     // SYSCALL_DEVICE_WRITE
    (uint32_t)DeviceRead,      // SYSCALL_DEVICE_READ
    (uint32_t)DeviceIoctl,     // SYSCALL_DEVICE_IOCTL
    (uint32_t)DeviceClose,     // SYSCALL_DEVICE_CLOSE
    (uint32_t)GetCurrentTask,  // SYSCALL_GET_CURRENT_TASK
    (uint32_t)SuspendTask,     // SYSCALL_SUSPEND_TASK
    (uint32_t)ResumeTask,      // SYSCALL_RESUME_TASK
    (uint32_t)GetTaskPriority, // SYSCALL_GET_TASK_PRIORITY
    (uint32_t)SetTaskPriority, // SYSCALL_SET_TASK_PRIORITY
    (uint32_t)AcquireMutex,    // SYSCALL_ACQUIRE_MUTEX
    (uint32_t)ReleaseMutex,    // SYSCALL_RELEASE_MUTEX
    (uint32_t)ConsolePrint,    // SYSCALL_CONSOLE_PRINT
    (uint32_t)EnableHK,        // SYSCALL_ENABLE_HK
    (uint32_t)DisableHK,       // SYSCALL_DISABLE_HK
    (uint32_t)EmitHK,          // SYSCALL_EMIT_HK
    (uint32_t)CollectHKs       // SYSCALL_COLLECT_HKS
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      StartOS(void)
 * @brief   Function that starts the OS
 * @return  Nothing
 */
void StartOS(void)
{
    vTaskStartScheduler();
}

/**
 * @fn      vInitTaskPrivilege(TaskHandle_t xTask, BaseType_t xRunPrivileged)
 * @brief   Function that sets the task privilege
 */
void vInitTaskPrivilege(TaskHandle_t xTask, BaseType_t xRunPrivileged)
{
    // Variable Initialisation
    StackType_t **ppxTopOfStack = (StackType_t **)xTask;
    StackType_t *pxTopOfStack = *ppxTopOfStack;

    // Update r3 with the initial control register for the task
    if (xRunPrivileged != pdTRUE)
    {
        *pxTopOfStack = INITIAL_CONTROL_IF_UNPRIVILEGED; // Remove privilege by set up R3 (a.k.a. control saved here)
    }

    // Update pxTopOfStack in xTask
    *ppxTopOfStack = pxTopOfStack;
}

/**************************** OS Hooks Definitions ***************************/

/**
 * @fn      vApplicationIdleHook(void)
 * @brief   Function called by the Idle Task
 * @return  Nothing
 */
void vApplicationIdleHook(void)
{
    // Wait for Interrupt instruction puts the
    // cpu in sleep until the next interrupt.
    // It will reduce a bit the consumption when
    // the system is not overloaded.
    __WFI();
}

/**
 * @fn      vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
 * @brief   This function is executed if a task runs out of stack
 * @return  Nothing
 *
 * Os specific function that need to be provided if stack overflow hook is used
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // Unused Parameters
    (void)xTask;
    (void)pcTaskName;

    // Function Core
    ErrorHandler();
}

/**
 * @fn      vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
 * @brief   This function is used to allocate memory to Idle Task when scheduler is started
 * @return  Nothing
 *
 * Os specific function that need to be provided if static allocation is used
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    /* Idle task control block and stack */
    static StaticTask_t Idle_TCB;
    static StackType_t Idle_Stack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &Idle_TCB;
    *ppxIdleTaskStackBuffer = &Idle_Stack[0];
    *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}

/**
 * @fn      vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
 * @brief   This function is used to allocate memory to timer tasks when they are created
 * @return  Nothing
 *
 * Os specific function that need to be provided if static allocation is used
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    /* Timer task control block and stack */
    static StaticTask_t Timer_TCB;
    static StackType_t Timer_Stack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &Timer_TCB;
    *ppxTimerTaskStackBuffer = &Timer_Stack[0];
    *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}

/********************* pxPortInitialiseStack Reefinitions ********************/

/* Constants required to set up the initial stack. */
#define portINITIAL_XPSR            0x01000000                  /**< Initial value for xPSR */
#define portINITIAL_EXC_RETURN      0xFFFFFFFD                  /**< Initial value for exception return */
#define portSTART_ADDRESS_MASK      ((StackType_t)0xFFFFFFFEu)  /**< Initial start address (PC) value */
#define portTASK_RETURN_ADDRESS     ErrorHandler                /**< Task return address */

/**
 * @fn      InitializeFirstTaskContext(void)
 * @brief   Initialize the context for the first stack when scheduler starts
 * @note    Retrieved from FreeRTOS and slightly modified to meet requirements
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    /* Simulate the stack frame as it would be created by a context switch
     * interrupt. */

    /* Offset added to account for the way the MCU uses the stack on entry/exit
     * of interrupts, and to ensure alignment. */
    pxTopOfStack--;

    *pxTopOfStack = portINITIAL_XPSR; /* xPSR */
    pxTopOfStack--;
    *pxTopOfStack = ((StackType_t)pxCode) & portSTART_ADDRESS_MASK; /* PC */
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t)portTASK_RETURN_ADDRESS; /* LR */

    /* Save code space by skipping register initialisation. */
    pxTopOfStack -= 5;                         /* R12, R3, R2 and R1. */
    *pxTopOfStack = (StackType_t)pvParameters; /* R0 */

    /* A save method is being used that requires each task to maintain its
     * own exec return value. */
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_EXC_RETURN;

    pxTopOfStack -= 9;                             /* R11, R10, R9, R8, R7, R6, R5, R4 and R3. */
    *pxTopOfStack = INITIAL_CONTROL_IF_PRIVILEGED; /* Initialise R3 with default CONTROL in privileged mode */

    return pxTopOfStack;
}

/********************* System Calls Handling Definitions *********************/

/**
 * @fn      InitializeFirstTaskContext(void)
 * @brief   Initialize the context for the first stack when scheduler starts
 * @note    Retrieved from FreeRTOS and slightly modified to meet requirements
 */
void __attribute__((naked)) InitializeFirstTaskContext(void)
{
    __asm volatile(
        "ldr r3, pxCurrentTCBConst2             \n" /* Restore the context. */
        "ldr r2, [r3]                           \n" /* Use pxCurrentTCBConst to get the pxCurrentTCB address. */
        "ldr r1, [r2]                           \n" /* The first item in pxCurrentTCB is the task top of stack. */
        "                                       \n"
        "ldmia r1!, {r3-r11, lr}                \n" /* Pop the registers that are not automatically saved on exception entry and the critical nesting count. */
        "msr psp, r1                            \n" /* Restore the task stack pointer. */
        "msr control, r3                        \n" /* Update control register with R3 (contains control register value) */
        "isb                                    \n"
        "                                       \n"
        "mov r0, #0                             \n"
        "msr basepri, r0                        \n"
        "bx lr                                  \n"
        "                                       \n"
        ".align 4                               \n"
        "pxCurrentTCBConst2: .word pxCurrentTCB \n");
}

/**
 * @fn      CallSVCExit(void)
 * @brief   Call SVCExit using a system call (internally)
 */
static __attribute__((naked)) void CallSVCExit(void)
{
    // Call SVC exception
    __asm volatile("svc %0 \n" ::"i"(SYSCALL_EXIT) : "memory");
}

/**
 * @fn          SVCEntry(uint32_t *p_stack, uint32_t svc_no)
 * @brief       Function that executes syscalls
 * @param[in]   p_stack     Pointer to the stack before interruption
 * @param[in]   svc_no      SuperVisor Call numero
 * @note        Largely based on FreeRTOS syscall management for MPUs
 */
static void SVCEntry(uint32_t *p_stack, uint32_t svc_no)
{
    // Variable initialisation
    extern uint32_t _syscalls_start_[];
    extern uint32_t _syscalls_end_[];
    uint32_t *syscall_location = (uint32_t *)(p_stack[OFFSET_TO_PC]); // cppcheck-suppress misra-c2012-11.4; Is one of the exception of the rule because p_stack[OFFSET_TO_PC] is an address

    // Check syscall location
    if ((syscall_location >= _syscalls_start_) && (syscall_location <= _syscalls_end_))
    {
        // Get current task
        taskNo_t current_task = uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());

        // Store LR store before the syscall
        g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].syscall_tmp_lr = p_stack[OFFSET_TO_LR];

        // Raise the privilege for the duration of the system call
        __asm volatile(
            " mrs r1, control     \n" /* Obtain current control value. */
            " bic r1, #1          \n" /* Clear nPRIV bit. */
            " msr control, r1     \n" /* Write back new control value. */
            ::: "r1", "memory");

        // Set PC to to the kernel function to execute and the LR to the exit syscall request
        p_stack[OFFSET_TO_PC] = syscall_vector[svc_no];
        p_stack[OFFSET_TO_LR] = (uint32_t)CallSVCExit;
    }
    else
    {
        ErrorHandler();
    }
}

/**
 * @fn          SVCExit(uint32_t *p_stack)
 * @brief       Function that returns from syscalls
 * @param[in]   p_stack     Pointer to the stack before interruption
 * @note        Largely based on FreeRTOS syscall management for MPUs
 */
static void SVCExit(uint32_t *p_stack)
{
    // Drop the privilege before returning to the thread mode
    __asm volatile(
        " mrs r1, control     \n" /* Obtain current control value. */
        " orr r1, #1          \n" /* Set nPRIV bit. */
        " msr control, r1     \n" /* Write back new control value. */
        ::: "r1", "memory");

    // Get current task
    taskNo_t current_task = uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());

    // Restore PC and LR before the syscall was called
    p_stack[OFFSET_TO_PC] = g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].syscall_tmp_lr;
    p_stack[OFFSET_TO_LR] = g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].syscall_tmp_lr;
}

/*********************** Exception Handlers Definitions **********************/

/**
 * @fn      SVC_Handler(void)
 * @brief   SuperVisor Call exception handler
 * @note    Largely based on FreeRTOS syscall management for MPUs
 */
void __attribute__((naked)) SVC_Handler(void)
{
    __asm volatile(
        ".syntax unified                \n"
        ".extern ErrorHandler           \n"
        "                               \n"
        "tst lr, #4                     \n" // Get EXC_RETURN 3rd bit value
        "ite eq                         \n" // Check if the bit is equal to 0
        "mrseq r0, msp                  \n" // If yes then store the msp to r0
        "mrsne r0, psp                  \n" // If bo then store the psp to r0
        "ldr r2, [r0, #24]              \n" // Get pc address that execute 'svc' instruction
        "ldrb r1, [r2, #-2]             \n" // Store the svc_no (immediate value) to r1
        "                               \n"
        "cmp r1, #0                     \n" // Compare svc_no to 0 (first stack initialisation called by the OS initialisation)
        "beq %0                         \n" // If equal go to InitializeFirstTaskContext
        "cmp r1, %1                     \n" // Compare svc_no to NB_SYSCALLS
        "blt %3                         \n" // If inferior then go to the SVCEntry function
        "cmp r1, %2                     \n" // Else compare to the exit syscall numero
        "beq %4                         \n" // If equal then go to the SVCExit function
        "b ErrorHandler                 \n" // Else go to the error Handler
        :                                   /* No outputs. */
        : "i"(InitializeFirstTaskContext), "i"(NB_SYSCALLS), "i"(SYSCALL_EXIT), "i"(SVCEntry), "i"(SVCExit)
        : "r0", "r1", "r2", "memory");
}

/**
 * @fn      PendSV_Handler(void)
 * @brief   Pending SuperVisor exception handler
 * @note    Retrieved from FreeRTOS and slightly modified to meet requirements
 */
void __attribute__((naked)) PendSV_Handler(void)
{
    __asm volatile(
        "mrs r0, psp                            \n"
        "isb                                    \n"
        "                                       \n"
        "ldr r3, pxCurrentTCBConst              \n" /* Get the location of the current TCB. */
        "ldr r2, [r3]                           \n"
        "                                       \n"
        "tst r14, #0x10                         \n" /* Is the task using the FPU context?  If so, push high vfp registers. */
        "it eq                                  \n"
        "vstmdbeq r0!, {s16-s31}                \n"
        "                                       \n"
        "stmdb r0!, {r3-r11, r14}               \n" /* Save the core registers. */
        "str r0, [r2]                           \n" /* Save the new top of stack into the first member of the TCB. */
        "                                       \n"
        "stmdb sp!, {r0, r3}                    \n"
        "mov r0, %0                             \n"
        "msr basepri, r0                        \n"
        "dsb                                    \n"
        "isb                                    \n"
        "bl vTaskSwitchContext                  \n"
        "mov r0, #0                             \n"
        "msr basepri, r0                        \n"
        "ldmia sp!, {r0, r3}                    \n"
        "                                       \n"
        "ldr r1, [r3]                           \n" /* The first item in pxCurrentTCB is the task top of stack. */
        "ldr r0, [r1]                           \n"
        "                                       \n"
        "ldmia r0!, {r3-r11, r14}               \n" /* Pop the core registers. */
        "msr control, r3                        \n" /* Update control register with R3 (contains control register value) */
        "                                       \n"
        "tst r14, #0x10                         \n" /* Is the task using the FPU context?  If so, pop the high vfp registers too. */
        "it eq                                  \n"
        "vldmiaeq r0!, {s16-s31}                \n"
        "                                       \n"
        "msr psp, r0                            \n"
        "isb                                    \n"
        "                                       \n"
        "bx r14                                 \n"
        "                                       \n"
        ".align 4                               \n"
        "pxCurrentTCBConst: .word pxCurrentTCB  \n" ::"i"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
}
