/**
 * @file    art_pi_bsp.h
 * @author  Merlin Kooshmanian
 * @date    26/05/2023
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef ART_PI_BSP_H
#define ART_PI_BSP_H

/******************************* Include Files *******************************/

#include "stm32h7xx_hal.h"

/***************************** Macros Definitions ****************************/

/* BLUE LED CONSTANTS */
#define BLUE_LED_PIN                            GPIO_PIN_8
#define BLUE_LED_GPIO_PORT                      GPIOI

/* RED LED CONSTANTS */
#define RED_LED_PIN                             GPIO_PIN_15
#define RED_LED_GPIO_PORT                       GPIOC

/* USER LED CONSTANTS */
#define USER_LED_PIN                            BLUE_LED_PIN
#define USER_LED_GPIO_PORT                      BLUE_LED_GPIO_PORT

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                         GPIO_PIN_4
#define USER_BUTTON_GPIO_PORT                   GPIOH
#define USER_BUTTON_EXTI_IRQn                   EXTI4_IRQn
#define USER_BUTTON_IRQ_HANDLER                 EXTI4_IRQHandler
#define USER_BUTTON_EXTI_LINE                   EXTI_LINE_4

/* UART TMTC CONSTANTS */
#define UART_TMTC                               UART4
#define UART_TMTC_IRQ_HANDLER                   UART4_IRQHandler
#define UART_TMTC_IRQ_NO                        UART4_IRQn
#define UART_TMTC_CLK_ENABLE()                  __HAL_RCC_UART4_CLK_ENABLE()
#define UART_TMTC_CLK_DISABLE()                 __HAL_RCC_UART4_CLK_DISABLE()
#define UART_TMTC_TX_PIN                        GPIO_PIN_0
#define UART_TMTC_TX_GPIO_PORT                  GPIOA
#define UART_TMTC_RX_PIN                        GPIO_PIN_9
#define UART_TMTC_RX_GPIO_PORT                  GPIOI

/* UART TMTC DMA CONSTANTS */
#define UART_TMTC_DMA_RX                        hdma_uart4_rx
#define UART_TMTC_DMA_TX                        hdma_uart4_tx
#define UART_TMTC_DMA_CLK_ENABLE()              __HAL_RCC_DMA1_CLK_ENABLE()
#define UART_TMTC_DMA_DISABLE()                 __HAL_RCC_DMA1_CLK_DISABLE()
#define UART_TMTC_DMA_RX_IRQ_HANDLER            DMA1_Stream0_IRQHandler
#define UART_TMTC_DMA_RX_IRQ_NO                 DMA1_Stream0_IRQn
#define UART_TMTC_DMA_TX_IRQ_HANDLER            DMA1_Stream1_IRQHandler
#define UART_TMTC_DMA_TX_IRQ_NO                 DMA1_Stream1_IRQn

/* UART PRINT CONSTANTS */
#define UART_PRINT                              USART1
#define UART_PRINT_IRQ_HANDLER                  USART1_IRQHandler
#define UART_PRINT_IRQ_NO                       USART1_IRQn
#define UART_PRINT_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define UART_PRINT_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()
#define UART_PRINT_TX_PIN                       GPIO_PIN_9
#define UART_PRINT_TX_GPIO_PORT                 GPIOA
#define UART_PRINT_RX_PIN                       GPIO_PIN_10
#define UART_PRINT_RX_GPIO_PORT                 GPIOA

/* UART PL CONSTANTS */
#define UART_PL                                 USART6
#define UART_PL_IRQ_HANDLER                     USART6_IRQHandler
#define UART_PL_IRQ_NO                          USART6_IRQn
#define UART_PL_CLK_ENABLE()                    __HAL_RCC_USART6_CLK_ENABLE()
#define UART_PL_CLK_DISABLE()                   __HAL_RCC_USART6_CLK_DISABLE()
#define UART_PL_TX_PIN                          GPIO_PIN_6
#define UART_PL_TX_GPIO_PORT                    GPIOC
#define UART_PL_RX_PIN                          GPIO_PIN_7
#define UART_PL_RX_GPIO_PORT                    GPIOC

/* I2C AVIONIC CONSTANTS */
#define I2C_AVIONIC                             I2C1
#define I2C_AVIONIC_EVT_IRQ_HANDLER             I2C1_EV_IRQHandler
#define I2C_AVIONIC_EVT_IRQ_NO                  I2C1_EV_IRQn
#define I2C_AVIONIC_SCL_PIN                     GPIO_PIN_6
#define I2C_AVIONIC_SCL_GPIO_PORT               GPIOB
#define I2C_AVIONIC_SDA_PIN                     GPIO_PIN_7
#define I2C_AVIONIC_SDA_GPIO_PORT               GPIOB

