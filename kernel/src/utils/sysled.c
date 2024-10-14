/**
 * @file    sysled.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for system LED handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/sysled.h"
#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     ledstat_inst
 * @brief   Status LED instance declaration
 */
static gpioInst_t ledstat_inst = {
    .port = BLUE_LED_PORT,
    .pin = BLUE_LED_PIN,
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
    .port = RED_LED_PORT,
    .pin = RED_LED_PIN,
    .mode = GPIO_MODE_OUTPUT_PP,
    .pull = GPIO_NOPULL,
    .speed = GPIO_SPEED_FREQ_LOW,
    .irq_no = IRQ_NONE,
    .callback = NULL,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitSysLED(void)
 * @brief   Function that initialises the system leds (used for debug)
 * @retval  #RET_ERROR if one of the system led initialisation failed
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitSysLED(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    return_value = GpioOpen(&ledstat_inst);
    if (return_value == RET_SUCCESSFUL)
    {
        return_value = GpioOpen(&lederror_inst);
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
    (void)GpioIoctl(&ledstat_inst, GPIO_IOCTL_TOGGLE, NULL, 0u);
}

/**
 * @fn      LEDErrorOn(void)
 * @brief   Function that turns on the LED error (and turn off the LED stat)
 * @return  Nothing
 */
void LEDErrorOn(void)
{
    (void)GpioWrite(&ledstat_inst, GPIO_PIN_RESET);
    (void)GpioWrite(&lederror_inst, GPIO_PIN_SET);
}
