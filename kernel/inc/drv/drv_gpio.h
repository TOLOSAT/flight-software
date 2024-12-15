/**
 * @file    drv_gpio.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GPIO functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_gpio GPIO Driver
 * @brief Abstraction layer for controlling GPIOs
 * @{
 */

#ifndef DRV_GPIO_H
#define DRV_GPIO_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "core/irq.h"

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

// GPIO Mode compatibility
#ifndef GPIO_MODE_OUTPUT_PP
#define GPIO_MODE_OUTPUT_PP GPIO_MODE_OUTPUT    /**< Redefinition of GPIO_MODE_OUTPUT_PP if not existing */
#endif

#ifndef GPIO_MODE_OUTPUT_OD
#define GPIO_MODE_OUTPUT_OD GPIO_MODE_OUTPUT    /**< Redefinition of GPIO_MODE_OUTPUT_OD if not existing */
#endif

#ifndef GPIO_MODE_IT_FALLING
#define GPIO_MODE_IT_FALLING GPIO_MODE_INTERRUPT_FALLING_EDGE   /**< Redefinition of GPIO_MODE_IT_FALLING if not existing */
#endif

#ifndef GPIO_MODE_IT_RISING
#define GPIO_MODE_IT_RISING GPIO_MODE_INTERRUPT_RISING_EDGE /**< Redefinition of GPIO_MODE_IT_RISING if not existing */
#endif

#ifndef GPIO_NOPULL
#define GPIO_NOPULL        (0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define GPIO_PULLUP        (0x00000001U)   /*!< Pull-up activation                  */
#define GPIO_PULLDOWN      (0x00000002U)   /*!< Pull-down activation                */
#endif

#ifndef GPIO_SPEED_FREQ_LOW
#define GPIO_SPEED_FREQ_LOW         (0x00000000U)  /*!< Low speed     */
#define GPIO_SPEED_FREQ_MEDIUM      (0x00000001U)  /*!< Medium speed  */
#define GPIO_SPEED_FREQ_HIGH        (0x00000002U)  /*!< Fast speed    */
#define GPIO_SPEED_FREQ_VERY_HIGH   (0x00000003U)  /*!< High speed    */
#endif

/***************************** Types Definitions *****************************/

/** @brief GPIO port pointer type */
typedef GPIO_TypeDef gpioPort_t;

/** @brief GPIO pin mask type */
typedef uint16_t gpioPin_t;

/** @brief GPIO value enum (SET = 1, RESET = 0) */
typedef GPIO_PinState gpioValue_t;

/** @brief GPIO interrupt callback definition */
typedef void (*gpioCallBack_t)(void);

/**
 * @struct  gpioInst_t
 * @brief   Struct type definition of a GPIO instance
 */
typedef struct
{
    gpioPort_t *port;           /**< @brief GPIO port pointer (points to the register address) */
    gpioPin_t pin;              /**< @brief GPIO pin mask */
    uint32_t mode;              /**< @brief GPIO mode (input, output, etc) */
    uint32_t pull;              /**< @brief GPIO pull-up / pull-down setting */
    uint32_t speed;             /**< @brief GPIO speed configuration */
    IRQNo_t irq_no;             /**< @brief GPIO related interrupt (IRQ_NONE if none) */
    gpioCallBack_t callback;    /**< @brief GPIO interrupt callback (if any) */
} gpioInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t GpioOpen(gpioInst_t *gpio_inst);
extern returnCode_t GpioWrite(gpioInst_t *gpio_inst, gpioValue_t value);
extern returnCode_t GpioRead(gpioInst_t *gpio_inst, gpioValue_t *value);
extern returnCode_t GpioIoctl(gpioInst_t *gpio_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t GpioClose(gpioInst_t *gpio_inst);

#endif /* DRV_GPIO_H */

/**
 * @}
 * @}
 * @}
 */