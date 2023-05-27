/*
 * Auto generated Run-Time-Environment Component Configuration File
 *      *** Do not modify ! ***
 *
 * Project: Blinky
 * RTE configuration: Blinky.rteconfig
*/
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

/*
 * Define the Device Header File:
*/

#if defined(STM32F411xE)
#define CMSIS_device_header "stm32f4xx.h"
#elif defined(STM32F103xB)
#define CMSIS_device_header "stm32f1xx.h"
#elif defined(STM32H745xx)
#define CMSIS_device_header "stm32h7xx.h"
#else 
#error "Board is not supported"
#endif

#endif /* RTE_COMPONENTS_H */
