/**
 * @file    tolosat_hal_gpio.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL GPIO functions
 * @date    29/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_gpio Driver GPIO
 * Functions that allows to drive GPIOs independantly of which 
 * hardware or HAL is used
 * @{
 */

#ifndef TOLOSAT_HAL_GPIO_H
#define TOLOSAT_HAL_GPIO_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

#ifndef GPIO_PIN_0
#define GPIO_PIN_0 ((uint16_t)0x0001) /**< Pin 0 selected */
#endif

#ifndef GPIO_PIN_1
#define GPIO_PIN_1 ((uint16_t)0x0002) /**< Pin 1 selected */
#endif

#ifndef GPIO_PIN_2
#define GPIO_PIN_2 ((uint16_t)0x0004) /**< Pin 2 selected */
#endif

#ifndef GPIO_PIN_3
#define GPIO_PIN_3 ((uint16_t)0x0008) /**< Pin 3 selected */
#endif

#ifndef GPIO_PIN_4
#define GPIO_PIN_4 ((uint16_t)0x0010) /**< Pin 4 selected */
#endif

#ifndef GPIO_PIN_5
#define GPIO_PIN_5 ((uint16_t)0x0020) /**< Pin 5 selected */
#endif

#ifndef GPIO_PIN_6
#define GPIO_PIN_6 ((uint16_t)0x0040) /**< Pin 6 selected */
#endif

#ifndef GPIO_PIN_7
#define GPIO_PIN_7 ((uint16_t)0x0080) /**< Pin 7 selected */
#endif

#ifndef GPIO_PIN_8
#define GPIO_PIN_8 ((uint16_t)0x0100) /**< Pin 8 selected */
#endif

#ifndef GPIO_PIN_9
#define GPIO_PIN_9 ((uint16_t)0x0200) /**< Pin 9 selected */
#endif

#ifndef GPIO_PIN_10
#define GPIO_PIN_10 ((uint16_t)0x0400) /**< Pin 10 selected */
#endif

#ifndef GPIO_PIN_11
#define GPIO_PIN_11 ((uint16_t)0x0800) /**< Pin 11 selected */
#endif

#ifndef GPIO_PIN_12
#define GPIO_PIN_12 ((uint16_t)0x1000) /**< Pin 12 selected */
#endif

#ifndef GPIO_PIN_13
#define GPIO_PIN_13 ((uint16_t)0x2000) /**< Pin 13 selected */
#endif

#ifndef GPIO_PIN_14
#define GPIO_PIN_14 ((uint16_t)0x4000) /**< Pin 14 selected */
#endif

#ifndef GPIO_PIN_15
#define GPIO_PIN_15 ((uint16_t)0x8000) /**< Pin 15 selected */
#endif

#ifndef GPIO_PIN_ALL
#define GPIO_PIN_All ((uint16_t)0xFFFF) /**< All pins selected */
#endif

/***************************** Types Definitions *****************************/

/** @brief GPIO port pointer type */
typedef GPIO_TypeDef gpioPort_t;

/** @brief GPIO pin mask type */
typedef uint16_t gpioPin_t;

/** @brief GPIO value enum (SET = 1, RESET = 0) */
typedef GPIO_PinState gpioValue_t;

/** 
 * @struct  gpioInst_t
 * @brief   Struct type definition of a GPIO instance
 */
typedef struct
{
    gpioPort_t *port;   /**< @brief GPIO port pointer (points to the register address) */
    gpioPin_t pin;      /**< @brief GPIO pin mask */
    uint32_t mode;      /**< @brief GPIO mode (input, output, etc) */
    uint32_t pull;      /**< @brief GPIO pull-up / pull-down setting */   
    uint32_t speed;     /**< @brief GPIO speed configuration */
} gpioInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t GpioOpen(gpioInst_t *gpio_inst, gpioPort_t *port, gpioPin_t pin);
halStatus_t GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value);
halStatus_t GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value);
halStatus_t GpioToggle(gpioInst_t *gpio_inst);
halStatus_t GpioIoctl(gpioInst_t *gpio_inst, halIoCtlCmd_t io_cmd);
halStatus_t GpioClose(gpioInst_t *gpio_inst);

#endif /* TOLOSAT_HAL_GPIO_H */

/** 
 * @} 
 * @} 
 */