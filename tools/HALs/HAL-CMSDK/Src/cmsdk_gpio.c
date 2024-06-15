/**
 * @file    cmsdk_gpio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GPIO CMSDK functions
 * @date    13/06/2024
 *
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

/******************************* Include Files *******************************/

#include "cmsdk_hal.h"

/***************************** Macros Definitions ****************************/

#define GPIO_PIN_MASK               (0x0000FFFFU) /* PIN mask for assert test */

#define IS_GPIO_PIN(__PIN__)        ((((uint32_t)(__PIN__) & GPIO_PIN_MASK) != 0x00U) &&\
                                     (((uint32_t)(__PIN__) & ~GPIO_PIN_MASK) == 0x00U))

#define IS_GPIO_PIN_ACTION(ACTION)  (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))

#define IS_GPIO_MODE(MODE)          (((MODE) == GPIO_MODE_ALTERNATIVE_FUNCTION)     ||\
                                     ((MODE) == GPIO_MODE_OUTPUT)                   ||\
                                     ((MODE) == GPIO_MODE_INPUT)                    ||\
                                     ((MODE) == GPIO_MODE_INTERRUPT_LOW)            ||\
                                     ((MODE) == GPIO_MODE_INTERRUPT_HIGH)           ||\
                                     ((MODE) == GPIO_MODE_INTERRUPT_RISING_EDGE)    ||\
                                     ((MODE) == GPIO_MODE_INTERRUPT_FALLING_EDGE))

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @brief Init Gpio
 */
HAL_StatusTypeDef cmsdk_GpioInit(GPIO_TypeDef  *gpio, uint16_t pin, GPIO_ModeTypeDef gpio_mode)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (IS_GPIO_PIN(pin) && IS_GPIO_MODE(gpio_mode))
    {
        if (gpio_mode & GPIO_ALTERNATE_MASK)
        {
            // Enable alternative function mode
            gpio->ALTFUNCSET |= pin;
        }
        else
        {
            // Disable alternative function mode
            gpio->ALTFUNCCLR |= pin;

            // Check direction
            if (gpio_mode & GPIO_OUTPUT_MASK)
            {
                // Enable output
                gpio->OUTENABLESET |= pin;
            }
            else
            {
                // Disable output
                gpio->OUTENABLECLR |= pin;

                // Check Interrupt
                if (gpio_mode & GPIO_INTERRUPT_MASK)
                {
                    // Check if we want the interrupt on edge or not
                    if (gpio_mode & GPIO_EDGE_MASK)
                    {
                        gpio->INTTYPESET |= pin;
                    }
                    else
                    {
                        gpio->INTTYPECLR |= pin;
                    }

                    // Check the polarity of the interrupt
                    if (gpio_mode & GPIO_HIGH_OR_RISING_MASK)
                    {
                        // Set HIGH level or rising edge
                        gpio->INTPOLSET |= pin;
                    }
                    else
                    {
                        // Set LOW level or falling edge
                        gpio->INTPOLCLR |= pin;
                    }
                }
            }
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief Write a specific pin from a gpio
 */
HAL_StatusTypeDef cmsdk_GpioWritePin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState pin_state)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (IS_GPIO_PIN(pin) & IS_GPIO_PIN_ACTION(pin_state))
    {
        if (pin_state == GPIO_PIN_SET)
        {
            gpio->DATAOUT |= pin;
        }
        else
        {
            gpio->DATAOUT &= ~pin;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief Read a specific pin from a gpio
 */
HAL_StatusTypeDef cmsdk_GpioReadPin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState *pin_state)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (IS_GPIO_PIN(pin))
    {
        if ((gpio->DATA & pin) != 0x0000u)
        {
            *pin_state = GPIO_PIN_SET;
        }
        else
        {
            *pin_state = GPIO_PIN_RESET;
        }
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}

/**
 * @brief Toggles a specific pin from a gpio
 */
HAL_StatusTypeDef cmsdk_GpioTogglePin(GPIO_TypeDef *gpio, uint16_t pin)
{
    HAL_StatusTypeDef status = HAL_OK;

    if (IS_GPIO_PIN(pin))
    {
        gpio->DATAOUT ^= pin;
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}