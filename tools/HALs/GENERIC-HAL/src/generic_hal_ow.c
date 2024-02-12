/**
 * @file    generic_hal_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL OW functions
 * @date    11/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define OW_RESET_PULSE_DURATION         480u    /**< Amount of time the line need to be pulled down to initialise One Wire connection */
#define OW_PRESENCE_WAIT_DURATION       70u     /**< Amount of time we need to wait until the slave will pull the line down */
#define OW_PRESENCE_PULSE_DURATION      410u    /**< Amount of time we need to wait until the slave will release the line */
#define OW_WRITE_1_PULL_DOWN_TIME_US    10u     /**< Amount of time the line need to be pulled down to write 1 for One Wire */
#define OW_WRITE_1_PULL_UP_TIME_US      55u     /**< Amount of time the line need to be pulled up to write 1 for One Wire */
#define OW_WRITE_0_PULL_DOWN_TIME_US    65u     /**< Amount of time the line need to be pulled down to write 0 for One Wire */
#define OW_WRITE_0_PULL_UP_TIME_US      5u      /**< Amount of time the line need to be pulled up to write 0 for One Wire */

/*************************** Functions Declarations **************************/

static halStatus_t OwWriteByte(owInst_t *ow_inst, uint8_t byte);
static halStatus_t OwReadByte(owInst_t *ow_inst, uint8_t *byte);
static halStatus_t OwInitConnection(owInst_t *ow_inst);
static halStatus_t OwWriteBit(owInst_t *ow_inst, uint8_t bit);
static halStatus_t OwReadBit(owInst_t *ow_inst, uint8_t *bit);
static halStatus_t OwTimerInit(owInst_t *ow_inst);
static void OwDelayUs(owInst_t *ow_inst, uint32_t delay_us);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_inst)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #GEN_HAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #GEN_HAL_ERROR if an error occured
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
halStatus_t OwOpen(owInst_t *ow_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        return_value = GpioOpen(&ow_inst->gpio_inst);
        (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
        if (return_value == GEN_HAL_SUCCESSFUL)
        {
            return_value = OwTimerInit(ow_inst);
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   msg Message to write
 * @param[in]   length Number of byte to write
 * @retval      #GEN_HAL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #GEN_HAL_ERROR if an error occured when using GPIO
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
halStatus_t OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((ow_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == GEN_HAL_SUCCESSFUL) && (i < length))
        {
            return_value = OwWriteByte(ow_inst, msg[i]);
            i++;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[out]  msg Message read
 * @param[in]   length Number of byte to read
 * @retval      #GEN_HAL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #GEN_HAL_ERROR if an error occured when using GPIO
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
halStatus_t OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((ow_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == GEN_HAL_SUCCESSFUL) && (i < length))
        {
            return_value = OwReadByte(ow_inst, &msg[i]);
            i++;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwIoctl(owInst_t *ow_inst, halIoCtlCmd_t io_cmd)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_inst Instance that contains One Wire parameters handlers
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
halStatus_t OwIoctl(owInst_t *ow_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        switch (io_cmd.cmd)
        {
        case OW_IOCTL_INIT_CONNECTION:
            return_value = OwInitConnection(ow_inst);
            break;
        default:
            return_value = GEN_HAL_INVALID_PARAM;
            break;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwClose(owInst_t *ow_inst)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #GEN_HAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #GEN_HAL_ERROR if an error occured
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
halStatus_t OwClose(owInst_t *ow_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        return_value = GpioClose(&ow_inst->gpio_inst);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWriteByte(owInst_t *ow_inst, uint8_t byte)
 * @brief       Function that writes a byte onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   byte Byte to write
 * @retval      #GEN_HAL_INVALID_PARAM if there is a null pointer
 * @retval      #GEN_HAL_ERROR if an error occured when using GPIO
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t OwWriteByte(owInst_t *ow_inst, uint8_t byte)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        uint32_t i = 0u;
        while ((return_value == GEN_HAL_SUCCESSFUL) && (i < 8u))
        {
            uint8_t bit = (uint8_t)((byte & (1u << i)) >> i);
            return_value = OwWriteBit(ow_inst, bit);
            i++;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwReadByte(owInst_t *ow_inst, uint8_t byte)
 * @brief       Function that reads a byte onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   byte Byte to read
 * @retval      #GEN_HAL_INVALID_PARAM if there is a null pointer
 * @retval      #GEN_HAL_ERROR if an error occured when using GPIO
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t OwReadByte(owInst_t *ow_inst, uint8_t *byte)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        uint32_t i = 0u;
        while ((return_value == GEN_HAL_SUCCESSFUL) && (i < 8u))
        {
            uint8_t bit = 0u;
            return_value = OwReadBit(ow_inst, &bit);
            *byte |= bit << i;
            i++;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwInitConnection(owInst_t *ow_inst)
 * @brief           Function that initialize a One Wire connection
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #GEN_HAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #GEN_HAL_BUSY line is busy, somebody is pulling the line low
 * @retval          #GEN_HAL_BUSY if nobody has answered the master after a reset pulse
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t OwInitConnection(owInst_t *ow_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        // First check the line is idle (pulled up)
        gpioValue_t line_state = GPIO_PIN_RESET;
        (void)GpioRead(&ow_inst->gpio_inst, &line_state);
        if (line_state == GPIO_PIN_SET)
        {
            // First pulling the line down as a "reset pulse"
            (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_RESET);
            OwDelayUs(ow_inst, OW_RESET_PULSE_DURATION);

            // Then release the line and wait for the slave to answer
            (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
            OwDelayUs(ow_inst, OW_PRESENCE_WAIT_DURATION);

            // Then read the line
            (void)GpioRead(&ow_inst->gpio_inst, &line_state);
            OwDelayUs(ow_inst, OW_PRESENCE_WAIT_DURATION);

            // Check if slave has answered
            if (line_state != GPIO_PIN_RESET)
            {
                return_value = GEN_HAL_TIMEOUT;
            }
        }
        else
        {
            return_value = GEN_HAL_BUSY;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWriteBit(owInst_t *ow_inst, uint8_t bit)
 * @brief       Function that writes a bit onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   bit Bit to write
 * @retval      #GEN_HAL_INVALID_PARAM if there is a null pointer
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t OwWriteBit(owInst_t *ow_inst, uint8_t bit)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        if ((bit & 0x01u) == 0x01u)
        {
            // Write '1'
            (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_RESET);
            OwDelayUs(ow_inst, OW_WRITE_1_PULL_DOWN_TIME_US); // Delay for '1'
            (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
            OwDelayUs(ow_inst, OW_WRITE_1_PULL_UP_TIME_US); // Delay to complete the time slot
        }
        else
        {
            // Write '0'
            (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_RESET);
            OwDelayUs(ow_inst, OW_WRITE_0_PULL_DOWN_TIME_US); // Delay for '0'
            (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
            OwDelayUs(ow_inst, OW_WRITE_0_PULL_UP_TIME_US); // Delay to complete the time slot
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwReadBit(owInst_t *ow_inst, uint8_t *bit)
 * @brief       Function that reads a bit onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   bit Bit to read
 * @retval      #GEN_HAL_INVALID_PARAM if there is a null pointer
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t OwReadBit(owInst_t *ow_inst, uint8_t *bit)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        gpioValue_t line_state = GPIO_PIN_RESET;
        (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_RESET);
        OwDelayUs(ow_inst, 3); // Short delay
        (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
        OwDelayUs(ow_inst, 10); // Wait for the device to respond
        (void)GpioRead(&ow_inst->gpio_inst, &line_state);
        OwDelayUs(ow_inst, 45); // Wait to complete 60us period

        *bit = (uint8_t)line_state;
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwTimerInit(owInst_t *ow_inst)
 * @brief           Function that initialises the One Wire timer
 * @param[in,out]   ow_inst 
 * @retval          #GEN_HAL_ERROR if timer has encountered an error at init
 * @retval          #GEN_HAL_INVALID_PARAM if there is a null pointer
 * @retval          #GEN_HAL_SUCCESSFUL else 
 */
static halStatus_t OwTimerInit(owInst_t *ow_inst) 
{
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    if (ow_inst != NULL)
    {
        __HAL_RCC_TIM5_CLK_ENABLE(); // Activez l'horloge du timer 5
        ow_inst->timer.Instance = TIM5;
        ow_inst->timer.Init.Prescaler = (uint32_t)((SystemCoreClock) / 1000000) - 1u; // 1 MHz Counter Clock
        ow_inst->timer.Init.CounterMode = TIM_COUNTERMODE_UP;
        ow_inst->timer.Init.Period = 0xFFFF; // Max period
        ow_inst->timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        ow_inst->timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        HAL_StatusTypeDef test_val = HAL_OK;
        test_val = HAL_TIM_Base_Init(&ow_inst->timer);
        if (test_val == HAL_OK)
        {
            test_val = HAL_TIM_Base_Start(&ow_inst->timer); // Start the timer
            if (test_val != HAL_OK)
            {
                return_value = GEN_HAL_ERROR;
            }
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }
    
    return return_value;
}

/**
 * @fn          OwDelayUs(owInst_t *ow_inst, uint32_t delay_us)
 * @brief       Just a delay with active waiting
 * @param[in]   ow_inst 
 * @param[in]   delay_us 
 */
static void OwDelayUs(owInst_t *ow_inst, uint32_t delay_us)
{
    if (ow_inst != NULL)
    {
        uint32_t counter_value = 0u;
        __HAL_TIM_SET_COUNTER(&ow_inst->timer, 0); // Set the counter value to 0
        while (counter_value < delay_us)
        {
            counter_value = __HAL_TIM_GET_COUNTER(&ow_inst->timer);
        }
    }
}