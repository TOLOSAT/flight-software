/**
 * @file    nucleo_f103rb_bsp.h
 * @author  Merlin Kooshmanian
 * @date    22/04/2023
 * 
 * @copyright Copyright (c) 2023
 */
#ifndef NUCLEO_F103RB_BSP_H
#define NUCLEO_F103RB_BSP_H

/******************************* Include Files *******************************/

#include "stm32f1xx_hal.h"

/***************************** Macros Definitions ****************************/

/* LED 2 CONSTANTS */
#define LED2_PIN                                        GPIO_PIN_5
#define LED2_GPIO_PORT                                  GPIOA

/* USER LED CONSTANTS */
#define USER_LED_PIN                                    LED2_PIN
#define USER_LED_GPIO_PORT                              LED2_GPIO_PORT

/* USER BUTTON CONSTANTS */
#define USER_BUTTON_PIN                                 GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                           GPIOC
#define USER_BUTTON_EXTI_IRQn                           EXTI15_10_IRQn
#define USER_BUTTON_IRQ_HANDLER                         EXTI15_10_IRQHandler
#define USER_BUTTON_IRQ_CALLBACK                        HAL_GPIO_EXTI_Callback
#define USER_BUTTON_EXTI_LINE                           EXTI_LINE_13

/* UART TMTC CONSTANTS */
#define TAPAS_UART_TMTC                                 USART1
#define TAPAS_UART_TMTC_IRQ_HANDLER                     USART1_IRQHandler
#define TAPAS_UART_TMTC_IRQ_NO                          USART1_IRQn
#define TAPAS_UART_TMTC_CLK_ENABLE()                    __HAL_RCC_USART1_CLK_ENABLE()
#define TAPAS_UART_TMTC_CLK_DISABLE()                   __HAL_RCC_USART1_CLK_DISABLE()
#define TAPAS_UART_TMTC_TX_PIN                          GPIO_PIN_6
#define TAPAS_UART_TMTC_TX_GPIO_PORT                    GPIOB
#define TAPAS_UART_TMTC_RX_PIN                          GPIO_PIN_7
#define TAPAS_UART_TMTC_RX_GPIO_PORT                    GPIOB

/* UART TMTC DMA CONSTANTS */
#define TAPAS_UART_TMTC_DMA_RX                          hdma_usart1_rx
#define TAPAS_UART_TMTC_DMA_TX                          hdma_usart1_tx
#define TAPAS_UART_TMTC_DMA_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
#define TAPAS_UART_TMTC_DMA_DISABLE()                   __HAL_RCC_DMA1_CLK_DISABLE()
#define TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER              DMA1_Channel5_IRQHandler
#define TAPAS_UART_TMTC_DMA_RX_IRQ_NO                   DMA1_Channel5_IRQn
#define TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER              DMA1_Channel4_IRQHandler
#define TAPAS_UART_TMTC_DMA_TX_IRQ_NO                   DMA1_Channel4_IRQn

/* UART PRINT CONSTANTS */
#define TAPAS_UART_PRINT                                USART2
#define TAPAS_UART_PRINT_IRQ_HANDLER                    USART2_IRQHandler
#define TAPAS_UART_PRINT_IRQ_NO                         USART2_IRQn
#define TAPAS_UART_PRINT_CLK_ENABLE()                   __HAL_RCC_USART2_CLK_ENABLE()
#define TAPAS_UART_PRINT_CLK_DISABLE()                  __HAL_RCC_USART2_CLK_DISABLE()
#define TAPAS_UART_PRINT_TX_PIN                         GPIO_PIN_2
#define TAPAS_UART_PRINT_TX_GPIO_PORT                   GPIOA
#define TAPAS_UART_PRINT_RX_PIN                         GPIO_PIN_3
#define TAPAS_UART_PRINT_RX_GPIO_PORT                   GPIOA

/* UART PL CONSTANTS */
#define TAPAS_UART_PL                                   USART3
#define TAPAS_UART_PL_IRQ_HANDLER                       USART3_IRQHandler
#define TAPAS_UART_PL_IRQ_NO                            USART3_IRQn
#define TAPAS_UART_PL_CLK_ENABLE()                      __HAL_RCC_USART3_CLK_ENABLE()
#define TAPAS_UART_PL_CLK_DISABLE()                     __HAL_RCC_USART3_CLK_DISABLE()
#define TAPAS_UART_PL_TX_PIN                            GPIO_PIN_10
#define TAPAS_UART_PL_TX_GPIO_PORT                      GPIOB
#define TAPAS_UART_PL_RX_PIN                            GPIO_PIN_11
#define TAPAS_UART_PL_RX_GPIO_PORT                      GPIOB

