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
#include "boot_misc.h"
#include "boot_fdir.h"
#include "file-system/user_diskio.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t InitHal(void);
static halStatus_t InitLeds(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      BootInit(void)
 * @brief   Function that initialise tools and HAL for boot
 * @return  Nothing
 */
void BootInit(void)
{
    // Variable Initialisation
    uint32_t status = 0u;
    static FATFS file_system = {0};
    static Diskio_drvTypeDef driver = {0};
    char disk_path[4] = {0};

    // HAL Initialisation
    status = InitHal();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // LEDs initialisation
    status = InitLeds();
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Link drivers for FATFS
    driver.disk_initialize = DiskInitialize;
    driver.disk_status = DiskStatus;
    driver.disk_read = DiskRead;
    driver.disk_write = DiskWrite;
    driver.disk_ioctl = DiskIoctl;
    status = FATFS_LinkDriver(&driver, disk_path);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Mount the SD card
    status = f_mount(&file_system, "/", 1);
    CheckErrors(status, FDIR_ERROR_HANDLER);
}

/**
 * @fn      BootDeInit(void)
 * @brief   Function that disinitialise boot software
 * @return  Nothing
 */
void BootDeInit(void)
{
    // Turn off blue LED
    HAL_GPIO_WritePin(BLUE_LED_GPIO_PORT, BLUE_LED_PIN, GPIO_PIN_SET);

    // Unmount SD card
    f_unmount("/");

    // Deinit HAL
    HAL_SuspendTick();
    HAL_RCC_DeInit();
    HAL_DeInit();
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
        // Init Clock
        RCC_OscInitTypeDef RCC_OscInitStruct = {0};
        RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

        // Initializes the RCC Oscillator Powers
        CLOCK_SPECIFIC_INIT_PWR();

        // Initializes the RCC Oscillators
        CLOCK_SPECIFIC_INIT_OSC(RCC_OscInitStruct);

        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK)
        {
            // Initializes the CPU, AHB and APB buses clocks
            CLOCK_SPECIFIC_INIT_BUS(RCC_ClkInitStruct);
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_ERROR;
    }

    return return_value;
}

/**
 * @fn      InitLeds(void)
 * @brief   GPIO Initialization Function
 * @retval  #GEN_HAL_SUCCESSFUL always
 */
static halStatus_t InitLeds(void)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Function Core
    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    // Configure GPIO pin Output Level
    HAL_GPIO_WritePin(RED_LED_GPIO_PORT, RED_LED_PIN, GPIO_PIN_SET);

    // Configure GPIO pin Output Level
    HAL_GPIO_WritePin(BLUE_LED_GPIO_PORT, BLUE_LED_PIN, GPIO_PIN_RESET);

    // Configure GPIO pin : RED LED
    GPIO_InitStruct.Pin = RED_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RED_LED_GPIO_PORT, &GPIO_InitStruct);

    // Configure GPIO pin : BLUE LED
    GPIO_InitStruct.Pin = BLUE_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BLUE_LED_GPIO_PORT, &GPIO_InitStruct);

    return return_value;
}
