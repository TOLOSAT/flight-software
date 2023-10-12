/**
 * @file    tolosat_hal_gpio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL GPIO functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t GpioEnableInterrupt(gpioInst_t *gpio_inst);
static halStatus_t GpioDisableInterrupt(gpioInst_t *gpio_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              GpioOpen(gpioInst_t *gpio_inst, gpioPort_t *port, gpioPin_t pin)
 * @brief           Function that initialise a GPIO
 * @param[in,out]   gpio_inst Instance that contains GPIOs parameters
 * @param[in]       port Gpio port (GPIOA, GPIOB, GPIOC, GPIOH)
 * @param[in]       pin Pin (GPIO_PIN_0 to GPIO_PIN_15)
 * @retval          #THAL_SUCCESSFUL if creation succeed
 * @retval          #THAL_INVALID_PARAM if GPIO port is not available for this board, pin = 0 or one pointer is null
 *
 * Attention : GPIO_PIN_0 != 0, GPIO_PIN_0=0x0001 (cf tolosat_hal_gpio.h)
 */
halStatus_t GpioOpen(gpioInst_t *gpio_inst, gpioPort_t *port, gpioPin_t pin)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Function Core
    if ((gpio_inst != NULL) && (port != NULL) && (pin != 0u))
    {
        switch ((uint32_t)port)
        {
        case GPIOA_BASE:
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case GPIOB_BASE:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case GPIOC_BASE:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
#if defined(GPIOD_BASE)
        case GPIOD_BASE:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
#endif
#if defined(GPIOE_BASE)
        case GPIOE_BASE:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
#endif
#if defined(GPIOF_BASE)
        case GPIOF_BASE:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
#endif
#if defined(GPIOG_BASE)
        case GPIOG_BASE:
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
#endif
#if defined(GPIOH_BASE)
        case GPIOH_BASE:
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
#endif
#if defined(GPIOI_BASE)
        case GPIOI_BASE:
            __HAL_RCC_GPIOI_CLK_ENABLE();
            break;
#endif
#if defined(GPIOJ_BASE)
        case GPIOJ_BASE:
            __HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
#endif
#if defined(GPIOK_BASE)
        case GPIOK_BASE:
            __HAL_RCC_GPIOK_CLK_ENABLE();
            break;
#endif
        default:
            return_value = THAL_INVALID_PARAM;
            break;
        }

        if (return_value == THAL_SUCCESSFUL)
        {
            gpio_inst->port = port;
            gpio_inst->pin = pin;
            GPIO_InitStruct.Pin = pin;
            GPIO_InitStruct.Mode = gpio_inst->mode;
            GPIO_InitStruct.Pull = gpio_inst->pull;
            GPIO_InitStruct.Speed = gpio_inst->speed;
            HAL_GPIO_Init(gpio_inst->port, &GPIO_InitStruct);
            return_value = GpioEnableInterrupt(gpio_inst);
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
 * @brief       Function that writes into a GPIO pin
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @param[in]   value Value we want to write on the pin
 * @retval      #THAL_SUCCESSFUL if write succeed
 * @retval      #THAL_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
halStatus_t GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst != NULL) && ((gpio_inst->mode == GPIO_MODE_OUTPUT_PP) || (gpio_inst->mode == GPIO_MODE_OUTPUT_OD)))
    {
        HAL_GPIO_WritePin(gpio_inst->port, gpio_inst->pin, value);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
 * @brief       Function that reads into a GPIO pin
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @param[out]  value Value we want to write on the pin
 * @retval      #THAL_SUCCESSFUL if write succeed
 * @retval      #THAL_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 *
 * Only works in INPUT mode without interrupt
 */
halStatus_t GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst != NULL) && (gpio_inst->mode == GPIO_MODE_INPUT))
    {
        *value = HAL_GPIO_ReadPin(gpio_inst->port, gpio_inst->pin);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioToggle(gpioInst_t *gpio_inst)
 * @brief       Function that toggles a GPIO pin
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @retval      #THAL_SUCCESSFUL if toggle succeed
 * @retval      #THAL_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
halStatus_t GpioToggle(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst != NULL) && ((gpio_inst->mode == GPIO_MODE_OUTPUT_PP) || (gpio_inst->mode == GPIO_MODE_OUTPUT_OD)))
    {
        HAL_GPIO_TogglePin(gpio_inst->port, gpio_inst->pin);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GpioIoctl(gpioInst_t *gpio_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   gpio_inst Instance that contains GPIOs parameters
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_BUSY if action cannot be performed because driver is busy
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
halStatus_t GpioIoctl(gpioInst_t *gpio_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (gpio_inst != NULL)
    {
        /* TO DO */
        (void)(gpio_inst);
        (void)(io_cmd);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GpioClose(gpioInst_t *gpio_inst)
 * @brief           Function that desinit the gpio pin and puts defaults parameters
 * @param[in,out]   gpio_inst Instance that contains GPIOs parameters
 * @retval          #THAL_SUCCESSFUL if changing parameters succeed
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase gpio_inst
 */
halStatus_t GpioClose(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    gpioInst_t null_inst = {
        .port = NULL,
        .pin = 0,
        .mode = 0,
        .pull = 0,
        .speed = 0,
    };

    // Function Core
    if (gpio_inst != NULL)
    {
        HAL_GPIO_DeInit(gpio_inst->port, gpio_inst->pin);
        return_value = GpioDisableInterrupt(gpio_inst);
        *gpio_inst = null_inst;
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioEnableInterrupt(gpioInst_t *gpio_inst)
 * @brief       Function that enables interrupt if needed
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this GPIO
 */
static halStatus_t GpioEnableInterrupt(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst->mode == GPIO_MODE_IT_FALLING) || (gpio_inst->mode == GPIO_MODE_IT_RISING) || (gpio_inst->mode == GPIO_MODE_IT_RISING_FALLING))
    {
        switch ((uint32_t)gpio_inst->pin)
        {
        case GPIO_PIN_0:
            HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI0_IRQn);
            break;
        case GPIO_PIN_1:
            HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI1_IRQn);
            break;
        case GPIO_PIN_2:
            HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI2_IRQn);
            break;
        case GPIO_PIN_3:
            HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI3_IRQn);
            break;
        case GPIO_PIN_4:
            HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI3_IRQn);
            break;
        case GPIO_PIN_5:
        case GPIO_PIN_6:
        case GPIO_PIN_7:
        case GPIO_PIN_8:
        case GPIO_PIN_9:
            HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
            break;
        case GPIO_PIN_10:
        case GPIO_PIN_11:
        case GPIO_PIN_12:
        case GPIO_PIN_13:
        case GPIO_PIN_14:
        case GPIO_PIN_15:
            HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
            break;
        default:
            return_value = THAL_INVALID_PARAM;
            break;
        }
    }

    return return_value;
}

