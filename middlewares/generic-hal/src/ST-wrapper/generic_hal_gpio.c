/**
 * @file    generic_hal_gpio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL GPIO functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void GpioGenericIRQHandler(void *param);
static halStatus_t GpioSetupIRQs(const gpioInst_t *gpio_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              GpioOpen(gpioInst_t *gpio_inst)
 * @brief           Function that initialise a GPIO
 * @param[in,out]   gpio_inst Instance that contains GPIOs parameters
 * @retval          #GEN_HAL_SUCCESSFUL if creation succeed
 * @retval          #GEN_HAL_INVALID_PARAM if GPIO port is not available for this board, pin = 0 or one pointer is null
 *
 * Attention : GPIO_PIN_0 != 0, GPIO_PIN_0=0x0001 (cf generic_hal_gpio.h)
 */
halStatus_t IN_GPIO_TEXT_SECTION GpioOpen(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Function Core
    if (gpio_inst != NULL)
    {
        switch ((uint32_t)gpio_inst->port)
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
            return_value = GEN_HAL_INVALID_PARAM;
            break;
        }

        if (return_value == GEN_HAL_SUCCESSFUL)
        {
            GPIO_InitStruct.Pin = gpio_inst->pin;
            GPIO_InitStruct.Mode = gpio_inst->mode;
            GPIO_InitStruct.Pull = gpio_inst->pull;
            GPIO_InitStruct.Speed = gpio_inst->speed;
            HAL_GPIO_Init(gpio_inst->port, &GPIO_InitStruct);
            return_value = GpioSetupIRQs(gpio_inst);
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
 * @brief       Function that writes into a GPIO pin
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @param[in]   value Value we want to write on the pin
 * @retval      #GEN_HAL_SUCCESSFUL if write succeed
 * @retval      #GEN_HAL_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
halStatus_t IN_GPIO_TEXT_SECTION GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (gpio_inst != NULL)
    {
        HAL_GPIO_WritePin(gpio_inst->port, gpio_inst->pin, value);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
 * @brief       Function that reads into a GPIO pin
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @param[out]  value Value we want to write on the pin
 * @retval      #GEN_HAL_SUCCESSFUL if write succeed
 * @retval      #GEN_HAL_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 *
 * Only works in INPUT mode without interrupt
 */
halStatus_t IN_GPIO_TEXT_SECTION GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (gpio_inst != NULL)
    {
        *value = HAL_GPIO_ReadPin(gpio_inst->port, gpio_inst->pin);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioToggle(gpioInst_t *gpio_inst)
 * @brief       Function that toggles a GPIO pin
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @retval      #GEN_HAL_SUCCESSFUL if toggle succeed
 * @retval      #GEN_HAL_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
halStatus_t IN_GPIO_TEXT_SECTION GpioToggle(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst != NULL) && ((gpio_inst->mode == GPIO_MODE_OUTPUT_PP) || (gpio_inst->mode == GPIO_MODE_OUTPUT_OD)))
    {
        HAL_GPIO_TogglePin(gpio_inst->port, gpio_inst->pin);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GpioIoctl(gpioInst_t *gpio_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   gpio_inst Instance that contains GPIOs parameters
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 * @retval          #GEN_HAL_BUSY if action cannot be performed because driver is busy
 * @retval          #GEN_HAL_ERROR if io control encountered an error
 * @retval          #GEN_HAL_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
halStatus_t IN_GPIO_TEXT_SECTION GpioIoctl(gpioInst_t *gpio_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (gpio_inst != NULL)
    {
        /* TO DO */
        (void)(gpio_inst);
        (void)(io_cmd);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GpioClose(gpioInst_t *gpio_inst)
 * @brief           Function that desinit the gpio pin and puts defaults parameters
 * @param[in,out]   gpio_inst Instance that contains GPIOs parameters
 * @retval          #GEN_HAL_SUCCESSFUL if changing parameters succeed
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase gpio_inst
 */
halStatus_t IN_GPIO_TEXT_SECTION GpioClose(gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (gpio_inst != NULL)
    {
        HAL_GPIO_DeInit(gpio_inst->port, gpio_inst->pin);
        return_value = DisableIRQ(gpio_inst->irq_no);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioSetupIRQs(gpioInst_t *gpio_inst)
 * @brief       Function that setups interrupt if needed
 * @param[in]   gpio_inst Instance that contains GPIOs parameters
 * @retval      #GEN_HAL_SUCCESSFUL if changing parameters succeed
 * @retval      #GEN_HAL_INVALID_PARAM if IT is not available for this GPIO
 */
static halStatus_t IN_GPIO_TEXT_SECTION GpioSetupIRQs(const gpioInst_t *gpio_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((gpio_inst->mode == GPIO_MODE_IT_FALLING) || (gpio_inst->mode == GPIO_MODE_IT_RISING) || (gpio_inst->mode == GPIO_MODE_IT_RISING_FALLING))
    {
        IRQHandlerParam_t param = (IRQHandlerParam_t)gpio_inst;
        return_value = RequestIRQ(gpio_inst->irq_no, 5u, GpioGenericIRQHandler, param);
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn              GpioGenericIRQHandler(void *param)
 * @brief           Generic Gpio Handler
 */
static void IN_GPIO_TEXT_SECTION GpioGenericIRQHandler(void *param)
{
    gpioInst_t *gpio_inst = (gpioInst_t *)param;

    // First clear interrupt flag
    if (__HAL_GPIO_EXTI_GET_IT(gpio_inst->pin) != 0x00U)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(gpio_inst->pin);
    }

    // Then executes callback
    if (gpio_inst->callback != NULL)
    {
        gpio_inst->callback();
    }
}