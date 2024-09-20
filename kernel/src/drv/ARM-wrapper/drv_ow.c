/**
 * @file    drv_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for OW functions
 * @date    11/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_ow.h"

/***************************** Macros Definitions ****************************/

#define OW_RESET_PULSE_DURATION         480u    /**< Amount of time the line need to be pulled down to initialise One Wire connection */
#define OW_PRESENCE_WAIT_DURATION       70u     /**< Amount of time we need to wait until the slave will pull the line down */
#define OW_PRESENCE_PULSE_DURATION      410u    /**< Amount of time we need to wait until the slave will release the line */
#define OW_WRITE_1_PULL_DOWN_TIME_US    10u     /**< Amount of time the line needed to be pulled down to write 1 for One Wire */
#define OW_WRITE_1_PULL_UP_TIME_US      55u     /**< Amount of time the line needed to be pulled up to write 1 for One Wire */
#define OW_WRITE_0_PULL_DOWN_TIME_US    60u     /**< Amount of time the line needed to be pulled down to write 0 for One Wire */
#define OW_WRITE_0_PULL_UP_TIME_US      5u      /**< Amount of time the line needed to be pulled up to write 0 for One Wire */
#define OW_READ_PULL_DOWN_TIME_US       3u      /**< Amount of time the line needed to be pulled down to read on One Wire */
#define OW_READ_WAIT_ANSWER_TIME_US     10u     /**< Amount of time the line needed to wait before reading on One Wire */
#define OW_READ_COMPLETE_TIME_US        52u     /**< Amount of time the line need to be pulled up to complete the read on One Wire */

#define MICROSECONDS_PER_SECOND     1000000u    /**< Number of microseconds per second */

/*************************** Functions Declarations **************************/

