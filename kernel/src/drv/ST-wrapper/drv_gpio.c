/**
 * @file    drv_gpio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GPIO functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_gpio.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t GpioToggle(gpioInst_t *gpio_inst);
static void GpioGenericIRQHandler(void *param);
static returnCode_t GpioSetupIRQs(gpioInst_t *gpio_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              GpioOpen(gpioInst_t *gpio_inst)
 * @brief           Function that initialise a GPIO
 * @param[in,out]   gpio_inst   Instance that contains GPIOs parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if GPIO port is not available for this board, pin = 0 or one pointer is null
 *
 * Attention : GPIO_PIN_0 != 0, GPIO_PIN_0=0x0001 (cf drv_gpio.h)
 */
returnCode_t GpioOpen(gpioInst_t *gpio_inst)
{
    returnCode_t return_value        = RET_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    // Check parameter(s)
    if (gpio_inst != NULL)
    {
        switch ((uint32_t)gpio_inst->port)
        {
            case GPIOA_BASE :
                __HAL_RCC_GPIOA_CLK_ENABLE();
                break;
            case GPIOB_BASE :
                __HAL_RCC_GPIOB_CLK_ENABLE();
                break;
            case GPIOC_BASE :
                __HAL_RCC_GPIOC_CLK_ENABLE();
                break;
#if defined(GPIOD_BASE)
            case GPIOD_BASE :
                __HAL_RCC_GPIOD_CLK_ENABLE();
                break;
#endif
#if defined(GPIOE_BASE)
            case GPIOE_BASE :
                __HAL_RCC_GPIOE_CLK_ENABLE();
                break;
#endif
#if defined(GPIOF_BASE)
            case GPIOF_BASE :
                __HAL_RCC_GPIOF_CLK_ENABLE();
                break;
#endif
#if defined(GPIOG_BASE)
            case GPIOG_BASE :
                __HAL_RCC_GPIOG_CLK_ENABLE();
                break;
#endif
#if defined(GPIOH_BASE)
            case GPIOH_BASE :
                __HAL_RCC_GPIOH_CLK_ENABLE();
                break;
#endif
#if defined(GPIOI_BASE)
            case GPIOI_BASE :
                __HAL_RCC_GPIOI_CLK_ENABLE();
                break;
#endif
#if defined(GPIOJ_BASE)
            case GPIOJ_BASE :
                __HAL_RCC_GPIOJ_CLK_ENABLE();
                break;
#endif
#if defined(GPIOK_BASE)
            case GPIOK_BASE :
                __HAL_RCC_GPIOK_CLK_ENABLE();
                break;
#endif
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }

        if (return_value == RET_SUCCESSFUL)
        {
            GPIO_InitStruct.Pin   = gpio_inst->pin;
            GPIO_InitStruct.Mode  = gpio_inst->mode;
            GPIO_InitStruct.Pull  = gpio_inst->pull;
            GPIO_InitStruct.Speed = gpio_inst->speed;
            HAL_GPIO_Init(gpio_inst->port, &GPIO_InitStruct);
            return_value = GpioSetupIRQs(gpio_inst);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
 * @brief       Function that writes into a GPIO pin
 * @param[in]   gpio_inst   Instance that contains GPIOs parameters
 * @param[in]   value       Value we want to write on the pin
 * @retval      #RET_SUCCESSFUL if write succeed
 * @retval      #RET_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
returnCode_t GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (gpio_inst != NULL)
    {
        HAL_GPIO_WritePin(gpio_inst->port, gpio_inst->pin, value);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
 * @brief       Function that reads into a GPIO pin
 * @param[in]   gpio_inst   Instance that contains GPIOs parameters
 * @param[out]  value       Value we want to write on the pin
 * @retval      #RET_SUCCESSFUL if write succeed
 * @retval      #RET_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 *
 * Only works in INPUT mode without interrupt
 */
returnCode_t GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (gpio_inst != NULL)
    {
        *value = HAL_GPIO_ReadPin(gpio_inst->port, gpio_inst->pin);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GpioIoctl(gpioInst_t *gpio_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   gpio_inst   Instance that contains GPIOs parameters
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t GpioIoctl(gpioInst_t *gpio_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(data);
    (void)(data_size);

    // Check parameter(s)
    if (gpio_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_GPIO_TOGGLE :
                return_value = GpioToggle(gpio_inst);
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              GpioClose(gpioInst_t *gpio_inst)
 * @brief           Function that desinit the gpio pin and puts defaults parameters
 * @param[in,out]   gpio_inst   Instance that contains GPIOs parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 *
 * This function erase gpio_inst
 */
returnCode_t GpioClose(gpioInst_t *gpio_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (gpio_inst != NULL)
    {
        HAL_GPIO_DeInit(gpio_inst->port, gpio_inst->pin);
        return_value = DisableIRQ(gpio_inst->irq_no);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioToggle(gpioInst_t *gpio_inst)
 * @brief       Function that toggles a GPIO pin
 * @param[in]   gpio_inst   Instance that contains GPIOs parameters
 * @retval      #RET_SUCCESSFUL if toggle succeed
 * @retval      #RET_INVALID_PARAM if GPIO is not an output or instance is a null pointer
 */
static returnCode_t GpioToggle(gpioInst_t *gpio_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((gpio_inst != NULL) && ((gpio_inst->mode == GPIO_MODE_OUTPUT_PP) || (gpio_inst->mode == GPIO_MODE_OUTPUT_OD)))
    {
        HAL_GPIO_TogglePin(gpio_inst->port, gpio_inst->pin);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GpioSetupIRQs(gpioInst_t *gpio_inst)
 * @brief       Function that setups interrupt if needed
 * @param[in]   gpio_inst   Instance that contains GPIOs parameters
 * @retval      #RET_SUCCESSFUL if changing parameters succeed
 * @retval      #RET_INVALID_PARAM if IT is not available for this GPIO
 */
static returnCode_t GpioSetupIRQs(gpioInst_t *gpio_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((gpio_inst->mode == GPIO_MODE_IT_FALLING) || (gpio_inst->mode == GPIO_MODE_IT_RISING) || (gpio_inst->mode == GPIO_MODE_IT_RISING_FALLING))
    {
        // Set gpio inst as the interrupt parameter to pass it to the interrupt routine
        IRQHandlerParam_t param = (IRQHandlerParam_t)gpio_inst;
        // Request the interrupt
        return_value = RequestIRQ(gpio_inst->irq_no, 5u, GpioGenericIRQHandler, param);
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn              GpioGenericIRQHandler(void *param)
 * @brief           Generic Gpio Handler
 */
static void GpioGenericIRQHandler(void *param)
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