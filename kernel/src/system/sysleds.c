/**
 * @file    sysleds.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for system LEDs handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/sysleds.h"
#include "drv/peripherals.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#ifdef CONFIG_HAS_INVERTED_SYSLED_LOGIC
#define SYSLED_ON  GPIO_PIN_RESET /**< Define GPIO line state for which the LED is ON */
#define SYSLED_OFF GPIO_PIN_SET   /**< Define GPIO line state for which the LED is OFF */
#else
#define SYSLED_ON  GPIO_PIN_SET   /**< Define GPIO line state for which the LED is ON */
#define SYSLED_OFF GPIO_PIN_RESET /**< Define GPIO line state for which the LED is OFF */
#endif

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     ledstat_inst
 * @brief   Status LED instance declaration
 */
static gpioInst_t ledstat_inst = { 0 };

/**
 * @var     lederror_inst
 * @brief   Error LED instance declaration
 */
static gpioInst_t lederror_inst = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitSysLEDs(void)
 * @brief   Function that initialises the system leds (used for debug)
 * @return  Nothing
 */
void InitSysLEDs(void)
{
    returnCode_t return_value;
    gpioConf_t ledstat_conf = {
        .port   = LED_STATUS_PORT,
        .pin    = LED_STATUS_PIN,
        .inout  = GPIO_MODE_OUTPUT_PP,
        .pull   = GPIO_NOPULL,
        .speed  = GPIO_SPEED_FREQ_LOW,
        .irq_no = IRQ_NONE,
    };
    gpioConf_t lederror_conf = {
        .port   = LED_ERROR_PORT,
        .pin    = LED_ERROR_PIN,
        .inout  = GPIO_MODE_OUTPUT_PP,
        .pull   = GPIO_NOPULL,
        .speed  = GPIO_SPEED_FREQ_LOW,
        .irq_no = IRQ_NONE,
    };

    // First initialises LED Status
    return_value = GpioOpen(&ledstat_inst, &ledstat_conf);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioWrite(&ledstat_inst, SYSLED_OFF);
        if (return_value != RET_SUCCESSFUL)
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }

    // Then initialises LED Error
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioOpen(&lederror_inst, &lederror_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            if (GpioWrite(&lederror_inst, SYSLED_OFF) != RET_SUCCESSFUL)
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }
}

/**
 * @fn      LEDStatToggle(void)
 * @brief   Function that toggles the LED stat
 * @return  Nothing
 */
void LEDStatToggle(void)
{
    (void)GpioIoctl(&ledstat_inst, IOCTL_GPIO_TOGGLE, NULL, 0u);
}

/**
 * @fn      LEDErrorOn(void)
 * @brief   Function that turns on the LED error (and turn off the LED stat)
 * @return  Nothing
 */
void LEDErrorOn(void)
{
    (void)GpioWrite(&ledstat_inst, SYSLED_OFF);
    (void)GpioWrite(&lederror_inst, SYSLED_ON);
}
