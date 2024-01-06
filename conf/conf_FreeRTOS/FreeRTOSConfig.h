/* --------------------------------------------------------------------------
 * Copyright (c) 2013-2023 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * --------------------------------------------------------------------------
 *
 * $Revision:   V10.5.0
 *
 * Project:     CMSIS-FreeRTOS
 * Title:       FreeRTOS configuration definitions
 *
 * --------------------------------------------------------------------------*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#include <stdint.h>
extern uint32_t SystemCoreClock;

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// Minimal stack size [words] <0-65535>
// Stack for idle task and default task stack in words.
// Default: 128
#define configMINIMAL_STACK_SIZE                ((uint16_t)(512))

// Total heap size [bytes] <0-0xFFFFFFFF>
// Heap memory size in bytes.
// Default: 8192
#if defined(NUCLEO_H745ZI) || defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#define configTOTAL_HEAP_SIZE                 ((size_t)64*1024)
#elif defined(NUCLEO_F103RB)
#define configTOTAL_HEAP_SIZE                 ((size_t)10*1024)
#else
#error "Board is not supported"
#endif


// Kernel tick frequency [Hz] <0-0xFFFFFFFF>
// Kernel tick rate in Hz.
// Default: 1000
#define configTICK_RATE_HZ                      ((TickType_t)1000)

// Timer task stack depth [words] <0-65535>
// Stack for timer task in words.
// Default: 80
#define configTIMER_TASK_STACK_DEPTH            256

// Timer task priority <0-56>
// Timer task priority.
// Default: 40 (High)
#define configTIMER_TASK_PRIORITY               40

// Timer queue length <0-1024>
// Timer command queue length.
// Default: 5
#define configTIMER_QUEUE_LENGTH                10

// Preemption interrupt priority
// Maximum priority of interrupts that are safe to call FreeRTOS API.
// Default: 16
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    16

// Use time slicing
// Enable setting to use timeslicing.
// Default: 1
#define configUSE_TIME_SLICING                  1

// Use tickless idle
// Enable low power tickless mode to stop the periodic tick interrupt during idle periods or
// disable it to keep the tick interrupt running at all times.
// Default: 0
#define configUSE_TICKLESS_IDLE                 0

// Idle should yield
// Control Yield behaviour of the idle task.
// Default: 1
#define configIDLE_SHOULD_YIELD                 1

// Check for stack overflow
// Enable or disable stack overflow checking.
// Callback function vApplicationStackOverflowHook implementation is required when stack checking is enabled.
// Default: 0
//    <0=>Disable <1=>Method one <2=>Method two
#define configCHECK_FOR_STACK_OVERFLOW          2

// Use idle hook
// Enable callback function call on each idle task iteration.
// Callback function vApplicationIdleHook implementation is required when idle hook is enabled.
// Default: 0
#define configUSE_IDLE_HOOK                     0

// Use tick hook
// Enable callback function call during each tick interrupt.
// Callback function vApplicationTickHook implementation is required when tick hook is enabled.
// Default: 0
#define configUSE_TICK_HOOK                     0

// Use deamon task startup hook
// Enable callback function call when timer service starts.
// Callback function vApplicationDaemonTaskStartupHook implementation is required when deamon task startup hook is enabled.
// Default: 0
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

// Use malloc failed hook
// Enable callback function call when out of dynamic memory.
// Callback function vApplicationMallocFailedHook implementation is required when malloc failed hook is enabled.
// Default: 0
#define configUSE_MALLOC_FAILED_HOOK            1

// Queue registry size
// Define maximum number of queue objects registered for debug purposes.
// The queue registry is used by kernel aware debuggers to locate queue and semaphore structures and display associated text names.
// Default: 0
#define configQUEUE_REGISTRY_SIZE               0

// <h>Event Recorder configuration
// Initialize and setup Event Recorder level filtering.
// Settings have no effect when Event Recorder is not present.

// Initialize Event Recorder
// Initialize Event Recorder before FreeRTOS kernel start.
// Default: 1
#define configEVR_INITIALIZE                    1

// Setup recording level filter
// Enable configuration of FreeRTOS events recording level
// Default: 1
#define configEVR_SETUP_LEVEL                   1

// Tasks functions
// Define event recording level bitmask for events generated from Tasks functions.
// Default: 0x05
//    <0x00=>Off <0x01=>Errors <0x05=>Errors + Operation <0x0F=>All
#define configEVR_LEVEL_TASKS                   0x05

// Queue functions
// Define event recording level bitmask for events generated from Queue functions.
// Default: 0x05
//    <0x00=>Off <0x01=>Errors <0x05=>Errors + Operation <0x0F=>All
#define configEVR_LEVEL_QUEUE                   0x05

// Timer functions
// Define event recording level bitmask for events generated from Timer functions.
// Default: 0x05
//    <0x00=>Off <0x01=>Errors <0x05=>Errors + Operation <0x0F=>All
#define configEVR_LEVEL_TIMERS                  0x05

// Event Groups functions
// Define event recording level bitmask for events generated from Event Groups functions.
// Default: 0x05
//    <0x00=>Off <0x01=>Errors <0x05=>Errors + Operation <0x0F=>All
#define configEVR_LEVEL_EVENTGROUPS             0x05

// Heap functions
// Define event recording level bitmask for events generated from Heap functions.
// Default: 0x05
//    <0x00=>Off <0x01=>Errors <0x05=>Errors + Operation <0x0F=>All
#define configEVR_LEVEL_HEAP                    0x05

// Stream Buffer functions
// Define event recording level bitmask for events generated from Stream Buffer functions.
// Default: 0x05
//    <0x00=>Off <0x01=>Errors <0x05=>Errors + Operation <0x0F=>All
#define configEVR_LEVEL_STREAMBUFFER            0x05

// Port Specific Features
// Enable and configure port specific features.
// Check FreeRTOS documentation for definitions that apply for the used port.

// Use Floating Point Unit
// Using Floating Point Unit (FPU) affects context handling.
// Enable FPU when application uses floating point operations.
// Default: 1
#if defined(FPU_AVAILABLE)
#define configENABLE_FPU                      1
#else
#define configENABLE_FPU                      0
#endif

// Use M-Profile Vector Extension
// Using M-Profile Vector Extension (MVE) affects context handling.
// Enable MVE when application uses signal processing and ML algorithms.
// Default: 0
#define configENABLE_MVE                      0

// Use Memory Protection Unit
// Using Memory Protection Unit (MPU) requires detailed memory map definition.
// This setting is only releavant for MPU enabled ports.
// Default: 0
#if defined(MPU_AVAILABLE)
#define configENABLE_MPU                      1
#else
#define configENABLE_MPU                      0
#endif

// Use TrustZone Secure Side Only
// This settings prevents FreeRTOS contex switch to Non-Secure side.
// Enable this setting when FreeRTOS runs on the Secure side only.
#define configRUN_FREERTOS_SECURE_ONLY        0

// Use TrustZone Security Extension
// Using TrustZone affects context handling.
// Enable TrustZone when FreeRTOS runs on the Non-Secure side and calls functions from the Secure side.
// Default: 1
#define configENABLE_TRUSTZONE                1

// Minimal secure stack size [words] <0-65535>
// Stack for idle task Secure side context in words.
// This setting is only relevant when TrustZone extension is enabled.
// Default: 128
#define configMINIMAL_SECURE_STACK_SIZE       ((uint32_t)128)

//------------- <<< end of configuration section >>> ---------------------------

/* Defines needed by FreeRTOS */
#define configCPU_CLOCK_HZ                      (SystemCoreClock)
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configUSE_PREEMPTION                    1
#define configUSE_TIMERS                        1
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_TRACE_FACILITY                1
#define configUSE_16_BIT_TICKS                  0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configMAX_PRIORITIES                    56
#define configKERNEL_INTERRUPT_PRIORITY         255

