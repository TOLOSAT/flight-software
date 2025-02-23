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

#define GPIO_PIN_MASK              (0x0000FFFFU) /**< PIN mask for assert test */

/**
 * @def     IS_GPIO_PIN
 * @brief   Check if the pin(s) choosen is correct
 */
#define IS_GPIO_PIN(__PIN__)       ((((uint32_t)(__PIN__) & GPIO_PIN_MASK) != 0x00U) && (((uint32_t)(__PIN__) & ~GPIO_PIN_MASK) == 0x00U))

/**
 * @def     IS_GPIO_PIN_STATE
 * @brief   Check if the gpio pin(s) state is correct (PIN RESET or PIN SET)
 */
#define IS_GPIO_PIN_STATE(STATE) (((STATE) == GPIO_PIN_RESET) || ((STATE) == GPIO_PIN_SET))

/**
 * @def     IS_GPIO_MODE
 * @brief   Check if the gpio mode is correct
 */
#define IS_GPIO_MODE(MODE)                                                                                                         \
    (((MODE) == GPIO_MODE_ALTERNATIVE_FUNCTION) || ((MODE) == GPIO_MODE_OUTPUT) || ((MODE) == GPIO_MODE_INPUT)                     \
     || ((MODE) == GPIO_MODE_INTERRUPT_LOW) || ((MODE) == GPIO_MODE_INTERRUPT_HIGH) || ((MODE) == GPIO_MODE_INTERRUPT_RISING_EDGE) \
     || ((MODE) == GPIO_MODE_INTERRUPT_FALLING_EDGE))

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          cmsdk_GpioInit(GPIO_TypeDef *gpio, uint16_t pin, GPIO_ModeTypeDef gpio_mode)
 * @brief       Init Gpio
 * @param[in]   gpio        GPIO handle struct
 * @param[in]   pin         GPIO pin
 * @param[in]   gpio_mode   GPIO pin mode
 * @retval      #HAL_ERROR gpio is a null pointer
 * @retval      #HAL_ERROR if the pin selected is not correct
 * @retval      #HAL_ERROR if the mode selected is not correct
 * @retval      #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_GpioInit(GPIO_TypeDef *gpio, uint16_t pin, GPIO_ModeTypeDef gpio_mode)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((gpio != NULL) && (IS_GPIO_PIN(pin)) && (IS_GPIO_MODE(gpio_mode)))
    {
        if ((gpio_mode & GPIO_ALTERNATE_MASK) == GPIO_ALTERNATE_MASK)
        {
            // Enable alternative function mode
            gpio->ALTFUNCSET |= pin;
        }
        else
        {
            // Disable alternative function mode
            gpio->ALTFUNCCLR |= pin;

            // Check direction
            if ((gpio_mode & GPIO_OUTPUT_MASK) == GPIO_OUTPUT_MASK)
            {
                // Enable output
                gpio->OUTENSET |= pin;
            }
            else
            {
                // Disable output
                gpio->OUTENCLR |= pin;

                // Check Interrupt
                if ((gpio_mode & GPIO_INTERRUPT_MASK) == GPIO_INTERRUPT_MASK)
                {
                    // Check if we want the interrupt on edge or not
                    if ((gpio_mode & GPIO_EDGE_MASK) == GPIO_EDGE_MASK)
                    {
                        gpio->INTTYPESET |= pin;
                    }
                    else
                    {
                        gpio->INTTYPECLR |= pin;
                    }

                    // Check the polarity of the interrupt
                    if ((gpio_mode & GPIO_HIGH_OR_RISING_MASK) == GPIO_HIGH_OR_RISING_MASK)
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
 * @fn      cmsdk_GpioWritePin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState pin_state)
 * @brief   Writes a specific pin from a gpio
 * @param   gpio        GPIO handle struct
 * @param   pin         Pin
 * @param   pin_state   State to set on the pin
 * @retval  #HAL_ERROR gpio is a null pointer
 * @retval  #HAL_ERROR if pin is not a correct value
 * @retval  #HAL_ERROR if pin state is not a correct input
 * @retval  #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_GpioWritePin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState pin_state)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((gpio != NULL) && (IS_GPIO_PIN(pin)) && (IS_GPIO_PIN_STATE(pin_state)))
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
 * @fn      cmsdk_GpioReadPin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState *pin_state)
 * @brief   Reads a specific pin from a gpio
 * @param   gpio        GPIO handle struct
 * @param   pin         Pin
 * @param   pin_state   State of the pin
 * @retval  #HAL_ERROR gpio is a null pointer
 * @retval  #HAL_ERROR pin_state is a null pointer
 * @retval  #HAL_ERROR if pin is not a correct value
 * @retval  #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_GpioReadPin(GPIO_TypeDef *gpio, uint16_t pin, GPIO_PinState *pin_state)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((gpio != NULL) && (pin_state != NULL) && IS_GPIO_PIN(pin))
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
 * @fn      cmsdk_GpioTogglePin(GPIO_TypeDef *gpio, uint16_t pin)
 * @brief   Toggles a specific pin from a gpio
 * @param   gpio GPIO handle struct
 * @param   pin  Pin to toggle
 * @retval  #HAL_ERROR gpio is a null pointer
 * @retval  #HAL_ERROR if pin is not a correct value
 * @retval  #HAL_OK else
 */
HAL_StatusTypeDef cmsdk_GpioTogglePin(GPIO_TypeDef *gpio, uint16_t pin)
{
    HAL_StatusTypeDef status = HAL_OK;

    if ((gpio != NULL) && IS_GPIO_PIN(pin))
    {
        gpio->DATAOUT ^= pin;
    }
    else
    {
        status = HAL_ERROR;
    }

    return status;
}