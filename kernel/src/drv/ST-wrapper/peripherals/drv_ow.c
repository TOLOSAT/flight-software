/**
 * @file    drv_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for OW functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_ow.h"
#include "fdir/fdir.h"

#include "core/os.h" // TO DO : do better with IRQs

/***************************** Macros Definitions ****************************/

#define OW_RESET_PULSE_DURATION      480u /**< Amount of time the line need to be pulled down to initialise One Wire connection */
#define OW_PRESENCE_WAIT_DURATION    70u  /**< Amount of time we need to wait until the slave will pull the line down */
#define OW_PRESENCE_PULSE_DURATION   410u /**< Amount of time we need to wait until the slave will release the line */
#define OW_WRITE_1_PULL_DOWN_TIME_US 5u   /**< Amount of time the line needed to be pulled down to write 1 for One Wire */
#define OW_WRITE_1_PULL_UP_TIME_US   65u  /**< Amount of time the line needed to be pulled up to write 1 for One Wire */
#define OW_WRITE_0_PULL_DOWN_TIME_US 65u  /**< Amount of time the line needed to be pulled down to write 0 for One Wire */
#define OW_WRITE_0_PULL_UP_TIME_US   5u   /**< Amount of time the line needed to be pulled up to write 0 for One Wire */
#define OW_READ_PULL_DOWN_TIME_US    5u   /**< Amount of time the line needed to be pulled down to read on One Wire */
#define OW_READ_WAIT_ANSWER_TIME_US  15u  /**< Amount of time the line needed to wait before reading on One Wire */
#define OW_READ_COMPLETE_TIME_US     50u  /**< Amount of time the line need to be pulled up to complete the read on One Wire */

/**
 * @def     OW_SET_TIMER(OW_INSTANCE, DURATION)
 * @brief   Preprocessor function to set the OneWire timer
 */
#define OW_SET_TIMER(OW_INSTANCE, DURATION)                               \
    do                                                                    \
    {                                                                     \
        __HAL_TIM_SET_AUTORELOAD(&(OW_INSTANCE)->timer, (DURATION) - 1u); \
        (OW_INSTANCE)->timer.Instance->EGR = TIM_EGR_UG;                  \
    } while (0)

/**
 * @def     OW_RESET_TIMER(OW_INSTANCE)
 * @brief   Preprocessor function to reset the OneWire timer
 */
#define OW_RESET_TIMER(OW_INSTANCE)                           \
    do                                                        \
    {                                                         \
        __HAL_TIM_SET_AUTORELOAD(&(OW_INSTANCE)->timer, -1u); \
        (OW_INSTANCE)->timer.Instance->EGR = TIM_EGR_UG;      \
    } while (0)

/*************************** Functions Declarations **************************/

static void OwGenericIRQHandler(void *param);

static returnCode_t OwInitConnection(owInst_t *ow_inst);
static returnCode_t OwCheckRXTX(owInst_t *ow_inst);
static returnCode_t OwStartOperation(owInst_t *ow_inst, owOp_t operation, data_t data, length_t length);
static returnCode_t OwTimerInit(owInst_t *ow_inst, const owConf_t *const ow_conf);

