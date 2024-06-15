/**
 * @file    mps2_an500_startup.c
 * @brief   Startup File for MPS2-AN500
 */

/******************************* Include Files *******************************/

#include <stdint.h>

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
void UART0RX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART0TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART1RX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART1TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART2RX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART2TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0ALL_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO1ALL_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TIMER1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DUALTIMER_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI_0_1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART_0_1_2_OVF_Handler(void) __attribute__((weak, alias("Default_Handler")));
void ETHERNET_Handler(void) __attribute__((weak, alias("Default_Handler")));
void I2S_Handler(void) __attribute__((weak, alias("Default_Handler")));
void TOUCHSCREEN_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART3RX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART3TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4RX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UART4TX_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SPI_3_4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_1_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_2_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_4_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_5_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_6_Handler(void) __attribute__((weak, alias("Default_Handler")));
void GPIO0_7_Handler(void) __attribute__((weak, alias("Default_Handler")));

/*************************** Variables Definitions ***************************/

extern uint32_t __stack_end__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __data_start_initialize__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

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
    (uint32_t)&UART0RX_Handler,
    (uint32_t)&UART0TX_Handler,
    (uint32_t)&UART1RX_Handler,
    (uint32_t)&UART1TX_Handler,
    (uint32_t)&UART2RX_Handler,
    (uint32_t)&UART2TX_Handler,
    (uint32_t)&GPIO0ALL_Handler,
    (uint32_t)&GPIO1ALL_Handler,
    (uint32_t)&TIMER0_Handler,
    (uint32_t)&TIMER1_Handler,
    (uint32_t)&DUALTIMER_Handler,
    (uint32_t)&SPI_0_1_Handler,
    (uint32_t)&UART_0_1_2_OVF_Handler,
    (uint32_t)&ETHERNET_Handler,
    (uint32_t)&TOUCHSCREEN_Handler,
    (uint32_t)&GPIO2_Handler,
    (uint32_t)&GPIO3_Handler,
    (uint32_t)&UART3RX_Handler,
    (uint32_t)&UART3TX_Handler,
    (uint32_t)&UART4RX_Handler,
    (uint32_t)&UART4TX_Handler,
    (uint32_t)&SPI_2_Handler,
    (uint32_t)&SPI_3_4_Handler,
    (uint32_t)&GPIO0_0_Handler,
    (uint32_t)&GPIO0_1_Handler,
    (uint32_t)&GPIO0_2_Handler,
    (uint32_t)&GPIO0_3_Handler,
    (uint32_t)&GPIO0_4_Handler,
    (uint32_t)&GPIO0_5_Handler,
    (uint32_t)&GPIO0_6_Handler,
    (uint32_t)&GPIO0_7_Handler,
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
#if defined(LOAD_FLASH)
  uint8_t *ptr_flash = 0u;
#endif

#if defined(LOAD_FLASH)
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
