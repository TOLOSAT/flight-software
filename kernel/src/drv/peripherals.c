/**
 * @file    peripherals.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining peripherals
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals.h"
#include "core/tasks.h"
#include "core/signals.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t PeripheralSetCallback(peripheralNo_t peripheral);
static void PeripheralLock(peripheralNo_t peripheral);
static void PeripheralUnlock(peripheralNo_t peripheral);
static void PeripheralLockRX(peripheralNo_t peripheral);
static void PeripheralUnlockRX(peripheralNo_t peripheral);
static void PeripheralLockTX(peripheralNo_t peripheral);
static void PeripheralUnlockTX(peripheralNo_t peripheral);
static void PeripheralRXCallback(void *param);
static void PeripheralTXCallback(void *param);

/*************************** Variables Definitions ***************************/

static bool peripherals_mutexes_initialised = false;

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitPeripherals(void)
 * @brief   Function that initialises the peripherals
 * @return  Nothing
 */
void InitPeripherals(void)
{
    returnCode_t return_value;
    peripheralNo_t peripheral = 1u;

    // Init all peripherals
    while (PERIPHERAL_CONF(peripheral).peripheral != NO_PERIPHERAL)
    {
        drivingMode_t driving_mode;
        // Initialise peripheral depending of the peripheral type and get driving mode
        switch (PERIPHERAL_CONF(peripheral).type)
        {
            case PERIPHERAL_GPIO :
                return_value =
                    GpioOpen((gpioInst_t *)PERIPHERAL_DESC(peripheral).p_inst, (const gpioConf_t *const)PERIPHERAL_CONF(peripheral).p_conf);
                driving_mode = POLLING_MODE;
                break;
            case PERIPHERAL_UART :
                return_value =
                    UartOpen((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, (const uartConf_t *const)PERIPHERAL_CONF(peripheral).p_conf);
                driving_mode = ((const uartConf_t *const)PERIPHERAL_CONF(peripheral).p_conf)->default_mode;
                break;
            case PERIPHERAL_I2C :
                return_value = I2cOpen((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, (const i2cConf_t *const)PERIPHERAL_CONF(peripheral).p_conf);
                driving_mode = ((const i2cConf_t *const)PERIPHERAL_CONF(peripheral).p_conf)->default_mode;
                break;
            case PERIPHERAL_SPI :
                return_value = SpiOpen((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, (const spiConf_t *const)PERIPHERAL_CONF(peripheral).p_conf);
                driving_mode = ((const spiConf_t *const)PERIPHERAL_CONF(peripheral).p_conf)->default_mode;
                break;
            case PERIPHERAL_OW :
                return_value = OwOpen((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, (const owConf_t *const)PERIPHERAL_CONF(peripheral).p_conf);
                driving_mode = ((const owConf_t *const)PERIPHERAL_CONF(peripheral).p_conf)->default_mode;
                break;
            default :
                KernelPanic();
                break;
        }

        // Check peripheral init return
        if (return_value != RET_SUCCESSFUL)
        {
            KernelPanic();
        }

        // Checks that the synchronisation parameter is consistent with the driving type
        if (((driving_mode == POLLING_MODE) && (PERIPHERAL_CONF(peripheral).synchronisation == PERIPHERAL_ASYNCHRONOUS))
            || ((driving_mode == INTERRUPT_MODE) && (PERIPHERAL_CONF(peripheral).synchronisation == PERIPHERAL_SYNCHRONOUS))
            || ((driving_mode == DMA_MODE) && (PERIPHERAL_CONF(peripheral).synchronisation == PERIPHERAL_SYNCHRONOUS)))
        {
            KernelPanic();
        }

        // Then set callback
        return_value = PeripheralSetCallback(peripheral);
        if (return_value != RET_SUCCESSFUL)
        {
            KernelPanic();
        }

        // Indicates the peripheral is initialised
        PERIPHERAL_DESC(peripheral).status = DESC_USED;
        peripheral++;
    }
}

/**
 * @fn      extern void CreatePeripheralsMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
extern void CreatePeripheralsMutexes(void)
{
    peripheralNo_t peripheral = 1u;

    // Init all peripherals
    while (IS_A_VALID_PERIPHERAL(peripheral))
    {
        // Initialise global mutex
        PERIPHERAL_DESC(peripheral).mutex = xSemaphoreCreateMutexStatic(PERIPHERAL_CONF(peripheral).p_mutex_queue);
        if (PERIPHERAL_DESC(peripheral).mutex == NULL)
        {
            KernelPanic();
        }

        // Initialise rx mutex
        PERIPHERAL_DESC(peripheral).rx.mutex = xSemaphoreCreateMutexStatic(PERIPHERAL_CONF(peripheral).p_rx_mutex_queue);
        if (PERIPHERAL_DESC(peripheral).mutex == NULL)
        {
            KernelPanic();
        }

        // Initialise tx mutex
        PERIPHERAL_DESC(peripheral).tx.mutex = xSemaphoreCreateMutexStatic(PERIPHERAL_CONF(peripheral).p_tx_mutex_queue);
        if (PERIPHERAL_DESC(peripheral).mutex == NULL)
        {
            KernelPanic();
        }

        peripheral++;
    }

    peripherals_mutexes_initialised = true;
}

/**
 * @fn          PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length)
 * @brief       Function that writes data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[in]   data        Data that will be sent to the device
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_PERIPHERAL(peripheral)) && (data != NULL))
    {
        // First lock peripheral
        PeripheralLockTX(peripheral);

        // Setup current tx owner
        PERIPHERAL_DESC(peripheral).tx.owner = GetCurrentTask();

        // Then get peripheral and type
        peripheralType_t type = PERIPHERAL_CONF(peripheral).type;

        // Then use the correct driver to write
        switch (type)
        {
            case PERIPHERAL_GPIO :
                return_value = GpioWrite((gpioInst_t *)PERIPHERAL_DESC(peripheral).p_inst, *data);
                break;
            case PERIPHERAL_UART :
                return_value = UartWrite((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            case PERIPHERAL_I2C :
                return_value = I2cWrite((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            case PERIPHERAL_SPI :
                return_value = SpiWrite((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            case PERIPHERAL_OW :
                return_value = OwWrite((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            default :
                KernelPanic();
                break;
        }

        // If the peripheral is asynchronous wait for TX complete signal from IRQ
        if (PERIPHERAL_CONF(peripheral).synchronisation == PERIPHERAL_ASYNCHRONOUS)
        {
            return_value = WaitSignal(SIGNAL_PERIPHERAL_TX_DONE);
        }

        // Reset current tx owner
        PERIPHERAL_DESC(peripheral).tx.owner = NO_TASK;

        // Unlock anyway
        PeripheralUnlockTX(peripheral);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length)
 * @brief       Function that reads data to a peripheral
 * @param[in]   peripheral  Peripheral numero
 * @param[out]  data        Data that will be received to the peripheral
 * @param[in]   length      Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or peripheral is not valid
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_PERIPHERAL(peripheral)) && (data != NULL))
    {
        // First lock peripheral
        PeripheralLockRX(peripheral);

        // Setup current rx owner
        PERIPHERAL_DESC(peripheral).rx.owner = GetCurrentTask();

        // Then get peripheral and type
        peripheralType_t type = PERIPHERAL_CONF(peripheral).type;

        // Then use the correct driver to read
        switch (type)
        {
            case PERIPHERAL_GPIO :
                return_value = GpioRead((gpioInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data);
                break;
            case PERIPHERAL_UART :
                return_value = UartRead((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            case PERIPHERAL_I2C :
                return_value = I2cRead((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            case PERIPHERAL_SPI :
                return_value = SpiRead((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            case PERIPHERAL_OW :
                return_value = OwRead((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, length);
                break;
            default :
                KernelPanic();
                break;
        }

        // If the peripheral is asynchronous wait for RX complete signal from IRQ
        if (PERIPHERAL_CONF(peripheral).synchronisation == PERIPHERAL_ASYNCHRONOUS)
        {
            return_value = WaitSignal(SIGNAL_PERIPHERAL_RX_DONE);
        }

        // Reset current rx owner
        PERIPHERAL_DESC(peripheral).rx.owner = NO_TASK;

        // Unlock anyway
        PeripheralUnlockRX(peripheral);
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
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_PERIPHERAL(peripheral))
    {
        // First get peripheral and type
        peripheralType_t type = PERIPHERAL_CONF(peripheral).type;

        // Do IOCTL depending on the command
        if (cmd == IOCTL_PERIPHERAL_START_RX)
        {
            // First lock peripheral
            PeripheralLockRX(peripheral);

            // Setup current rx owner
            PERIPHERAL_DESC(peripheral).rx.owner = GetCurrentTask();

            // Start RX IOCTL
            switch (type)
            {
                case PERIPHERAL_UART :
                    return_value = UartRead((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_I2C :
                    return_value = I2cRead((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_SPI :
                    return_value = SpiRead((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_OW :
                    return_value = OwRead((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_GPIO :
                    // Start RX not available for this peripherals
                    return_value = RET_INVALID_PARAM;
                    break;
                default :
                    KernelPanic();
                    break;
            }
        }
        else if (cmd == IOCTL_PERIPHERAL_START_TX)
        {
            // First lock peripheral
            PeripheralLockTX(peripheral);

            // Setup current tx owner
            PERIPHERAL_DESC(peripheral).tx.owner = GetCurrentTask();

            // Start TX IOCTL
            switch (type)
            {
                case PERIPHERAL_UART :
                    return_value = UartWrite((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_I2C :
                    return_value = I2cWrite((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_SPI :
                    return_value = SpiWrite((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_OW :
                    return_value = OwWrite((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, data, data_size);
                    break;
                case PERIPHERAL_GPIO :
                    // Start TX not available for this peripherals
                    return_value = RET_INVALID_PARAM;
                    break;
                default :
                    KernelPanic();
                    break;
            }
        }
        else if (cmd == IOCTL_PERIPHERAL_CHECK_RX)
        {
            // Check RX IOCTL
            switch (type)
            {
                case PERIPHERAL_UART :
                    return_value = UartIoctl((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_I2C :
                    return_value = I2cIoctl((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_SPI :
                    return_value = SpiIoctl((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_OW :
                    return_value = OwIoctl((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_GPIO :
                    // Check RX not available for this peripherals
                    return_value = RET_INVALID_PARAM;
                    break;
                default :
                    KernelPanic();
                    break;
            }

            // Reset current rx owner if reception is done and unlock reception
            if (return_value == RET_SUCCESSFUL)
            {
                PERIPHERAL_DESC(peripheral).rx.owner = NO_TASK;
                PeripheralUnlockRX(peripheral);
            }
        }
        else if (cmd == IOCTL_PERIPHERAL_CHECK_TX)
        {
            // Check TX IOCTL
            switch (type)
            {
                case PERIPHERAL_UART :
                    return_value = UartIoctl((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_I2C :
                    return_value = I2cIoctl((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_SPI :
                    return_value = SpiIoctl((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_OW :
                    return_value = OwIoctl((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_GPIO :
                    // Check TX not available for this peripherals
                    return_value = RET_INVALID_PARAM;
                    break;
                default :
                    KernelPanic();
                    break;
            }

            // Reset current tx owner if transmission is done and unlock transmission
            if (return_value == RET_SUCCESSFUL)
            {
                PERIPHERAL_DESC(peripheral).tx.owner = NO_TASK;
                PeripheralUnlockTX(peripheral);
            }
        }
        else
        {
            // First lock peripheral
            PeripheralLock(peripheral);

            // Peripheral specific IOCTL
            switch (type)
            {
                case PERIPHERAL_GPIO :
                    return_value = GpioIoctl((gpioInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_UART :
                    return_value = UartIoctl((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_I2C :
                    return_value = I2cIoctl((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_SPI :
                    return_value = SpiIoctl((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                case PERIPHERAL_OW :
                    return_value = OwIoctl((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst, cmd, data, data_size);
                    break;
                default :
                    KernelPanic();
                    break;
            }

            // Unlock anyway
            PeripheralUnlock(peripheral);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          PeripheralSetCallback(peripheralNo_t peripheral)
 * @brief       Set peripheral callback for IRQ/DMA, in order to send a signal when TX/RX is done
 * @param[in]   peripheral  Peripheral to setup
 * @retval      #RET_INVALID_PARAM if peripheral does not exist
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t PeripheralSetCallback(peripheralNo_t peripheral)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((peripheral != (peripheralNo_t)NO_PERIPHERAL) && ((peripheral) < (peripheralNo_t)CONFIG_MAX_NB_PERIPHERALS))
    {
        switch (PERIPHERAL_CONF(peripheral).type)
        {
            case PERIPHERAL_UART :
                ((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed       = PeripheralRXCallback;
                ((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed_param = &PERIPHERAL_DESC(peripheral);
                ((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed       = PeripheralTXCallback;
                ((uartInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed_param = &PERIPHERAL_DESC(peripheral);
                break;
            case PERIPHERAL_I2C :
                ((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed       = PeripheralRXCallback;
                ((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed_param = &PERIPHERAL_DESC(peripheral);
                ((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed       = PeripheralTXCallback;
                ((i2cInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed_param = &PERIPHERAL_DESC(peripheral);
                break;
            case PERIPHERAL_SPI :
                ((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed       = PeripheralRXCallback;
                ((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed_param = &PERIPHERAL_DESC(peripheral);
                ((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed       = PeripheralTXCallback;
                ((spiInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed_param = &PERIPHERAL_DESC(peripheral);
                break;
            case PERIPHERAL_OW :
                ((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed       = PeripheralRXCallback;
                ((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_rx_completed_param = &PERIPHERAL_DESC(peripheral);
                ((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed       = PeripheralTXCallback;
                ((owInst_t *)PERIPHERAL_DESC(peripheral).p_inst)->callback_tx_completed_param = &PERIPHERAL_DESC(peripheral);
                break;
            case PERIPHERAL_GPIO :
                // Do nothing
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
 * @fn          PeripheralLock(peripheralNo_t peripheral)
 * @brief       Lock the peripheral with a mutex
 * @param[in]   peripheral  Peripheral that will be locked
 * @return      Nothing
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static void PeripheralLock(peripheralNo_t peripheral)
{
    // Lock only if the mutexes has been initialised
    if ((peripherals_mutexes_initialised) && (IS_A_VALID_PERIPHERAL(peripheral)))
    {
        BaseType_t mutex_status;

        // Lock depending on the peripheral flow type
        if (PERIPHERAL_CONF(peripheral).flow_type == PERIPHERAL_FLOW_INDEPENDENT)
        {
            // If independant flow, first take global mutex to ensure coordination
            mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).mutex, portMAX_DELAY);
            if (mutex_status == pdTRUE)
            {
                // First take RX mutex
                mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).rx.mutex, portMAX_DELAY);
                if (mutex_status != pdTRUE)
                {
                    KernelPanic();
                }

                // Then take TX mutex
                mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).tx.mutex, portMAX_DELAY);
                if (mutex_status != pdTRUE)
                {
                    KernelPanic();
                }

                // Now unlock global mutex because coordination is not needed anymore
                mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).mutex);
                if (mutex_status != pdTRUE)
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
            // If coupled flow, just lock global mutex
            mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).mutex, portMAX_DELAY);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}

/**
 * @fn          PeripheralUnlock(peripheralNo_t peripheral)
 * @brief       Unlock the peripheral (which has been locked with a mutex)
 * @param[in]   peripheral  Peripheral that will be unlocked
 * @return      Nothing
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static void PeripheralUnlock(peripheralNo_t peripheral)
{
    // Unlock only if the mutexes has been initialised
    if ((peripherals_mutexes_initialised) && (IS_A_VALID_PERIPHERAL(peripheral)))
    {
        BaseType_t mutex_status;

        // Unlock depending on the peripheral flow type
        if (PERIPHERAL_CONF(peripheral).flow_type == PERIPHERAL_FLOW_INDEPENDENT)
        {
            // Global mutex is not used for unlocking in order to avoid deadlocks
            // First release TX mutex
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).rx.mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }

            // Then release RX mutex
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).tx.mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
        else
        {
            // If coupled flow, just unlock global mutex
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}

/**
 * @fn          PeripheralLockRX(peripheralNo_t peripheral)
 * @brief       Lock the peripheral reception with a mutex
 * @param[in]   peripheral  Peripheral that will be locked
 * @return      Nothing
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static void PeripheralLockRX(peripheralNo_t peripheral)
{
    // Lock only if the mutexes has been initialised
    if ((peripherals_mutexes_initialised) && (IS_A_VALID_PERIPHERAL(peripheral)))
    {
        BaseType_t mutex_status;

        // Lock depending on the peripheral flow type
        if (PERIPHERAL_CONF(peripheral).flow_type == PERIPHERAL_FLOW_INDEPENDENT)
        {
            // If independant flow, first take global mutex to ensure coordination
            mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).mutex, portMAX_DELAY);
            if (mutex_status == pdTRUE)
            {
                // Take RX mutex
                mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).rx.mutex, portMAX_DELAY);
                if (mutex_status != pdTRUE)
                {
                    KernelPanic();
                }

                // Now unlock global mutex because coordination is not needed anymore
                mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).mutex);
                if (mutex_status != pdTRUE)
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
            // If coupled flow, just lock global mutex
            mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).mutex, portMAX_DELAY);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}

/**
 * @fn          PeripheralUnlockRX(peripheralNo_t peripheral)
 * @brief       Unlock the peripheral reception (which has been locked with a mutex)
 * @param[in]   peripheral  Peripheral that will be unlocked
 * @return      Nothing
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static void PeripheralUnlockRX(peripheralNo_t peripheral)
{
    // Unlock only if the mutexes has been initialised
    if ((peripherals_mutexes_initialised) && (IS_A_VALID_PERIPHERAL(peripheral)))
    {
        BaseType_t mutex_status;

        // Unlock depending on the peripheral flow type
        if (PERIPHERAL_CONF(peripheral).flow_type == PERIPHERAL_FLOW_INDEPENDENT)
        {
            // If independant flow, release RX mutex (global mutex not used in order to avoid deadlocks)
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).rx.mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
        else
        {
            // If coupled flow, just unlock global mutex
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}

/**
 * @fn          PeripheralLockTX(peripheralNo_t peripheral)
 * @brief       Lock the peripheral transmission with a mutex
 * @param[in]   peripheral  Peripheral that will be locked
 * @return      Nothing
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static void PeripheralLockTX(peripheralNo_t peripheral)
{
    // Lock only if the mutexes has been initialised
    if ((peripherals_mutexes_initialised) && (IS_A_VALID_PERIPHERAL(peripheral)))
    {
        BaseType_t mutex_status;

        // Lock depending on the peripheral flow type
        if (PERIPHERAL_CONF(peripheral).flow_type == PERIPHERAL_FLOW_INDEPENDENT)
        {
            // If independant flow, first take global mutex to ensure coordination
            mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).mutex, portMAX_DELAY);
            if (mutex_status == pdTRUE)
            {
                // Take TX mutex
                mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).tx.mutex, portMAX_DELAY);
                if (mutex_status != pdTRUE)
                {
                    KernelPanic();
                }

                // Now unlock global mutex because coordination is not needed anymore
                mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).mutex);
                if (mutex_status != pdTRUE)
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
            // If coupled flow, just lock global mutex
            mutex_status = xSemaphoreTake(PERIPHERAL_DESC(peripheral).mutex, portMAX_DELAY);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}

/**
 * @fn          PeripheralUnlockTX(peripheralNo_t peripheral)
 * @brief       Unlock the peripheral transmission (which has been locked with a mutex)
 * @param[in]   peripheral  Peripheral that will be unlocked
 * @return      Nothing
 *
 * @warning     Cannot be used during init or ISR because of mutexes
 */
static void PeripheralUnlockTX(peripheralNo_t peripheral)
{
    // Unlock only if the mutexes has been initialised
    if ((peripherals_mutexes_initialised) && (IS_A_VALID_PERIPHERAL(peripheral)))
    {
        BaseType_t mutex_status;

        // Unlock depending on the peripheral flow type
        if (PERIPHERAL_CONF(peripheral).flow_type == PERIPHERAL_FLOW_INDEPENDENT)
        {
            // If independant flow, release TX mutex (global mutex not used in order to avoid deadlocks)
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).tx.mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
        else
        {
            // If coupled flow, just unlock global mutex
            mutex_status = xSemaphoreGive(PERIPHERAL_DESC(peripheral).mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}

/**
 * @fn      PeripheralRXCallback(void *param)
 * @brief   Peripheral generic RX completed callback
 */
static void PeripheralRXCallback(void *param)
{
    peripheralDesc_t *peripheral_desc = (peripheralDesc_t *)param;
    if (peripheral_desc->rx.owner != NO_TASK)
    {
        (void)SendSignal(peripheral_desc->rx.owner, SIGNAL_PERIPHERAL_RX_DONE);
    }
}

/**
 * @fn      PeripheralTXCallback(void *param)
 * @brief   Peripheral generic TX completed callback
 */
static void PeripheralTXCallback(void *param)
{
    peripheralDesc_t *peripheral_desc = (peripheralDesc_t *)param;
    if (peripheral_desc->tx.owner != NO_TASK)
    {
        (void)SendSignal(peripheral_desc->tx.owner, SIGNAL_PERIPHERAL_TX_DONE);
    }
}