static void OWIRQHandler(owInst_t *ow_inst);
static void OWIRQPullDown(owInst_t *ow_inst);
static void OWIRQPullUp(owInst_t *ow_inst);
static void OWIRQRead(owInst_t *ow_inst);
static void OWIRQCompleteBit(owInst_t *ow_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_inst, const owConf_t *constow_conf)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_inst   Instance that contains OW handlers
 * @param[in]       ow_conf   Configuration that contains OW parameters
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_INVALID_PARAM if driving_mode is DMA
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t OwOpen(owInst_t *ow_inst, const owConf_t *const ow_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((ow_inst != NULL) && (ow_conf != NULL) && (ow_conf->default_mode != DMA_MODE))
    {
        // Init OW timer
        return_value = OwTimerInit(ow_inst, ow_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Init OW GPIO
            gpioConf_t gpio_conf = {
                .port   = ow_conf->gpio_port,
                .pin    = ow_conf->gpio_pin,
                .inout  = GPIO_MODE_OUTPUT_OD,
                .pull   = GPIO_NOPULL,
                .speed  = GPIO_SPEED_FREQ_MEDIUM,
                .irq_no = IRQ_NONE,
            };
            return_value = GpioOpen(&ow_inst->gpio, &gpio_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Set current mode
                ow_inst->current_mode = ow_conf->default_mode;
                // Setup pin state
                if (GpioWrite(&ow_inst->gpio, GPIO_PIN_SET) != RET_SUCCESSFUL)
                {
                    KernelPanic();
                }
                ow_inst->state = OW_STATE_READY;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWrite(owInst_t *ow_inst, data_t data, length_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   data    Message to write
 * @param[in]   length  Number of byte to write
 * @retval      #RET_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #RET_TIMEOUT if the transaction timeouted before completion
 * @retval      #RET_NOT_AVAILABLE if no device is answering
 * @retval      #RET_NOT_AVAILABLE if the ow is already busy
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t OwWrite(owInst_t *ow_inst, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((ow_inst != NULL) && (data != NULL) && (length != 0u))
    {
        return_value = OwStartOperation(ow_inst, OW_OP_TX, data, length);
        // Poll the OW until write complete if OW is in POLLING_MODE
        if ((return_value == RET_SUCCESSFUL) && (ow_inst->current_mode == POLLING_MODE))
        {
            uint32_t tickstart = HAL_GetTick();
            while ((ow_inst->state != OW_STATE_READY) && (ow_inst->state != OW_STATE_ERROR) && ((HAL_GetTick() - tickstart) < DRV_MAX_DELAY))
            {
                __NOP();
            }

            // Depending on the state
            if ((HAL_GetTick() - tickstart) >= DRV_MAX_DELAY)
            {
                return_value = RET_TIMEOUT;
            }
            if (ow_inst->presence == false)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            if (ow_inst->state == OW_STATE_ERROR)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwRead(owInst_t *ow_inst, data_t data, length_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[out]  data    Message read
 * @param[in]   length  Number of byte to read
 * @retval      #RET_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #RET_TIMEOUT if the transaction timeouted before completion
 * @retval      #RET_NOT_AVAILABLE if no device is answering
 * @retval      #RET_NOT_AVAILABLE if the ow is already busy
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t OwRead(owInst_t *ow_inst, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((ow_inst != NULL) && (data != NULL) && (length != 0u))
    {
        return_value = OwStartOperation(ow_inst, OW_OP_RX, data, length);
        // Poll the OW until read complete if OW is in POLLING_MODE
        if ((return_value == RET_SUCCESSFUL) && (ow_inst->current_mode == POLLING_MODE))
        {
            uint32_t tickstart = HAL_GetTick();
            while ((ow_inst->state != OW_STATE_READY) && (ow_inst->state != OW_STATE_ERROR) && ((HAL_GetTick() - tickstart) < DRV_MAX_DELAY))
            {
                __NOP();
            }

            // Depending on the state
            if ((HAL_GetTick() - tickstart) >= DRV_MAX_DELAY)
            {
                return_value = RET_TIMEOUT;
            }
            if (ow_inst->presence == false)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            if (ow_inst->state == OW_STATE_ERROR)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_inst     Instance that contains One Wire parameters handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(data);
    (void)(data_size);

    // Check parameter(s)
    if (ow_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_OW_INIT_CONNECTION :
                return_value = OwInitConnection(ow_inst);
                break;
            case IOCTL_PERIPHERAL_CHECK_RX :
            case IOCTL_PERIPHERAL_CHECK_TX :
                return_value = OwCheckRXTX(ow_inst);
                break;
            case IOCTL_PERIPHERAL_STOP_RXTX :
            case IOCTL_PERIPHERAL_STOP_RX :
            case IOCTL_PERIPHERAL_STOP_TX :
                // TO DO : Add a function to stop RXTX with OneWire
                return_value = RET_INVALID_PARAM;
                break;
            default :
                return_value = RET_INVALID_PARAM;
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
 * @fn              OwClose(owInst_t *ow_inst)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t OwClose(owInst_t *ow_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (ow_inst != NULL)
    {
        return_value = GpioClose(&ow_inst->gpio);
        if (return_value == RET_SUCCESSFUL)
        {
            HAL_StatusTypeDef test_val = HAL_TIM_Base_DeInit(&ow_inst->timer);
            if (test_val != HAL_OK)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwInitConnection(owInst_t *ow_inst)
 * @brief           Function that initialize a One Wire connection
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_NOT_AVAILABLE line is busy, somebody is pulling the line low
 * @retval          #RET_NOT_AVAILABLE if nobody has answered the master after a reset pulse
 * @retval          #RET_NOT_AVAILABLE if the ow is already busy
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t OwInitConnection(owInst_t *ow_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (ow_inst != NULL)
    {
        return_value = OwStartOperation(ow_inst, OW_OP_INIT_CO, NULL, 0u);
        // Poll the OW until initialisation is complete (INTERRUPT_MODE is not available for IOCTL)
        if (return_value == RET_SUCCESSFUL)
        {
            uint32_t tickstart = HAL_GetTick();
            while ((ow_inst->state != OW_STATE_READY) && (ow_inst->state != OW_STATE_ERROR) && ((HAL_GetTick() - tickstart) < DRV_MAX_DELAY))
            {
                __NOP();
            }

            // Depending on the state
            if ((HAL_GetTick() - tickstart) >= DRV_MAX_DELAY)
            {
                return_value = RET_TIMEOUT;
            }
            if (ow_inst->presence == false)
            {
                return_value = RET_NOT_AVAILABLE;
            }
            if (ow_inst->state == OW_STATE_ERROR)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwCheckRXTX(owInst_t *ow_inst, void *data)
 * @brief           Function that checks the status of a OW reception and transmission
 * @param[in,out]   ow_inst   Instance that contains OW parameters and OW Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_TIMEOUT if the transaction timeouted before completion
 * @retval          #RET_NOT_AVAILABLE if no device is answering
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t OwCheckRXTX(owInst_t *ow_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (ow_inst != NULL)
    {
        if (ow_inst->state == OW_STATE_READY)
        {
            if (ow_inst->presence == true)
            {
                return_value = RET_SUCCESSFUL;
            }
            else
            {
                return_value = RET_NOT_AVAILABLE;
            }
        }
        else if ((ow_inst->state == OW_STATE_BUSY_RX) || (ow_inst->state == OW_STATE_BUSY_TX) || (ow_inst->state == OW_STATE_BUSY_INIT_CO))
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwTimerInit(owInst_t *ow_inst, const owConf_t *const ow_conf)
 * @brief           Function that initialises the One Wire timer
 * @param[in,out]   ow_inst   Instance that contains OW handlers
 * @param[in]       ow_conf   Configuration that contains OW parameters
 * @retval          #RET_INVALID_PARAM if there is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t OwTimerInit(owInst_t *ow_inst, const owConf_t *const ow_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameters
    if ((ow_inst != NULL) && (ow_conf != NULL))
    {
        // Retrieve clock configuration
        RCC_ClkInitTypeDef clkconfig = { 0 };
        uint32_t latency             = 0;
        HAL_RCC_GetClockConfig(&clkconfig, &latency);

        // Determine whether the timer is on APB1 or APB2
        uint32_t timer_addr = (uint32_t)ow_conf->timer_ref;
        uint32_t timer_clock;
        uint32_t prescaler_div;

        if ((timer_addr >= APB1PERIPH_BASE) && (timer_addr < (APB1PERIPH_BASE + 0x00010000UL)))
        {
            // Timer is on APB1
            prescaler_div = clkconfig.APB1CLKDivider;
            uint32_t pclk = HAL_RCC_GetPCLK1Freq();
            timer_clock   = (prescaler_div == RCC_HCLK_DIV1) ? pclk : (2U * pclk);
        }
        else if ((timer_addr >= APB2PERIPH_BASE) && (timer_addr < (APB2PERIPH_BASE + 0x00010000UL)))
        {
            // Timer is on APB2
            prescaler_div = clkconfig.APB2CLKDivider;
            uint32_t pclk = HAL_RCC_GetPCLK2Freq();
            timer_clock   = (prescaler_div == RCC_HCLK_DIV1) ? pclk : (2U * pclk);
        }
        else
        {
            // Timer does not belong to APB1 or APB2
            timer_clock = 0u;
        }

        // If timer_clock it means that smth went wrong
        if (timer_clock != 0u)
        {
            // Compute the prescaler value to have timer counter clock equal to 1MHz (1us period)
            uint32_t ow_timer_prescaler = (uint32_t)((timer_clock / 1000000U) - 1U);

            // Set the timer
            ow_inst->timer.Instance               = ow_conf->timer_ref;
            ow_inst->timer.Init.Prescaler         = ow_timer_prescaler;
            ow_inst->timer.Init.CounterMode       = TIM_COUNTERMODE_UP;
            ow_inst->timer.Init.Period            = -1u; // Max period
            ow_inst->timer.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
            ow_inst->timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

            // Then init the timer peripheral
            HAL_StatusTypeDef test_val = HAL_TIM_Base_Init(&ow_inst->timer);
            if (test_val == HAL_OK)
            {
                // Set OW inst as the interrupt parameter to pass it to the interrupt routine
                IRQHandlerParam_t param = (IRQHandlerParam_t)ow_inst;
                // Request the interrupt
                return_value = RequestIRQ(ow_conf->irq_no, 5u, OwGenericIRQHandler, param);
            }
            else
            {
                KernelPanic();
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OWIRQHandler(owInst_t *ow_inst)
 * @brief           OW IRQ Handler
 * @param[in,out]   ow_inst     Instance that contains One Wire parameters handlers
 * @param[in]       operation   Operation to perform (init co, rx, tx)
 * @param[out]      data        Operation pointer to data
 * @param[in]       length      Operation data len
 * @retval          #RET_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #RET_INVALID_PARAM if operation does not exists
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t OwStartOperation(owInst_t *ow_inst, owOp_t operation, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef status;

    // Check parameters
    if (ow_inst != NULL)
    {
        if (ow_inst->state == OW_STATE_READY)
        {
            switch (operation)
            {
                case OW_OP_INIT_CO :
                    // Update state and operation
                    ow_inst->state      = OW_STATE_BUSY_INIT_CO;
                    ow_inst->current_op = OW_OP_INIT_CO;
                    // Triggers the first interrupt
                    __HAL_TIM_SET_COUNTER(&ow_inst->timer, -1u);
                    status = HAL_TIM_Base_Start_IT(&ow_inst->timer);
                    if (status != HAL_OK)
                    {
                        KernelPanic();
                    }
                    break;
                case OW_OP_TX :
                    // Check inputs
                    if ((data != NULL) && (length != 0u))
                    {
                        // Update state and operation
                        ow_inst->state      = OW_STATE_BUSY_TX;
                        ow_inst->current_op = OW_OP_TX;
                        // Update data
                        ow_inst->p_op_data = data;
                        ow_inst->op_len    = length;
                        // Triggers the first interrupt
                        __HAL_TIM_SET_COUNTER(&ow_inst->timer, -1u);
                        status = HAL_TIM_Base_Start_IT(&ow_inst->timer);
                        if (status != HAL_OK)
                        {
                            KernelPanic();
                        }
                    }
                    else
                    {
                        return_value = RET_INVALID_PARAM;
                    }
                    break;
                case OW_OP_RX :
                    // Check inputs
                    if ((data != NULL) && (length != 0u))
                    {
                        // Update state and operation
                        ow_inst->state      = OW_STATE_BUSY_RX;
                        ow_inst->current_op = OW_OP_RX;
                        // Update data
                        ow_inst->p_op_data = data;
                        ow_inst->op_len    = length;
                        // Triggers the first interrupt
                        __HAL_TIM_SET_COUNTER(&ow_inst->timer, -1u);
                        status = HAL_TIM_Base_Start_IT(&ow_inst->timer);
                        if (status != HAL_OK)
                        {
                            KernelPanic();
                        }
                    }
                    else
                    {
                        return_value = RET_INVALID_PARAM;
                    }
                    break;
                default :
                    return_value = RET_INVALID_PARAM;
                    break;
            }
        }
        else if ((ow_inst->state == OW_STATE_BUSY_RX) || (ow_inst->state == OW_STATE_BUSY_TX) || (ow_inst->state == OW_STATE_BUSY_INIT_CO))
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn      OWIRQHandler(owInst_t *ow_inst)
 * @brief   OW IRQ Handler
 * @param   ow_inst Instance that contains One Wire parameters handlers
 */
static void OWIRQHandler(owInst_t *ow_inst)
{
    // Get previous operation state
    owOpState_t previous_state = ow_inst->op_state;
    // Depending on the previous operation state
    switch (previous_state)
    {
        case OW_OP_STATE_RESET :
            OWIRQPullDown(ow_inst);
            break;
        case OW_OP_STATE_PULL_DOWN :
            OWIRQPullUp(ow_inst);
            break;
        case OW_OP_STATE_PULL_UP :
            if (ow_inst->current_op == OW_OP_TX)
            {
                OWIRQCompleteBit(ow_inst);
            }
            else
            {
                OWIRQRead(ow_inst);
            }
            break;
        case OW_OP_STATE_READ :
            OWIRQCompleteBit(ow_inst);
            break;
        default :
            KernelPanic();
            break;
    }

    // Then handles the IRQ and stops it
    HAL_TIM_IRQHandler(&ow_inst->timer);
}

/**
 * @fn      OWIRQPullDown(owInst_t *ow_inst)
 * @brief   IRQ pull down step procedure
 */
static void OWIRQPullDown(owInst_t *ow_inst)
{
    // Update state
    ow_inst->op_state = OW_OP_STATE_PULL_DOWN;

    // Pull down the line
    if (GpioWrite(&ow_inst->gpio, GPIO_PIN_RESET) != RET_SUCCESSFUL)
    {
        KernelPanic();
    }

    // Then wait depending on the operation
    if (ow_inst->current_op == OW_OP_RX)
    {
        // Set the counter value to OW_READ_PULL_DOWN_TIME_US
        OW_SET_TIMER(ow_inst, OW_READ_PULL_DOWN_TIME_US);
    }
    else if (ow_inst->current_op == OW_OP_TX)
    {
        // Depending on the bit to write
        uint32_t bit_mask = 1u << ow_inst->op_bit_index;
        if ((ow_inst->p_op_data[ow_inst->op_index] & bit_mask) == bit_mask)
        {
            // Bit equals to 1
            // Set the counter value to OW_WRITE_1_PULL_DOWN_TIME_US
            OW_SET_TIMER(ow_inst, OW_WRITE_1_PULL_DOWN_TIME_US);
        }
        else
        {
            // Bit equals to 0
            // Set the counter value to OW_WRITE_0_PULL_DOWN_TIME_US
            OW_SET_TIMER(ow_inst, OW_WRITE_0_PULL_DOWN_TIME_US);
        }
    }
    else // OW_OP_INIT_CO
    {
        // Set the counter value to OW_RESET_PULSE_DURATION
        OW_SET_TIMER(ow_inst, OW_RESET_PULSE_DURATION);
    }
}

/**
 * @fn      OWIRQPullUp(owInst_t *ow_inst)
 * @brief   IRQ pull up step procedure
 */
static void OWIRQPullUp(owInst_t *ow_inst)
{
    // Update state
    ow_inst->op_state = OW_OP_STATE_PULL_UP;

    // Pull up the line
    if (GpioWrite(&ow_inst->gpio, GPIO_PIN_SET) != RET_SUCCESSFUL)
    {
        KernelPanic();
    }

    // Then wait depending on the operation
    if (ow_inst->current_op == OW_OP_INIT_CO)
    {
        // Set the counter value to OW_PRESENCE_WAIT_DURATION
        OW_SET_TIMER(ow_inst, OW_PRESENCE_WAIT_DURATION);
    }
    else if (ow_inst->current_op == OW_OP_RX)
    {
        // Set the counter value to OW_READ_WAIT_ANSWER_TIME_US
        OW_SET_TIMER(ow_inst, OW_READ_WAIT_ANSWER_TIME_US);
    }
    else if (ow_inst->current_op == OW_OP_TX)
    {
        // Depending on the bit to write
        uint32_t bit_mask = 1u << ow_inst->op_bit_index;
        if ((ow_inst->p_op_data[ow_inst->op_index] & bit_mask) == bit_mask)
        {
            // Bit equals to 1
            // Set the counter value to OW_WRITE_1_PULL_UP_TIME_US
            OW_SET_TIMER(ow_inst, OW_WRITE_1_PULL_UP_TIME_US);
        }
        else
        {
            // Bit equals to 0
            // Set the counter value to OW_WRITE_0_PULL_UP_TIME_US
            OW_SET_TIMER(ow_inst, OW_WRITE_0_PULL_UP_TIME_US);
        }
    }
    else
    {
        KernelPanic();
    }
}

/**
 * @fn      OWIRQRead(owInst_t *ow_inst)
 * @brief   IRQ read step procedure
 */
static void OWIRQRead(owInst_t *ow_inst)
{
    // Update state
    ow_inst->op_state = OW_OP_STATE_READ;

    // Read data
    gpioValue_t line_state = GPIO_PIN_RESET;
    if (GpioRead(&ow_inst->gpio, &line_state) != RET_SUCCESSFUL)
    {
        KernelPanic();
    }

    // Then wait depending on the operation
    if (ow_inst->current_op == OW_OP_RX)
    {
        // Update data
        ow_inst->p_op_data[ow_inst->op_index] |= (uint8_t)line_state << ow_inst->op_bit_index;
        // Set the counter value to OW_READ_COMPLETE_TIME_US
        OW_SET_TIMER(ow_inst, OW_READ_COMPLETE_TIME_US);
    }
    else // OW_OP_INIT_CO
    {
        // Update presence
        ow_inst->presence = (line_state == GPIO_PIN_RESET) ? true : false;
        // Set the counter value to OW_PRESENCE_PULSE_DURATION
        OW_SET_TIMER(ow_inst, OW_PRESENCE_PULSE_DURATION);
    }
}

/**
 * @fn      OWIRQCompleteBit(owInst_t *ow_inst)
 * @brief   IRQ bit completion step procedure
 */
static void OWIRQCompleteBit(owInst_t *ow_inst)
{
    // Depending on the operation
    if ((ow_inst->current_op == OW_OP_RX) || (ow_inst->current_op == OW_OP_TX))
    {
        // Updates op bit index
        ow_inst->op_bit_index++;
        if (ow_inst->op_bit_index == 8u)
        {
            // It means all bits have been written
            ow_inst->op_bit_index = 0u;
            // Update data index
            ow_inst->op_index++;
            if (ow_inst->op_index == ow_inst->op_len)
            {
                // All bytes have been written, operation complete
                ow_inst->op_state = OW_OP_STATE_RESET;
                // Reset timer
                OW_RESET_TIMER(ow_inst);
                // Reset the operation
                ow_inst->op_index     = 0u;
                ow_inst->op_bit_index = 0u;
                ow_inst->current_op   = OW_NO_OP;
                ow_inst->state        = OW_STATE_READY;
                // Stop timer
                HAL_TIM_Base_Stop_IT(&ow_inst->timer);
            }
            else
            {
                // New byte
                OWIRQPullDown(ow_inst);
            }
        }
        else
        {
            // New bit
            OWIRQPullDown(ow_inst);
        }
    }
    else
    {
        // Init completed, operation complete
        ow_inst->op_state = OW_OP_STATE_RESET;
        // Reset timer
        OW_RESET_TIMER(ow_inst);
        // Reset the operation
        ow_inst->current_op = OW_NO_OP;
        ow_inst->state      = OW_STATE_READY;
        // Stop timer
        HAL_TIM_Base_Stop_IT(&ow_inst->timer);
    }
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn      OwGenericIRQHandler(void *param)
 * @brief   Generic OW IRQ Handler
 */
static void OwGenericIRQHandler(void *param)
{
    // Get ow inst
    owInst_t *ow_inst = (owInst_t *)param;

    // Save pre-interrupt status
    owState_t old_status = ow_inst->state;

    // Do IRQ
    OWIRQHandler(ow_inst);

    // Check if action has completed
    if ((ow_inst->state == OW_STATE_READY))
    {
        if (old_status == OW_STATE_BUSY_RX)
        {
            // RX completed
            if (ow_inst->callback_rx_completed != NULL)
            {
                ow_inst->callback_rx_completed(ow_inst->callback_rx_completed_param);
            }
        }
        if (old_status == OW_STATE_BUSY_TX)
        {
            // TX completed
            if (ow_inst->callback_tx_completed != NULL)
            {
                ow_inst->callback_tx_completed(ow_inst->callback_tx_completed_param);
            }
        }
    }
}