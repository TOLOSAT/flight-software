/**
 * @file    stm32h750xb_startup.c
 * @brief   Startup File for STM32H750XB
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
void Generic_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

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
    (uint32_t)&Generic_IRQHandler, // WWDG_IRQHandler
    (uint32_t)&Generic_IRQHandler, // PVD_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TAMP_STAMP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // RTC_WKUP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FLASH_IRQHandler
    (uint32_t)&Generic_IRQHandler, // RCC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream6_IRQHandler
    (uint32_t)&Generic_IRQHandler, // ADC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FDCAN1_IT0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FDCAN2_IT0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FDCAN1_IT1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FDCAN2_IT1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI9_5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM1_BRK_TIM9_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM1_UP_TIM10_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM1_TRG_COM_TIM11_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM1_CC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C1_EV_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C1_ER_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C2_EV_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C2_ER_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPI1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPI2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // USART1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // USART2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // USART3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // EXTI15_10_IRQHandler
    (uint32_t)&Generic_IRQHandler, // RTC_Alarm_IRQHandler
    0,
    (uint32_t)&Generic_IRQHandler, // TIM8_BRK_TIM12_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM8_UP_TIM13_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM8_TRG_COM_TIM14_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM8_CC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA1_Stream7_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FMC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SDMMC1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPI3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // UART4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // UART5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM6_DAC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM7_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // ETH_IRQHandler
    (uint32_t)&Generic_IRQHandler, // ETH_WKUP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FDCAN_CAL_IRQHandler
    0,
    0,
    0,
    0,
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream6_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2_Stream7_IRQHandler
    (uint32_t)&Generic_IRQHandler, // USART6_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C3_EV_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C3_ER_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_HS_EP1_OUT_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_HS_EP1_IN_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_HS_WKUP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_HS_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DCMI_IRQHandler
    (uint32_t)&Generic_IRQHandler, // CRYP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HASH_RNG_IRQHandler
    (uint32_t)&Generic_IRQHandler, // FPU_IRQHandler
    (uint32_t)&Generic_IRQHandler, // UART7_IRQHandler
    (uint32_t)&Generic_IRQHandler, // UART8_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPI4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPI5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPI6_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SAI1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LTDC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LTDC_ER_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMA2D_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SAI2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // QUADSPI_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LPTIM1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // CEC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C4_EV_IRQHandler
    (uint32_t)&Generic_IRQHandler, // I2C4_ER_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SPDIF_RX_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_FS_EP1_OUT_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_FS_EP1_IN_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_FS_WKUP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // OTG_FS_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMAMUX1_OVR_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_Master_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_TIMA_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_TIMB_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_TIMC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_TIMD_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_TIME_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HRTIM1_FLT_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DFSDM1_FLT0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DFSDM1_FLT1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DFSDM1_FLT2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DFSDM1_FLT3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SAI3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SWPMI1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM15_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM16_IRQHandler
    (uint32_t)&Generic_IRQHandler, // TIM17_IRQHandler
    (uint32_t)&Generic_IRQHandler, // MDIOS_WKUP_IRQHandler
    (uint32_t)&Generic_IRQHandler, // MDIOS_IRQHandler
    (uint32_t)&Generic_IRQHandler, // JPEG_IRQHandler
    (uint32_t)&Generic_IRQHandler, // MDMA_IRQHandler
    0,
    (uint32_t)&Generic_IRQHandler, // SDMMC2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // HSEM1_IRQHandler
    0,
    (uint32_t)&Generic_IRQHandler, // ADC3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // DMAMUX2_OVR_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel0_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel6_IRQHandler
    (uint32_t)&Generic_IRQHandler, // BDMA_Channel7_IRQHandler
    (uint32_t)&Generic_IRQHandler, // COMP1_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LPTIM2_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LPTIM3_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LPTIM4_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LPTIM5_IRQHandler
    (uint32_t)&Generic_IRQHandler, // LPUART1_IRQHandler
    0,
    (uint32_t)&Generic_IRQHandler, // CRS_IRQHandler
    (uint32_t)&Generic_IRQHandler, // ECC_IRQHandler
    (uint32_t)&Generic_IRQHandler, // SAI4_IRQHandler
    0,
    0,
    (uint32_t)&Generic_IRQHandler, // WAKEUP_PIN_IRQHandler
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
