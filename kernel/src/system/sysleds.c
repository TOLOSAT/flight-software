/**
 * @file    sysleds.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for system LEDs handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "system/sysleds.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

#ifdef CONFIG_HAS_INVERTED_SYSLED_LOGIC
#define SYSLED_ON   GPIO_PIN_RESET  /**< Define GPIO line state for which the LED is ON */
#define SYSLED_OFF  GPIO_PIN_SET    /**< Define GPIO line state for which the LED is OFF */
#else
#define SYSLED_ON   GPIO_PIN_SET    /**< Define GPIO line state for which the LED is ON */
#define SYSLED_OFF  GPIO_PIN_RESET  /**< Define GPIO line state for which the LED is OFF */
#endif

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     ledstat_inst
 * @brief   Status LED instance declaration
 */
static gpioInst_t ledstat_inst = {
    .port = LED_STATUS_PORT,
    .pin = LED_STATUS_PIN,
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
    .irq_no = IRQ_NONE,
    .callback = NULL,
};

/**
 * @var     lederror_inst
 * @brief   Error LED instance declaration
 */
static gpioInst_t lederror_inst = {
    .port = LED_ERROR_PORT,
    .pin = LED_ERROR_PIN,
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
    .irq_no = IRQ_NONE,
    .callback = NULL,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitSysLEDs(void)
 * @brief   Function that initialises the system leds (used for debug)
 * @retval  #RET_ERROR if one of the system led initialisation failed
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitSysLEDs(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // First initialises LED Status
    return_value = GpioOpen(&ledstat_inst);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioWrite(&ledstat_inst, SYSLED_OFF);
    }

    // Then initialises LED Error
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioOpen(&lederror_inst);
        if (return_value == RET_SUCCESSFUL)
        {
            return_value = GpioWrite(&lederror_inst, SYSLED_OFF);
        }
    }

    return return_value;
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
