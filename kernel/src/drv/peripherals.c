/**
 * @file    peripherals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining peripherals
 * @date    19/08/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/peripherals.h"
#include "conf/peripherals_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitPeripherals(void)
 * @brief   Function that initialises the peripherals
 * @retval  #KERNEL_SUCCESSFUL if creation succeed
 * @retval  #KERNEL_ERROR if at least one peripheral initialisation failed
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION InitPeripherals(void)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    peripheralNo_t peripheral = 0u;

    // Function Core
    while ((peripheral < (peripheralNo_t)NB_PERIPHERALS) && (return_value == KERNEL_SUCCESSFUL))
    {
        // Initialise peripheral depending of the peripheral type
        switch (g_peripherals_desc_table[peripheral].type)
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
            return_value = KERNEL_ERROR;
            break;
        }

        // Check peripheral init return
        if (return_value == KERNEL_SUCCESSFUL)
        {
            // Then initialise mutex
            g_peripherals_desc_table[peripheral].mutex = xSemaphoreCreateMutexStatic(g_peripherals_conf_table[peripheral].p_mutex_queue);
            portENABLE_INTERRUPTS(); // WORKAROUND : FreeRTOS API disable interrupts by default if scheduler has not been started.
            if (g_peripherals_desc_table[peripheral].mutex == NULL)
            {
                return_value = KERNEL_ERROR;
            }
        }
        peripheral++;
    }

    return return_value;
}

/**
 * @fn          PeripheralWrite(peripheralNo_t peripheral, peripheralData_t *data, peripheralSize_t size, uint32_t extra_info)
 * @brief       Function that writes data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[in]   data        Data that will be sent to the device
 * @param[in]   size        Size of the data
 * @param[in]   extra_info  Extra data if relevant (e.g. slave adress for I2C)
 * @retval      #KERNEL_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #KERNEL_ERROR if peripheral writing encountered an error
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION PeripheralWrite(peripheralNo_t peripheral, peripheralData_t *data, peripheralSize_t size, uint32_t extra_info)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (peripheral < (peripheralNo_t)NB_PERIPHERALS))
    {
        // First get peripheral and type
        peripheralType_t type = g_peripherals_desc_table[peripheral].type;

        // Then use the correct driver to write
        switch (type)
        {
        case PERIPHERALS_GPIO:
            if (size == sizeof(peripheralData_t))
            {
                return_value = GpioWrite((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, *data);
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
            break;
        case PERIPHERALS_UART:
            return_value = UartWrite((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        case PERIPHERALS_I2C:
            return_value = I2cWrite((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, extra_info, data, size);
            break;
        case PERIPHERALS_SPI:
            return_value = SpiWrite((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        case PERIPHERALS_OW:
            return_value = OwWrite((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        default:
            return_value = KERNEL_ERROR;
            break;
        }
    }

    return return_value;
}

/**
 * @fn          PeripheralRead(peripheralNo_t peripheral, peripheralData_t *data, peripheralSize_t size, uint32_t extra_info)
 * @brief       Function that reads data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[out]  data        Data that will be received to the peripheral
 * @param[in]   size        Size of the data
 * @param[in]   extra_info  Extra data if relevant (e.g. slave adress for I2C)
 * @retval      #KERNEL_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #KERNEL_ERROR if peripheral reading encountered an error
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION PeripheralRead(peripheralNo_t peripheral, peripheralData_t *data, peripheralSize_t size, uint32_t extra_info)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((data != NULL) && (peripheral < (peripheralNo_t)NB_PERIPHERALS))
    {
        // First get peripheral and type
        peripheralType_t type = g_peripherals_desc_table[peripheral].type;

        // Then use the correct driver to read
        switch (type)
        {
        case PERIPHERALS_GPIO:
            if (size == sizeof(peripheralData_t))
            {
                return_value = GpioRead((gpioInst_t *) g_peripherals_desc_table[peripheral].p_instance, data);
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
            break;
        case PERIPHERALS_UART:
            return_value = UartRead((uartInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        case PERIPHERALS_I2C:
            return_value = I2cRead((i2cInst_t *) g_peripherals_desc_table[peripheral].p_instance, extra_info, data, size);
            break;
        case PERIPHERALS_SPI:
            return_value = SpiRead((spiInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, NULL, size); // TO DO : improve with read-write
            break;
        case PERIPHERALS_OW:
            return_value = OwRead((owInst_t *) g_peripherals_desc_table[peripheral].p_instance, data, size);
            break;
        default:
            return_value = KERNEL_ERROR;
            break;
        }
    }

    return return_value;
}

/**
 * @fn              PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the peripheral
 * @param[in]       peripheral  Peripheral numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data size (if any)
 * @retval          #KERNEL_INVALID_PARAM if peripheral is not valid
 * @retval          #KERNEL_ERROR if peripheral IOCTL encountered an error
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (peripheral < (peripheralNo_t)NB_PERIPHERALS)
    {
        // First get peripheral and type
        peripheralType_t type = g_peripherals_desc_table[peripheral].type;

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
            return_value = KERNEL_ERROR;
            break;
        }
    }

    return return_value;
}

/**
 * @fn          PeripheralLock(peripheralNo_t peripheral)
 * @brief       Lock the peripheral with a mutex
 * @param[in]   peripheral Peripheral that will be locked
 * @retval      #KERNEL_ERROR if cannot acquires the mutex
 * @retval      #KERNEL_SUCCESSFUL else 
 * 
 * @warning     Cannot be used during init or ISR because of mutexes
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION PeripheralLock(peripheralNo_t peripheral)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    BaseType_t mutex_status = xSemaphoreTake(g_peripherals_desc_table[peripheral].mutex, portMAX_DELAY);
    if (mutex_status != pdTRUE)
    {
        return_value = KERNEL_ERROR;
    }

    return return_value;
}

/**
 * @fn          PeripheralUnlock(peripheralNo_t peripheral)
 * @brief       Unlock the peripheral (which has been locked with a mutex)
 * @param[in]   peripheral Peripheral that will be unlocked
 * @retval      #KERNEL_ERROR if cannot release the mutex
 * @retval      #KERNEL_SUCCESSFUL else
 * 
 * @warning     Cannot be used during init or ISR because of mutexes
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION PeripheralUnlock(peripheralNo_t peripheral)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    BaseType_t mutex_status = xSemaphoreGive(g_peripherals_desc_table[peripheral].mutex);
    if (mutex_status != pdTRUE)
    {
        return_value = KERNEL_ERROR;
    }

    return return_value;
}