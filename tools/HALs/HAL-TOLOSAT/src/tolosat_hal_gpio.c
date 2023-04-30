/**
 * @file tolosat_hal_gpio.c
 * @author Merlin Kooshmanian
 * @brief Source file for TOLOSAT HAL GPIO functions
 * @date 29/04/2023
 *
 * Last Update : 29/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      GpioOpen(gpioInst_t *gpio, gpioPort_t *port, gpioPin_t pin)
 * @brief   Function that initialise a GPIO
 * @param   gpio_inst Instance that contains GPIOs parameters
 * @param   port Gpio port (GPIOA, GPIOB, GPIOC, GPIOH)
 * @param   pin Pin (GPIO_PIN_0 to GPIO_PIN_15)
 * @retval  FCT_SUCCESSFUL if creation succeed
 * @retval  FCT_INVALID_PARAM if GPIO is not GPIOA,GPIOB,GPIOC,GPIOD/H pin = 0, or one pointer is null
 * 
 * Attention : GPIO_PIN_0 != 0, GPIO_PIN_0=0x0001 (cf tolosat_hal_gpio.h)
 */
halStatus_t GpioOpen(gpioInst_t *gpio_inst, gpioPort_t *port, gpioPin_t pin)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Function Core
    if(gpio_inst != NULL && port != NULL && pin != 0)
    {
#if defined(STM32F411xE)
        if((uint32_t) port == GPIOA_BASE)
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOB_BASE)
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOC_BASE)
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOH_BASE)
        {
            __HAL_RCC_GPIOH_CLK_ENABLE();
        }
#endif
#if defined(STM32F103xB)
        if((uint32_t) port == GPIOA_BASE)
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOB_BASE)
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOC_BASE)
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOD_BASE)
        {
            __HAL_RCC_GPIOD_CLK_ENABLE();
        }
        else if((uint32_t) port == GPIOE_BASE)
        {
            __HAL_RCC_GPIOE_CLK_ENABLE();
        }
#endif
        else 
        {
            return_value = FCT_INVALID_PARAM;
        }

        if(return_value == FCT_SUCCESSFUL)
        {
            gpio_inst->port = port;
            gpio_inst->pin = pin;
            GPIO_InitStruct.Pin = pin;
            GPIO_InitStruct.Mode = gpio_inst->mode;
            GPIO_InitStruct.Pull = gpio_inst->pull;
            GPIO_InitStruct.Speed = gpio_inst->speed;
            HAL_GPIO_Init(port ,&GPIO_InitStruct);
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

/**
 * @fn      GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
 * @brief   Function that writes into a GPIO pin
 * @param   gpio_inst Instance that contains GPIOs parameters
 * @param   value Value we want to write on the pin
 * @retval  FCT_SUCCESSFUL if write succeed
 * @retval  FCT_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
halStatus_t GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(gpio_inst != NULL && (gpio_inst->mode == GPIO_MODE_OUTPUT_PP || gpio_inst->mode == GPIO_MODE_OUTPUT_OD))
    {
        HAL_GPIO_WritePin(gpio_inst->port,gpio_inst->pin, value);
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }
    

    return(return_value);    
}

/**
 * @fn      GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
 * @brief   Function that reads into a GPIO pin
 * @param   gpio_inst Instance that contains GPIOs parameters
 * @param   value Value we want to write on the pin
 * @retval  FCT_SUCCESSFUL if write succeed
 * @retval  FCT_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 * 
 * Only works in INPUT mode without interrupt
 */
halStatus_t GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(gpio_inst != NULL && gpio_inst->mode == GPIO_MODE_INPUT)
    {
        *value = HAL_GPIO_ReadPin(gpio_inst->port,gpio_inst->pin);
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);    
}

/**
 * @fn      GpioToggle(gpioInst_t *gpio_inst)
 * @brief   Function that toggles a GPIO pin
 * @param   gpio_inst Instance that contains GPIOs parameters
 * @retval  FCT_SUCCESSFUL if toggle succeed
 * @retval  FCT_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
halStatus_t GpioToggle(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(gpio_inst != NULL && (gpio_inst->mode == GPIO_MODE_OUTPUT_PP || gpio_inst->mode == GPIO_MODE_OUTPUT_OD))
    {
        HAL_GPIO_TogglePin(gpio_inst->port,gpio_inst->pin);
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);    
}

/**
 * @fn      GpioIoctl(gpioInst_t *gpio_inst)
 * @brief   Function that allows to change parameters such as mode, speed and pull
 * @param   gpio_inst Instance that contains GPIOs parameters
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 * 
 * This feature is not supported yet so it does nothing
 */
halStatus_t GpioIoctl(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(gpio_inst != NULL)
    {
        /* TO DO */
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);    
}

/**
 * @fn      GpioClose(gpioInst_t *gpio_inst)
 * @brief   Function that desinit the gpio pin and puts defaults parameters
 * @param   gpio_inst Instance that contains GPIOs parameters
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 * 
 * This function erase gpio_inst
 */
halStatus_t GpioClose(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    gpioInst_t null_inst = {
        .port = NULL,
        .pin = 0,
        .mode = 0,
        .pull = 0,
        .speed = 0,
    };

    // Function Core
    if(gpio_inst != NULL)
    {
        HAL_GPIO_DeInit(gpio_inst->port, gpio_inst->pin);
        *gpio_inst = null_inst;
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);    
}