/* SPI AVIONIC CONSTANTS */
#define SPI_SD_CARD                             SPI4
#define SPI_SD_CARD_IRQ_HANDLER                 SPI4_IRQHandler
#define SPI_SD_CARD_IRQ_NO                      SPI4_IRQn
#define SPI_SD_CARD_SCK_PIN                     GPIO_PIN_2
#define SPI_SD_CARD_SCK_GPIO_PORT               GPIOE
#define SPI_SD_CARD_MISO_PIN                    GPIO_PIN_5
#define SPI_SD_CARD_MISO_GPIO_PORT              GPIO_E
#define SPI_SD_CARD_MOSI_PIN                    GPIO_PIN_6
#define SPI_SD_CARD_MOSI_GPIO_PORT              GPIOE

/* SD CARD GPIO CONSTANTS */
#define SD_GPIO_PIN                             GPIO_PIN_4
#define SD_GPIO_PORT                            GPIOE

/* ONE WIRE CONSTANTS */
#define ONEWIRE_PIN                             GPIO_PIN_15
#define ONEWIRE_GPIO_PORT                       GPIOA

/* SPECIFIC PROCEDURES */
#define BSP_PROCEDURE(procedure)                procedure
#define CLOCK_SPECIFIC_INIT_PWR()               BSP_PROCEDURE(HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY); \
                                                              __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); \
                                                              while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)){})
#define CLOCK_SPECIFIC_INIT_OSC(osc_init_inst)  BSP_PROCEDURE(osc_init_inst.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE; \
                                                              osc_init_inst.HSEState = RCC_HSE_ON;  \
                                                              osc_init_inst.LSEState = RCC_LSE_BYPASS; \
                                                              osc_init_inst.PLL.PLLState = RCC_PLL_ON; \
                                                              osc_init_inst.PLL.PLLSource = RCC_PLLSOURCE_HSE; \
                                                              osc_init_inst.PLL.PLLM = 2; \
                                                              osc_init_inst.PLL.PLLN = 64; \
                                                              osc_init_inst.PLL.PLLP = 2; \
                                                              osc_init_inst.PLL.PLLQ = 4; \
                                                              osc_init_inst.PLL.PLLR = 2; \
                                                              osc_init_inst.PLL.PLLRGE = RCC_PLL1VCIRANGE_3; \
                                                              osc_init_inst.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE; \
                                                              osc_init_inst.PLL.PLLFRACN = 0;)
#define CLOCK_SPECIFIC_INIT_BUS(clk_init_inst)  BSP_PROCEDURE(clk_init_inst.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1; \
                                                              clk_init_inst.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; \
                                                              clk_init_inst.SYSCLKDivider = RCC_SYSCLK_DIV1; \
                                                              clk_init_inst.AHBCLKDivider = RCC_HCLK_DIV2; \
                                                              clk_init_inst.APB3CLKDivider = RCC_APB3_DIV2; \
                                                              clk_init_inst.APB1CLKDivider = RCC_APB1_DIV2; \
                                                              clk_init_inst.APB2CLKDivider = RCC_APB2_DIV2; \
                                                              clk_init_inst.APB4CLKDivider = RCC_APB4_DIV2; \
                                                              if (HAL_RCC_ClockConfig(&clk_init_inst, FLASH_LATENCY_2) != HAL_OK) \
                                                              { \
                                                                  return_value = GEN_HAL_ERROR; \
                                                              })
#define I2C_SPECIFIC_INIT(i2c_inst)             BSP_PROCEDURE(i2c_inst->handle_struct.Init.Timing = 0x307075B1;)
#define SPI_SPECIFIC_INIT(spi_inst)             BSP_PROCEDURE(spi_inst->handle_struct.Init.NSSPMode = SPI_NSS_PULSE_ENABLE; \
                                                              spi_inst->handle_struct.Init.NSSPolarity = SPI_NSS_POLARITY_LOW; \
                                                              spi_inst->handle_struct.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA; \
                                                              spi_inst->handle_struct.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN; \
                                                              spi_inst->handle_struct.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN; \
                                                              spi_inst->handle_struct.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE; \
                                                              spi_inst->handle_struct.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE; \
                                                              spi_inst->handle_struct.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE; \
                                                              spi_inst->handle_struct.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE; \
                                                              spi_inst->handle_struct.Init.IOSwap = SPI_IO_SWAP_DISABLE;)
#define RTC_SPECIFIC_INIT(rtc_inst)             BSP_PROCEDURE(rtc_inst.Init.HourFormat = RTC_HOURFORMAT_24; \
                                                              rtc_inst.Init.AsynchPrediv = 127u; \
                                                              rtc_inst.Init.SynchPrediv = 255u; \
                                                              rtc_inst.Init.OutPut = RTC_OUTPUT_DISABLE; \
                                                              rtc_inst.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH; \
                                                              rtc_inst.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN; \
                                                              rtc_inst.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;)
#define RTC_SET_MILLISEC(rtc_time)              BSP_PROCEDURE(rtc_time->millisecond = (MILLISECOND_SCALER*(time.SecondFraction-time.SubSeconds))/(time.SecondFraction+1);)

#endif /* ART_PI_BSP_H */
