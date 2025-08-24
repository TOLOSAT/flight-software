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
#include "conf/system_peripherals_conf.h"

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

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitSysLEDs(void)
 * @brief   Function that initialises the system leds (used for debug)
 * @return  Nothing
 */
void InitSysLEDs(void)
{
    returnCode_t return_value;

    // First initialises LED Status
    return_value = GpioOpen(&led_status_inst, &led_status_conf);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioWrite(&led_status_inst, SYSLED_OFF);
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
        return_value = GpioOpen(&led_error_inst, &led_error_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            if (GpioWrite(&led_error_inst, SYSLED_OFF) != RET_SUCCESSFUL)
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
    (void)GpioIoctl(&led_status_inst, IOCTL_GPIO_TOGGLE, NULL, 0u);
}

/**
 * @fn      LEDErrorOn(void)
 * @brief   Function that turns on the LED error (and turn off the LED stat)
 * @return  Nothing
 */
void LEDErrorOn(void)
{
    (void)GpioWrite(&led_status_inst, SYSLED_OFF);
    (void)GpioWrite(&led_error_inst, SYSLED_ON);
}
