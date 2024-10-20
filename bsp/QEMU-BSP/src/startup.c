/**
 * @file    startup.c
 * @brief   Startup file
 */

/******************************* Include Files *******************************/

#include <stdint.h>
#include "autoconf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern int main(void);
extern void SystemInit(void);

// Cortex-M system exceptions
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

// Interrupt Handlers
void Generic_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

/*************************** Variables Definitions ***************************/

extern uint32_t __stack_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
#if defined(CONFIG_LOAD_MEMORY_FLASH)
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __data_start_initialize__;
#endif
#if defined(CONFIG_LOAD_MEMORY_RAM)
extern uint32_t __tcm_bss_start__;
extern uint32_t __tcm_bss_end__;
#endif

/**
 * @brief ISR Vector Table
 */
uint32_t isr_vectors[] __attribute__((section(".isr_vector"))) = {
    (uint32_t)&__stack_end__,
    (uint32_t)&Reset_Handler,
    (uint32_t)&NMI_Handler,
    (uint32_t)&HardFault_Handler,
    (uint32_t)&MemManage_Handler,
    (uint32_t)&BusFault_Handler,
    (uint32_t)&UsageFault_Handler,
    0,
    0,
    0,
    0,
    (uint32_t)&SVC_Handler,
    (uint32_t)&DebugMon_Handler,
    0,
    (uint32_t)&PendSV_Handler,
    (uint32_t)&SysTick_Handler,
    (uint32_t)&Generic_IRQHandler, // UART0RX_Handler
    (uint32_t)&Generic_IRQHandler, // UART0TX_Handler
    (uint32_t)&Generic_IRQHandler, // UART1RX_Handler
    (uint32_t)&Generic_IRQHandler, // UART1TX_Handler
    (uint32_t)&Generic_IRQHandler, // UART2RX_Handler
    (uint32_t)&Generic_IRQHandler, // UART2TX_Handler
    (uint32_t)&Generic_IRQHandler, // GPIO0ALL_Handler
    (uint32_t)&Generic_IRQHandler, // GPIO1ALL_Handler
    (uint32_t)&Generic_IRQHandler, // TIMER0_Handler,
    (uint32_t)&Generic_IRQHandler, // TIMER1_Handler,
    (uint32_t)&Generic_IRQHandler, // DUALTIMER_Handler,
    (uint32_t)&Generic_IRQHandler, // SPI_0_1_Handler
    (uint32_t)&Generic_IRQHandler, // UART_0_1_2_OVF_Handler
    (uint32_t)&Generic_IRQHandler, // ETHERNET_Handler,
    (uint32_t)&Generic_IRQHandler, // I2C_Handler
    (uint32_t)&Generic_IRQHandler, // GPIO2_Handler
    (uint32_t)&Generic_IRQHandler, // GPIO3_Handler
    (uint32_t)&Generic_IRQHandler, // UART3RX_Handler
    (uint32_t)&Generic_IRQHandler, // UART3TX_Handler
    (uint32_t)&Generic_IRQHandler, // UART4RX_Handler
    (uint32_t)&Generic_IRQHandler, // UART4TX_Handler
    (uint32_t)&Generic_IRQHandler, // SPI_2_Handler
    (uint32_t)&Generic_IRQHandler, // SPI_3_4_Handler
    (uint32_t)&Generic_IRQHandler, // GPIO0_0_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_1_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_2_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_3_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_4_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_5_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_6_Handler,
    (uint32_t)&Generic_IRQHandler, // GPIO0_7_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

/*************************** Functions Definitions ***************************/

/**
 * @brief Reset Handler. Program goes here when a soft or hard reset has been done
 */
void Reset_Handler(void)
{
    // Then start system initialisation
    SystemInit();

    // Variable Initialisation
    uint32_t section_size = 0u;
    uint8_t *ptr_ram = 0u;
#if defined(CONFIG_LOAD_MEMORY_FLASH)
    uint8_t *ptr_flash = 0u;
#endif

#if defined(CONFIG_LOAD_MEMORY_FLASH)
    // Copy .data section from FLASH to RAM
    section_size = (uint32_t)&__data_end__ - (uint32_t)&__data_start__;
    ptr_ram = (uint8_t *)&__data_start__;
    ptr_flash = (uint8_t *)&__data_start_initialize__;
    for (uint32_t i = 0; i < section_size; i++)
    {
        *ptr_ram++ = *ptr_flash++;
    }
#endif

    // Initialise the .bss section with zero
    section_size = (uint32_t)&__bss_end__ - (uint32_t)&__bss_start__;
    ptr_ram = (uint8_t *)&__bss_start__;
    for (uint32_t i = 0; i < section_size; i++)
    {
        *ptr_ram++ = 0;
    }

#if defined(CONFIG_LOAD_MEMORY_RAM)
    // Initialise the .tcm_bss section with zero
    section_size = (uint32_t)&__tcm_bss_end__ - (uint32_t)&__tcm_bss_start__;
    ptr_ram = (uint8_t *)&__tcm_bss_start__;
    for (uint32_t i = 0; i < section_size; i++)
    {
        *ptr_ram++ = 0;
    }
#endif

    // Finally goes to main
    main();
}

/**
 * @brief Default Handler
 */
void Default_Handler(void)
{
  while (1)
  {
    // Do nothing
  }
}
