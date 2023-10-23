/*
 * Run-Time-Environment Component Configuration File
*/
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

/*
 * Define the Device Header File:
*/

#if defined(NUCLEO_H745ZI)
#define CMSIS_device_header "stm32h7xx.h"
#elif defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#define CMSIS_device_header "stm32f4xx.h"
#elif defined(NUCLEO_F103RB)
#define CMSIS_device_header "stm32f1xx.h"
#else 
#error "Board is not supported"
#endif

/* ARM.FreeRTOS::RTOS.Config.CMSIS RTOS2 */
#define RTE_RTOS_FreeRTOS_CONFIG_RTOS2  /* RTOS FreeRTOS Config for CMSIS RTOS2 API */

/* ARM.FreeRTOS::RTOS.Core.Cortex-M */
#define RTE_RTOS_FreeRTOS_CORE          /* RTOS FreeRTOS Core */

/* ARM.FreeRTOS::RTOS.Event Groups */
#define RTE_RTOS_FreeRTOS_EVENTGROUPS   /* RTOS FreeRTOS Event Groups */

/* ARM.FreeRTOS::RTOS.Heap.Heap_1 */
#define RTE_RTOS_FreeRTOS_HEAP_1        /* RTOS FreeRTOS Heap 1 */

/* ARM.FreeRTOS::RTOS.Timers */
#define RTE_RTOS_FreeRTOS_TIMERS        /* RTOS FreeRTOS Timers */

/* ARM::CMSIS.RTOS2.FreeRTOS.Cortex-M */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
#define RTE_CMSIS_RTOS2_FreeRTOS        /* CMSIS-RTOS2 FreeRTOS */

/* Board specific total heap size [bytes]*/
#if defined(NUCLEO_H745ZI) || defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#define specificTOTAL_HEAP_SIZE                 ((size_t)65536)
#elif defined(NUCLEO_F103RB)
#define specificTOTAL_HEAP_SIZE                 ((size_t)10240)
#else
#error "Board is not supported"
#endif

/* Board specific use of floating point unit */
#if defined(NUCLEO_H745ZI) || defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#define specificENABLE_FPU                      1
#elif defined(NUCLEO_F103RB)
#define specificENABLE_FPU                      0
#else
#error "Board is not supported"
#endif

/* Board specific use of memory protection unit */
#if defined(NUCLEO_H745ZI) 
#define specificENABLE_MPU                      0
#elif defined(NUCLEO_F103RB) || defined(NUCLEO_F411RE) || defined(DISCOVERY_F407VG)
#define specificENABLE_MPU                      0
#else
#error "Board is not supported"
#endif

#endif /* RTE_COMPONENTS_H */
