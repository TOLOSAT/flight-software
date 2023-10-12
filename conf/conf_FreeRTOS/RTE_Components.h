/*
 * Run-Time-Environment Component Configuration File
*/
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

/*
 * Define the Device Header File:
*/

#if defined(NUCLEO_F411RE)
#define CMSIS_device_header "stm32f4xx.h"
#elif defined(NUCLEO_F103RB)
#define CMSIS_device_header "stm32f1xx.h"
#elif defined(NUCLEO_H745ZI)
#define CMSIS_device_header "stm32h7xx.h"
#else 
#error "Board is not supported"
#endif

#endif /* RTE_COMPONENTS_H */
