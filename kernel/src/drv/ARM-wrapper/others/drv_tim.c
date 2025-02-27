/**
 * @file    drv_tim.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC TIMER functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/others/drv_tim.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

#define HAL_TIMER_FREQ            1000u /**< 1kHz timer freq */
#define HAL_TIMER_IRQ_PRIO        15UL  /**< Priority for hal timer interrupt */
#define MONITORING_TIMER_IRQ_PRIO 3UL   /**< Priority for monitoring timer interrupt */

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef HAL_InitTick(void);
static void MonitoringTickHandler(void *param);
static void HalTickHandler(void *param);
static void HalTickCallback(DUALTIM_TimerSelTypeDef sel);

/*************************** Variables Definitions ***************************/

/**
 * @var     haltick_timer
 * @brief   Timer used by the HAL
 */
static DUALTIM_HandleTypeDef haltick_timer = { 0 };

/**
 * @var     monitoring_timer
 * @brief   Monitoring timer instance used for FreeRTOS monitoring
 */
static timerInst_t monitoring_timer;

/**
 * @var     monitoring_tick
 * @brief   Tick for freertos monitoring
 */
static volatile uint64_t monitoring_tick;

/*************************** Functions Definitions ***************************/

/********************** HAL Timer Functions Definitions **********************/

/**
 * @brief  This function configures the HAL Timer
 */
HAL_StatusTypeDef HAL_InitTick(void)
{
    HAL_StatusTypeDef hal_status = HAL_OK;

    // Setup the timer information
    haltick_timer.instance    = CMSDK_DUALTIMER;
    haltick_timer.mode_1      = DUALTIMER_PERIODIC;
    haltick_timer.size_1      = DUALTIMER_32_BITS;
    haltick_timer.prescaler_1 = DUALTIMER_PRESCALER_1;
    haltick_timer.reload_1    = (SystemCoreClock / HAL_TIMER_FREQ) - 1u;
    haltick_timer.mode_2      = DUALTIMER_DISABLED;
    haltick_timer.callback    = &HalTickCallback;

    // Init the timer
    hal_status = cmsdk_DualTimerInit(&haltick_timer);
    if (hal_status == HAL_OK)
    {
        // Request the interrupt
        returnCode_t irq_status = RequestIRQ(DUALTIMER_IRQn, HAL_TIMER_IRQ_PRIO, &HalTickHandler, NULL);
        if (irq_status == RET_SUCCESSFUL)
        {
            // Start the timer
            cmsdk_DualTimerStart(&haltick_timer, DUALTIMER_TIMER_1);
        }
        else
        {
            hal_status = HAL_ERROR;
        }
    }

    return hal_status;
}

/******************* Monitoring Timer Functions Definitions ******************/

/**
 * @brief Monitoring Timer Initialization Function
 */
returnCode_t InitMonitoringTimer(void)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Setup the timer information
    monitoring_timer.instance = CMSDK_TIMER0;
    monitoring_timer.reload   = 1000;
    monitoring_timer.mode     = TIMER_PERIODIC;
    monitoring_timer.callback = NULL;

    // Init the timer
    HAL_StatusTypeDef hal_status = cmsdk_TimerInit(&monitoring_timer);
    if (hal_status == HAL_OK)
    {
        // Request the interrupt
        return_value = RequestIRQ(TIMER0_IRQn, MONITORING_TIMER_IRQ_PRIO, &MonitoringTickHandler, NULL);
    }

    return return_value;
}

/**
 * @brief This function start Monitoring Timer
 */
void StartMonitoringTimer(void)
{
    cmsdk_TimerStart(&monitoring_timer);
}

/**
 * @brief This function get the current value of the monitoring tick
 */
uint64_t GetMonitoringTick(void)
{
    return monitoring_tick;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @brief This function is the monitoring tick timer interrupt handler
 */
static void MonitoringTickHandler(void *param)
{
    // Unused Parameter
    (void)(param);

    // Interrupt Core
    cmsdk_TimerIrqHandler(&monitoring_timer);
    monitoring_tick++;
}

/**
 * @brief This function is the HAL tick timer interrupt handler
 */
static void HalTickHandler(void *param)
{
    // Unused Parameter
    (void)(param);

    // Interrupt Core
    cmsdk_DualTimerIrqHandler(&haltick_timer);
}

/**
 * @brief Hal Tick Interrupt Callback
 */
static void HalTickCallback(DUALTIM_TimerSelTypeDef sel)
{
    if (sel == DUALTIMER_TIMER_1)
    {
        cmsdk_HalIncTick();
    }
}