/**
 * @file    boot_init.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL used by boot
 * @date    25/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <ff.h>
#include <ff_gen_drv.h>

#include "boot_init.h"
#include "bootloader_misc.h"
#include "diskio.h"
#include "boot_fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void TIM3_IRQHandler(void);
extern void TIM4_IRQHandler(void);
extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static halStatus_t InitHal(void);
static halStatus_t SystemClock_Config(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     hal_tick_timer
 * @brief   Tick timer instance used for HAL delay and timing
 */
static timerInst_t hal_tick_timer;

/*************************** Functions Definitions ***************************/

/**
 * @fn      init_boot(void)
 * @brief   Function that initialise tools and HAL for boot
 */
void init_boot(void)
{
    // Variable Initialisation
    uint32_t status = 0u;
    static FATFS file_system = {0};
    static Diskio_drvTypeDef driver = {0};
    char disk_path[4] = {0};
    driver.disk_initialize = DiskInitialize;
    driver.disk_status = DiskStatus;
    driver.disk_read = DiskRead;
    driver.disk_write = DiskWrite;
    driver.disk_ioctl = DiskIoctl;

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    status = FATFS_LinkDriver(&driver, disk_path);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    status = f_mount(&file_system, "/", 1);
    CheckErrors(status, FDIR_ERROR_HANDLER);
}

/**
 * @fn      InitHal(void)
 * @brief   Function that init the choosen HAL dans sysclock
 * @retval  #GEN_HAL_ERROR if cannot init HAL or system clock
 * @retval  #GEN_HAL_SUCCESSFUL else
 *
 * If there is an error it goes to Error Handler
 */
static halStatus_t InitHal(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Function Core
    test_val = HAL_Init();
    if (test_val == HAL_OK)
    {
        return_value = SystemClock_Config();
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}

/**
 * @fn      SystemClock_Config
 * @brief   System Clock Configuration
 */
static halStatus_t SystemClock_Config(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Function Core
    /** Initializes the RCC Oscillator Powers */
    CLOCK_SPECIFIC_INIT_PWR();

    /** Initializes the RCC Oscillators */
    CLOCK_SPECIFIC_INIT_OSC(RCC_OscInitStruct);

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
    {
        /** Initializes the CPU, AHB and APB buses clocks */
        CLOCK_SPECIFIC_INIT_BUS(RCC_ClkInitStruct);
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}

/**
 * @brief  This function configures the TIM4 as a time base source.
 *         The time source is configured  to have 1ms time base with a dedicated
 *         Tick interrupt priority.
 * @note   This function is called  automatically at the beginning of program after
 *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
 * @param  TickPriority: Tick interrupt priority.
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock = 0U;
    uint32_t uwAPB1Prescaler = 0U;

    uint32_t uwPrescalerValue = 0U;
    uint32_t pFLatency;
    HAL_StatusTypeDef status;

    /* Enable TIM4 clock */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    /* Compute TIM4 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
    {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    }
    else
    {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM4 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM4 */
    hal_tick_timer.Instance = TIM4;

    /* Initialize TIMx peripheral as follow:

    + Period = [(TIM4CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    hal_tick_timer.Init.Period = (1000000U / 1000U) - 1U;
    hal_tick_timer.Init.Prescaler = uwPrescalerValue;
    hal_tick_timer.Init.ClockDivision = 0;
    hal_tick_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    hal_tick_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    status = HAL_TIM_Base_Init(&hal_tick_timer);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&hal_tick_timer);
        if (status == HAL_OK)
        {
            /* Enable the TIM4 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM4_IRQn);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority(TIM4_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            }
            else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}

/**
 * @brief  Suspend Tick increment.
 * @note   Disable the tick increment by disabling timer hal update interrupt.
 */
void HAL_SuspendTick(void)
{
    /* Disable timer HAL update Interrupt */
    __HAL_TIM_DISABLE_IT(&hal_tick_timer, TIM_IT_UPDATE);
}

/**
 * @brief  Resume Tick increment.
 * @note   Enable the tick increment by Enabling timer hal update interrupt.
 */
void HAL_ResumeTick(void)
{
    /* Enable TIM HAL Update interrupt */
    __HAL_TIM_ENABLE_IT(&hal_tick_timer, TIM_IT_UPDATE);
}

/**
 * @brief This function handles TIM4 trigger and commutation interrupts and TIM4 global interrupt.
 */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&hal_tick_timer);
}

/**
 * @brief HAL Timer(s) Callback Function
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        HAL_IncTick();
    }
}