/**
 * @file    cmsdk_gpio.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CMSDK GPIO functions
 * @date    13/06/2024
 *
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

#ifndef CMSDK_GPIO_H
#define CMSDK_GPIO_H

/******************************* Include Files *******************************/

#include "cmsdk_hal_types.h"

/***************************** Macros Definitions ****************************/

#define GPIO_INPUT_MASK             (0u << 0)
#define GPIO_OUTPUT_MASK            (1u << 0)
#define GPIO_INTERRUPT_MASK         (1u << 1)
#define GPIO_EDGE_MASK              (1u << 2)
#define GPIO_LOW_OR_FALLING_MASK    (0u << 3)
#define GPIO_HIGH_OR_RISING_MASK    (1u << 3)
#define GPIO_ALTERNATE_MASK         (1u << 4)


#define GPIO_MODE_ALTERNATIVE_FUNCTION      GPIO_ALTERNATE_MASK                                                                 /*!< Alternative function mode */
#define GPIO_MODE_OUTPUT                    GPIO_OUTPUT_MASK                                                                    /*!< Output mode */
#define GPIO_MODE_INPUT                     GPIO_INPUT_MASK                                                                     /*!< Input mode */
#define GPIO_MODE_INTERRUPT_LOW             (GPIO_INPUT_MASK | GPIO_INTERRUPT_MASK | GPIO_LOW_OR_FALLING_MASK)                  /*!< Interrupt on pin reset mode */
#define GPIO_MODE_INTERRUPT_HIGH            (GPIO_INPUT_MASK | GPIO_INTERRUPT_MASK | GPIO_HIGH_OR_RISING_MASK)                  /*!< Interrupt on pin set mode */
#define GPIO_MODE_INTERRUPT_RISING_EDGE     (GPIO_INPUT_MASK | GPIO_INTERRUPT_MASK | GPIO_EDGE_MASK | GPIO_LOW_OR_FALLING_MASK) /*!< Interrupt on pin rising edge mode */
#define GPIO_MODE_INTERRUPT_FALLING_EDGE    (GPIO_INPUT_MASK | GPIO_INTERRUPT_MASK | GPIO_EDGE_MASK | GPIO_HIGH_OR_RISING_MASK) /*!< Interrupt on pin falling edge mode */

/***************************** Types Definitions *****************************/

/**
 * @brief  GPIO port type redefinition
 */
typedef CMSDK_GPIO_TypeDef GPIO_TypeDef;

/**
 * @brief  GPIO pin mode definition
 */
typedef uint32_t GPIO_ModeTypeDef;

/**
 * @brief  GPIO Bit SET and Bit RESET enumeration
 */
typedef enum
{
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET
} GPIO_PinState;


/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

HAL_StatusTypeDef cmsdk_GpioInit(GPIO_TypeDef *gpio, uint16_t pin, GPIO_ModeTypeDef gpio_mode);
HAL_StatusTypeDef cmsdk_GpioWritePin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState pin_state);
HAL_StatusTypeDef cmsdk_GpioReadPin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState *pin_state);
HAL_StatusTypeDef cmsdk_GpioTogglePin(GPIO_TypeDef *gpio, uint16_t pin);

#endif /* CMSDK_GPIO_H */