/**
 * @fn          GpioDisableInterrupt(gpioInst_t *gpio_inst)
 * @brief       Function that disables interrupt if needed
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this GPIO
 */
static halStatus_t GpioDisableInterrupt(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst->mode == GPIO_MODE_IT_FALLING) || (gpio_inst->mode == GPIO_MODE_IT_RISING) || (gpio_inst->mode == GPIO_MODE_IT_RISING_FALLING))
    {
        switch (gpio_inst->pin)
        {
        case GPIO_PIN_0:
            HAL_NVIC_DisableIRQ(EXTI0_IRQn);
            break;
        case GPIO_PIN_1:
            HAL_NVIC_DisableIRQ(EXTI1_IRQn);
            break;
        case GPIO_PIN_2:
            HAL_NVIC_DisableIRQ(EXTI2_IRQn);
            break;
        case GPIO_PIN_3:
            HAL_NVIC_DisableIRQ(EXTI3_IRQn);
            break;
        case GPIO_PIN_5:
        case GPIO_PIN_6:
        case GPIO_PIN_7:
        case GPIO_PIN_8:
        case GPIO_PIN_9:
            HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
            break;
        case GPIO_PIN_10:
        case GPIO_PIN_11:
        case GPIO_PIN_12:
        case GPIO_PIN_13:
        case GPIO_PIN_14:
        case GPIO_PIN_15:
            HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
            break;
        default:
            return_value = THAL_INVALID_PARAM;
            break;
        }
    }

    return return_value;
}