/* Defines that include FreeRTOS functions which implement CMSIS RTOS2 API. Do not change! */
#define INCLUDE_xEventGroupSetBitsFromISR       1
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_xQueueGetMutexHolder            1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xTaskAbortDelay                 1
#define INCLUDE_xTimerPendFunctionCall          1

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS						__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS						4
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
 * function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY	15

/* The highest interrupt priority that can be used by any interrupt service
 * routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT
 * CALL INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A
 * HIGHER PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

/* Map the FreeRTOS port interrupt handlers to their CMSIS standard names. */
#define xPortPendSVHandler                      PendSV_Handler
#define vPortSVCHandler                         SVC_Handler

/* The following flag must be enabled only when using newlib */
#define configUSE_NEWLIB_REENTRANT              1

/* FreeRTOS MPU specific definitions. */
#if defined(MPU_AVAILABLE)
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS  0
#define configTOTAL_MPU_REGIONS                                 16
#define configTEX_S_C_B_FLASH                                   0x07UL /* Default value. */
#define configTEX_S_C_B_SRAM                                    0x03UL /* Default value. Do not mark the SRAM as shareable because caching is being used. */
#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY             1
#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS              0
#define configENABLE_ERRATA_837070_WORKAROUND                   0
#define configUSE_MPU_WRAPPERS_V1                               0
#define configENABLE_ACCESS_CONTROL_LIST                        1
#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                 150
#define configSYSTEM_CALL_STACK_SIZE                            128
#endif

/* Records Stack Usage */
#define configRECORD_STACK_HIGH_ADDRESS         1

/* Records Runtime Usage */
extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);
#define configGENERATE_RUN_TIME_STATS           1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS  configureTimerForRunTimeStats
#define portGET_RUN_TIME_COUNTER_VALUE          getRunTimeCounterValue

/* Normal assert() semantics without relying on the provision of an assert.h header file. */
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}

#endif /* FREERTOS_CONFIG_H */