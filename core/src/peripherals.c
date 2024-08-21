/**
 * @file    peripherals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining peripherals
 * @date    19/08/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitPeripherals(void)
 * @brief   Function that initialises the peripherals
 * @retval  #CORE_SUCCESSFUL if creation succeed
 * @retval  #CORE_ERROR if at least one peripheral initialisation failed
 */
coreStatus_t IN_CORE_TEXT_SECTION InitPeripherals(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    peripheralNo_t peripheral = 0u;

    // Function Core
    while ((peripheral < (peripheralNo_t)NB_PERIPHERALS) && (return_value == CORE_SUCCESSFUL))
    {
        // Initialise peripheral depending of the peripheral type
        halStatus_t test_hal = GEN_HAL_SUCCESSFUL;
        switch (g_peripherals_desc_table[peripheral].type)
        {
        case PERIPHERALS_GPIO:
            test_hal = GpioOpen((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_UART:
            test_hal = UartOpen((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_I2C:
            test_hal = I2cOpen((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_SPI:
            test_hal = SpiOpen((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_OW:
            test_hal = OwOpen((owInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        default:
            return_value = CORE_ERROR;
            break;
        }

        // Check Peripheral init return
        if (test_hal != GEN_HAL_SUCCESSFUL)
        {
            return_value = CORE_ERROR;
        }
        peripheral++;
    }

    return return_value;
}

coreStatus_t IN_CORE_TEXT_SECTION LockPeripherals(peripheralNo_t peripheral)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(peripheral);

    return return_value;
}

coreStatus_t IN_CORE_TEXT_SECTION UnlockPeripherals(peripheralNo_t peripheral)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    (void)(peripheral);

    return return_value;
}