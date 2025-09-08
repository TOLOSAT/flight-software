/**
 * @file    drv_gpio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GPIO functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_gpio.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t GpioToggle(gpioInst_t *gpio_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              GpioOpen(gpioInst_t *gpio_inst, const gpioConf_t *const gpio_conf)
 * @brief           Function that initialise a GPIO
 * @param[in,out]   gpio_inst   Instance that contains GPIO handlers
 * @param[in]       gpio_conf   Configuration that contains GPIO parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if GPIO port is not available for this board, pin = 0 or one pointer is null
 *
 * Attention : GPIO_PIN_0 != 0, GPIO_PIN_0=0x0001 (cf hal_gpio.h)
 */
returnCode_t GpioOpen(gpioInst_t *gpio_inst, const gpioConf_t *const gpio_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((gpio_inst != NULL) && (gpio_conf != NULL) && (gpio_conf->pin != 0u) && (gpio_conf->port != NULL))
    {
        gpio_inst->port          = gpio_conf->port;
        gpio_inst->pin           = gpio_conf->pin;
        gpio_inst->direction     = ((gpio_conf->mode & GPIO_OUTPUT_MASK) == GPIO_OUTPUT_MASK) ? GPIO_DIRECTION_OUTPUT : GPIO_DIRECTION_INPUT;
        HAL_StatusTypeDef status = cmsdk_GpioInit(gpio_inst->port, gpio_inst->pin, gpio_conf->mode);
        // Check return value
        switch (status)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT :
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY :
                return_value = RET_NOT_AVAILABLE;
                break;
            default :
                KernelPanic();
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
    if ((gpio_inst != NULL) && (gpio_inst->direction == GPIO_DIRECTION_OUTPUT))
    {
        HAL_StatusTypeDef status = cmsdk_GpioWritePin(gpio_inst->port, gpio_inst->pin, value);
        // Check return value
        switch (status)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT :
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY :
                return_value = RET_NOT_AVAILABLE;
                break;
            default :
                KernelPanic();
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
        HAL_StatusTypeDef status = cmsdk_GpioReadPin(gpio_inst->port, gpio_inst->pin, value);
        // Check return value
        switch (status)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT :
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY :
                return_value = RET_NOT_AVAILABLE;
                break;
            default :
                KernelPanic();
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
        (void)(gpio_inst);
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
    if ((gpio_inst != NULL) && (gpio_inst->direction == GPIO_DIRECTION_OUTPUT))
    {
        HAL_StatusTypeDef status = cmsdk_GpioTogglePin(gpio_inst->port, gpio_inst->pin);
        // Check return value
        switch (status)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT :
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY :
                return_value = RET_NOT_AVAILABLE;
                break;
            default :
                KernelPanic();
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