/* I2C AVIONIC CONSTANTS */
#define TAPAS_I2C_AVIONIC                               I2C1
#define TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER               I2C1_EV_IRQHandler
#define TAPAS_I2C_AVIONIC_EVT_IRQ_NO                    I2C1_EV_IRQn
#define TAPAS_I2C_AVIONIC_SCL_PIN                       GPIO_PIN_8
#define TAPAS_I2C_AVIONIC_SCL_GPIO_PORT                 GPIOB
#define TAPAS_I2C_AVIONIC_SDA_PIN                       GPIO_PIN_9
#define TAPAS_I2C_AVIONIC_SDA_GPIO_PORT                 GPIOB

/* SPI SD CARD CONSTANTS */
#define TAPAS_SPI_SDCARD                                SPI2
#define TAPAS_SPI_SDCARD_IRQ_HANDLER                    SPI2_IRQHandler
#define TAPAS_SPI_SDCARD_IRQ_NO                         SPI2_IRQn
#define TAPAS_SPI_SDCARD_SCK_PIN                        GPIO_PIN_13
#define TAPAS_SPI_SDCARD_SCK_GPIO_PORT                  GPIOB
#define TAPAS_SPI_SDCARD_MISO_PIN                       GPIO_PIN_14
#define TAPAS_SPI_SDCARD_MISO_GPIO_PORT                 GPIOB
#define TAPAS_SPI_SDCARD_MOSI_PIN                       GPIO_PIN_15
#define TAPAS_SPI_SDCARD_MOSI_GPIO_PORT                 GPIOB
#define TAPAS_SPI_SDCARD_CS_PIN                         GPIO_PIN_12
#define TAPAS_SPI_SDCARD_CS_GPIO_PORT                   GPIOB

/* SPECIFIC PROCEDURES */
#define BSP_PROCEDURE(procedure)                        procedure
#define TAPAS_CLOCK_SPECIFIC_INIT_PWR()                 BSP_PROCEDURE()
#define TAPAS_CLOCK_SPECIFIC_INIT_OSC(osc_init_inst)    BSP_PROCEDURE(osc_init_inst.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE; \
                                                                      osc_init_inst.HSEState = RCC_HSE_BYPASS; \
                                                                      osc_init_inst.HSEPredivValue = RCC_HSE_PREDIV_DIV1; \
                                                                      osc_init_inst.LSEState = RCC_LSE_ON; \
                                                                      osc_init_inst.HSIState = RCC_HSI_ON; \
                                                                      osc_init_inst.PLL.PLLState = RCC_PLL_ON; \
                                                                      osc_init_inst.PLL.PLLSource = RCC_PLLSOURCE_HSE; \
                                                                      osc_init_inst.PLL.PLLMUL = RCC_PLL_MUL9;)
#define TAPAS_CLOCK_SPECIFIC_INIT_BUS(clk_init_inst)    BSP_PROCEDURE(clk_init_inst.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2; \
                                                                      clk_init_inst.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; \
                                                                      clk_init_inst.AHBCLKDivider = RCC_SYSCLK_DIV1; \
                                                                      clk_init_inst.APB1CLKDivider = RCC_HCLK_DIV2; \
                                                                      clk_init_inst.APB2CLKDivider = RCC_HCLK_DIV1; \
                                                                      if (HAL_RCC_ClockConfig(&clk_init_inst, FLASH_LATENCY_2) == HAL_OK) \
                                                                      { \
                                                                          RCC_PeriphCLKInitTypeDef PeriphClkInit = {0}; \
                                                                          PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC; \
                                                                          PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; \
                                                                          if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) \
                                                                          { \
                                                                              return_value = THAL_ERROR; \
                                                                          } \
                                                                      } \
                                                                      else \
                                                                      { \
                                                                          return_value = THAL_ERROR; \
                                                                      })
#define TAPAS_IIC_SPECIFIC_INIT(iic_inst)               BSP_PROCEDURE(iic_inst->handle_struct.Init.ClockSpeed = 100000; \
                                                                      iic_inst->handle_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;)
#define TAPAS_SPI_SPECIFIC_INIT(spi_inst)               BSP_PROCEDURE(spi_inst->handle_struct.Init.Mode = SPI_MODE_MASTER;)
#define TAPAS_RTC_SPECIFIC_INIT(rtc_inst)               BSP_PROCEDURE(rtc_inst.Init.AsynchPrediv = RTC_AUTO_1_SECOND; \
                                                                      rtc_inst.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;)
#define TAPAS_RTC_SET_MILLISEC(rtc_time)                BSP_PROCEDURE(rtc_time->millisecond = 0u)

#endif /* NUCLEO_F103RB_BSP_H */
