/**
 * @file    peripherals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining peripherals
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/peripherals.h"
#include "core/tasks.h"
#include "core/signals.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t PeripheralLock(peripheralNo_t peripheral);
static returnCode_t PeripheralUnlock(peripheralNo_t peripheral);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitPeripherals(void)
 * @brief   Function that initialises the peripherals
 * @retval  #RET_SUCCESSFUL if creation succeed
 * @retval  #RET_ERROR if at least one peripheral initialisation failed
 */
returnCode_t InitPeripherals(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    peripheralNo_t peripheral = 0u;

    // Function Core
    while ((peripheral < NB_PERIPHERALS) && (return_value == RET_SUCCESSFUL))
    {
        // Initialise peripheral depending of the peripheral type
        switch (g_peripherals_conf_table[peripheral].type)
        {
        case PERIPHERALS_GPIO:
            return_value = GpioOpen((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_UART:
            return_value = UartOpen((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_I2C:
            return_value = I2cOpen((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_SPI:
            return_value = SpiOpen((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        case PERIPHERALS_OW:
            return_value = OwOpen((owInst_t *) g_peripherals_desc_table[peripheral].p_instance);
            break;
        default:
            return_value = RET_ERROR;
            break;
        }

        // Check peripheral init return
        if (return_value == RET_SUCCESSFUL)
        {
            // Then initialise mutex
            g_peripherals_desc_table[peripheral].mutex = xSemaphoreCreateMutexStatic(g_peripherals_conf_table[peripheral].p_mutex_queue);
            portENABLE_INTERRUPTS(); // WORKAROUND : FreeRTOS API disable interrupts by default if scheduler has not been started.
            if (g_peripherals_desc_table[peripheral].mutex == NULL)
            {
                return_value = RET_ERROR;
            }
        }
        peripheral++;
    }

    return return_value;
}

/**
 * @fn          PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length, uint32_t extra_info)
 * @brief       Function that writes data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[in]   data        Data that will be sent to the device
 * @param[in]   length      Length of the data
 * @param[in]   extra_info  Extra data if relevant (e.g. slave adress for I2C)
 * @retval      #RET_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #RET_ERROR if peripheral writing encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length, uint32_t extra_info)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    returnCode_t test_lock;

    // Function Core
    if ((data != NULL) && (peripheral < NB_PERIPHERALS))
    {
        // First lock peripheral
        test_lock = PeripheralLock(peripheral);
        if (test_lock == RET_SUCCESSFUL)
        {
            // Then get peripheral and type
            peripheralType_t type = g_peripherals_conf_table[peripheral].type;

            // Then use the correct driver to write
            switch (type)
            {
            case PERIPHERALS_GPIO:
                if (length == sizeof(uint8_t))
                {
                    return_value = GpioWrite((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, *data);
                }
                else
                {
                    return_value = RET_ERROR;
                }
                break;
            case PERIPHERALS_UART:
                return_value = UartWrite((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, length);
                break;
            case PERIPHERALS_I2C:
                return_value = I2cWrite((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, extra_info, data, length);
                break;
            case PERIPHERALS_SPI:
                return_value = SpiWrite((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, length);
                break;
            case PERIPHERALS_OW:
                return_value = OwWrite((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, length);
                break;
            default:
                return_value = RET_ERROR;
                break;
            }

            // Unlock anyway
            test_lock = PeripheralUnlock(peripheral);
            if (test_lock != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length, uint32_t extra_info)
 * @brief       Function that reads data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[out]  data        Data that will be received to the peripheral
 * @param[in]   length      Length of the data
 * @param[in]   extra_info  Extra data if relevant (e.g. slave adress for I2C)
 * @retval      #RET_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #RET_ERROR if peripheral reading encountered an error
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length, uint32_t extra_info)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    returnCode_t test_lock;

    // Function Core
    if ((data != NULL) && (peripheral < NB_PERIPHERALS))
    {
        // First lock peripheral
        test_lock = PeripheralLock(peripheral);
        if (test_lock == RET_SUCCESSFUL)
        {
            // Then get peripheral and type
            peripheralType_t type = g_peripherals_conf_table[peripheral].type;

            // Then use the correct driver to read
            switch (type)
            {
            case PERIPHERALS_GPIO:
                if (length == sizeof(uint8_t))
                {
                    return_value = GpioRead((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, data);
                }
                else
                {
                    return_value = RET_ERROR;
                }
                break;
            case PERIPHERALS_UART:
                return_value = UartRead((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, length);
                break;
            case PERIPHERALS_I2C:
                return_value = I2cRead((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, extra_info, data, length);
                break;
            case PERIPHERALS_SPI:
                return_value = SpiRead((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, NULL, length); // TO DO : improve with read-write
                break;
            case PERIPHERALS_OW:
                return_value = OwRead((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, length);
                break;
            default:
                return_value = RET_ERROR;
                break;
            }

            // Unlock anyway
            test_lock = PeripheralUnlock(peripheral);
            if (test_lock != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the peripheral
 * @param[in]       peripheral  Peripheral numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data length (if any)
 * @retval          #RET_INVALID_PARAM if peripheral is not valid
 * @retval          #RET_ERROR if peripheral IOCTL encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    returnCode_t test_lock;

    // Function Core
    if (peripheral < NB_PERIPHERALS)
    {
        // First lock peripheral
        test_lock = PeripheralLock(peripheral);
        if (test_lock == RET_SUCCESSFUL)
        {
            // First check if an RX/TX asynchronous action is required or not.
            if (cmd == IOCTL_PERIPHERAL_START_RX)
            {
                // Set owner
                return_value = GetCurrentTask(&g_peripherals_desc_table[peripheral].rx_owner);
            }
            else if (cmd == IOCTL_PERIPHERAL_START_TX)
            {
                // Set owner
                return_value = GetCurrentTask(&g_peripherals_desc_table[peripheral].tx_owner);
            }
            else if (cmd == IOCTL_PERIPHERAL_CHECK_RX_COMPLETED)
            {
                // Wait signal
                return_value = WaitSignal(SIGNAL_PERIPHERAL_RX_DONE);
            }
            else if (cmd == IOCTL_PERIPHERAL_CHECK_TX_COMPLETED)
            {
                // Wait signal
                return_value = WaitSignal(SIGNAL_PERIPHERAL_TX_DONE);
            }
            else if (cmd == IOCTL_PERIPHERAL_END_RX)
            {
                // Reset owner
                g_peripherals_desc_table[peripheral].rx_owner = NO_TASK;
            }
            else if (cmd == IOCTL_PERIPHERAL_END_TX)
            {
                // Reset owner
                g_peripherals_desc_table[peripheral].tx_owner = NO_TASK;
            }
            else
            {
                // Peripheral Specific IOCTL
                // Nothing to do prior IOCTL
            }

            // If no error occured, continue by doing the type specific IOCTL
            if (return_value == RET_SUCCESSFUL)
            {
                // Then get peripheral and type
                peripheralType_t type = g_peripherals_conf_table[peripheral].type;

                // Then use the correct driver to write
                switch (type)
                {
                case PERIPHERALS_GPIO:
                    return_value = GpioIoctl((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
                    break;
                case PERIPHERALS_UART:
                    return_value = UartIoctl((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
                    break;
                case PERIPHERALS_I2C:
                    return_value = I2cIoctl((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
                    break;
                case PERIPHERALS_SPI:
                    return_value = SpiIoctl((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
                    break;
                case PERIPHERALS_OW:
                    return_value = OwIoctl((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, cmd, data, data_size);
                    break;
                default:
                    return_value = RET_ERROR;
                    break;
                }
            }

            // Unlock anyway
            test_lock = PeripheralUnlock(peripheral);
            if (test_lock != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          PeripheralLock(peripheralNo_t peripheral)
 * @brief       Lock the peripheral with a mutex
 * @param[in]   peripheral  Peripheral that will be locked
 * @retval      #RET_ERROR if cannot acquires the mutex
 * @retval      #RET_SUCCESSFUL else
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static returnCode_t PeripheralLock(peripheralNo_t peripheral)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    BaseType_t mutex_status = xSemaphoreTake(g_peripherals_desc_table[peripheral].mutex, portMAX_DELAY);
    if (mutex_status != pdTRUE)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn          PeripheralUnlock(peripheralNo_t peripheral)
 * @brief       Unlock the peripheral (which has been locked with a mutex)
 * @param[in]   peripheral  Peripheral that will be unlocked
 * @retval      #RET_ERROR if cannot release the mutex
 * @retval      #RET_SUCCESSFUL else
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static returnCode_t PeripheralUnlock(peripheralNo_t peripheral)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    BaseType_t mutex_status = xSemaphoreGive(g_peripherals_desc_table[peripheral].mutex);
    if (mutex_status != pdTRUE)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}