static kernelStatus_t OwWriteByte(owInst_t *ow_inst, uint8_t byte);
static kernelStatus_t OwReadByte(owInst_t *ow_inst, uint8_t *byte);
static kernelStatus_t OwInitConnection(owInst_t *ow_inst);
static kernelStatus_t OwWriteBit(owInst_t *ow_inst, uint8_t bit);
static kernelStatus_t OwReadBit(owInst_t *ow_inst, uint8_t *bit);
static kernelStatus_t OwTimerInit(owInst_t *ow_inst);
static void OwDelayUs(owInst_t *ow_inst, uint32_t delay_us);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_inst)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #KERNEL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #KERNEL_ERROR if an error occured
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION OwOpen(owInst_t *ow_inst)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        return_value = GpioOpen(&ow_inst->gpio_inst);
        (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
        if (return_value == KERNEL_SUCCESSFUL)
        {
            return_value = OwTimerInit(ow_inst);
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   msg Message to write
 * @param[in]   length Number of byte to write
 * @retval      #KERNEL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #KERNEL_ERROR if an error occured when using GPIO
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((ow_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == KERNEL_SUCCESSFUL) && (i < length))
        {
            return_value = OwWriteByte(ow_inst, msg[i]);
            i++;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[out]  msg Message read
 * @param[in]   length Number of byte to read
 * @retval      #KERNEL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #KERNEL_ERROR if an error occured when using GPIO
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((ow_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == KERNEL_SUCCESSFUL) && (i < length))
        {
            return_value = OwReadByte(ow_inst, &msg[i]);
            i++;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_inst Instance that contains One Wire parameters handlers
 * @param[in]       cmd IO Control command
 * @param[in,out]   data IO Control command
 * @param[in]       data_size IO Control data size
 * @retval          #KERNEL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    // Unused
    (void)(data);
    (void)(data_size);

    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        switch (cmd)
        {
        case OW_IOCTL_INIT_CONNECTION:
            return_value = OwInitConnection(ow_inst);
            break;
        default:
            return_value = KERNEL_INVALID_PARAM;
            break;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwClose(owInst_t *ow_inst)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #KERNEL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #KERNEL_ERROR if an error occured
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION OwClose(owInst_t *ow_inst)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        return_value = GpioClose(&ow_inst->gpio_inst);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWriteByte(owInst_t *ow_inst, uint8_t byte)
 * @brief       Function that writes a byte onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   byte Byte to write
 * @retval      #KERNEL_INVALID_PARAM if there is a null pointer
 * @retval      #KERNEL_ERROR if an error occured when using GPIO
 * @retval      #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION OwWriteByte(owInst_t *ow_inst, uint8_t byte)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        uint32_t i = 0u;
        while ((return_value == KERNEL_SUCCESSFUL) && (i < 8u))
        {
            uint8_t bit = (uint8_t)((byte & (1u << i)) >> i);
            return_value = OwWriteBit(ow_inst, bit);
            i++;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwReadByte(owInst_t *ow_inst, uint8_t byte)
 * @brief       Function that reads a byte onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   byte Byte to read
 * @retval      #KERNEL_INVALID_PARAM if there is a null pointer
 * @retval      #KERNEL_ERROR if an error occured when using GPIO
 * @retval      #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION OwReadByte(owInst_t *ow_inst, uint8_t *byte)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        uint32_t i = 0u;
        while ((return_value == KERNEL_SUCCESSFUL) && (i < 8u))
        {
            uint8_t bit = 0u;
            return_value = OwReadBit(ow_inst, &bit);
            *byte |= bit << i;
            i++;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwInitConnection(owInst_t *ow_inst)
 * @brief           Function that initialize a One Wire connection
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #KERNEL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #KERNEL_BUSY line is busy, somebody is pulling the line low
 * @retval          #KERNEL_BUSY if nobody has answered the master after a reset pulse
 * @retval          #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION OwInitConnection(owInst_t *ow_inst)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

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
            OwDelayUs(ow_inst, OW_PRESENCE_PULSE_DURATION);

            // Check if slave has answered
            if (line_state != GPIO_PIN_RESET)
            {
                return_value = KERNEL_TIMEOUT;
            }
        }
        else
        {
            return_value = KERNEL_BUSY;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWriteBit(owInst_t *ow_inst, uint8_t bit)
 * @brief       Function that writes a bit onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   bit Bit to write
 * @retval      #KERNEL_INVALID_PARAM if there is a null pointer
 * @retval      #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION OwWriteBit(owInst_t *ow_inst, uint8_t bit)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

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
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwReadBit(owInst_t *ow_inst, uint8_t *bit)
 * @brief       Function that reads a bit onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   bit Bit to read
 * @retval      #KERNEL_INVALID_PARAM if there is a null pointer
 * @retval      #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION OwReadBit(owInst_t *ow_inst, uint8_t *bit)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        gpioValue_t line_state = GPIO_PIN_RESET;
        (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_RESET);
        OwDelayUs(ow_inst, OW_READ_PULL_DOWN_TIME_US); // Short delay
        (void)GpioWrite(&ow_inst->gpio_inst, GPIO_PIN_SET);
        OwDelayUs(ow_inst, OW_READ_WAIT_ANSWER_TIME_US); // Wait for the device to respond
        (void)GpioRead(&ow_inst->gpio_inst, &line_state);
        OwDelayUs(ow_inst, OW_READ_COMPLETE_TIME_US); // Wait to complete 60us period

        *bit = (uint8_t)line_state;
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwTimerInit(owInst_t *ow_inst)
 * @brief           Function that initialises the One Wire timer
 * @param[in,out]   ow_inst 
 * @retval          #KERNEL_ERROR if timer has encountered an error at init
 * @retval          #KERNEL_INVALID_PARAM if there is a null pointer
 * @retval          #KERNEL_SUCCESSFUL else 
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION OwTimerInit(owInst_t *ow_inst) 
{
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    if (ow_inst != NULL)
    {
        ow_inst->timer.instance = CMSDK_TIMER1;
        ow_inst->timer.mode = TIMER_ONESHOT;
        ow_inst->timer.reload = 0u; // For the moment dont care because OwDelayUs will set the reload 
        cmsdk_TimerInit(&ow_inst->timer);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }
    
    return return_value;
}

/**
 * @fn          OwDelayUs(owInst_t *ow_inst, uint32_t delay_us)
 * @brief       Just a delay with active waiting
 * @param[in]   ow_inst 
 * @param[in]   delay_us 
 */
static void IN_KERNEL_TEXT_SECTION OwDelayUs(owInst_t *ow_inst, uint32_t delay_us)
{
    if (ow_inst != NULL)
    {
        uint32_t counter_value = (delay_us * SystemCoreClock) / MICROSECONDS_PER_SECOND;
        __HAL_TIM_SET_COUNTER(ow_inst->timer, counter_value);
        cmsdk_TimerStart(&ow_inst->timer);
        while (counter_value > 1u)
        {
            counter_value = __HAL_TIM_GET_COUNTER(ow_inst->timer);
        }
        cmsdk_TimerStop(&ow_inst->timer);
    }
}