/**
 * @file    boot_init.c
 * @author  Merlin Kooshmanian
 * @brief   Source file initialising tools and HAL used by boot
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <ff.h>
#include <ff_gen_drv.h>

#include "boot_init.h"
#include "boot_fdir.h"
#include "file-system/drv_disk.h"
#include "memory/memdrv_qspi.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static coreStatus_t InitHal(void);
static coreStatus_t DeInitHal(void);
static coreStatus_t InitLeds(void);
static coreStatus_t InitUserBtn(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      BootInit(void)
 * @brief   Function that initialise tools and HAL for boot
 * @return  Nothing
 */
void BootInit(void)
{
    uint32_t status                 = 0u;
    static FATFS file_system        = { 0 };
    static Diskio_drvTypeDef driver = { 0 };
    char disk_path[4]               = { 0 };

    // HAL Initialisation
    status = InitHal();
    if (status != 0u)
    {
        ErrorHandler();
    }

    // LEDs initialisation
    status = InitLeds();
    if (status != 0u)
    {
        ErrorHandler();
    }

    // User button initialisation
    status = InitUserBtn();
    if (status != 0u)
    {
        ErrorHandler();
    }

    // Link drivers for FATFS
    driver.disk_initialize = DiskInitialize;
    driver.disk_status     = DiskStatus;
    driver.disk_read       = DiskRead;
    driver.disk_write      = DiskWrite;
    driver.disk_ioctl      = DiskIoctl;
    status                 = FATFS_LinkDriver(&driver, disk_path);
    if (status != 0u)
    {
        ErrorHandler();
    }

    // Mount the SD card
    status = f_mount(&file_system, "/", 1);
    if (status != 0u)
    {
        ErrorHandler();
    }

    // Init QSPI memory
    status = QSPI_MemoryInit();
    if (status != 0u)
    {
        ErrorHandler();
    }
}

/**
 * @fn      BootDeInit(void)
 * @brief   Function that disinitialise boot software
 * @return  Nothing
 */
void BootDeInit(void)
{
    uint32_t status = 0u;

    // Turn off blue LED
    HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_SET);

    // Unmount SD card
    f_unmount("/");

    // Deinit HAL
    status = DeInitHal();
    if (status != 0u)
    {
        ErrorHandler();
    }
}

/**
 * @fn      InitHal(void)
 * @brief   Function that initialises the HAL
 * @retval  #CORE_ERROR if cannot init HAL or system clock
 * @retval  #CORE_SUCCESSFUL else
 */
static coreStatus_t InitHal(void)
{
    coreStatus_t return_value = CORE_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Init HAL
    test_val = HAL_Init();
    if (test_val == HAL_OK)
    {
        returnCode_t test_bsp = SystemClock_Config();
        if (test_bsp != RET_SUCCESSFUL)
        {
            return_value = CORE_ERROR;
        }
    }
    else
    {
        return_value = CORE_ERROR;
    }

    return return_value;
}

/**
 * @fn      DeInitHal(void)
 * @brief   Function that deinitialises the HAL
 * @retval  #CORE_ERROR if cannot deinit HAL
 * @retval  #CORE_SUCCESSFUL else
 */
static coreStatus_t DeInitHal(void)
{
    coreStatus_t return_value = CORE_SUCCESSFUL;
    HAL_StatusTypeDef test_val;

    // Desinit everything
    HAL_SuspendTick();
    test_val = HAL_RCC_DeInit();
    if (test_val == HAL_OK)
    {
        test_val = HAL_DeInit();
        if (test_val != HAL_OK)
        {
            return_value = CORE_ERROR;
        }
    }
    else
    {
        return_value = CORE_ERROR;
    }

    return return_value;
}

/**
 * @fn      InitLeds(void)
 * @brief   GPIO Initialization Function
 * @retval  #CORE_SUCCESSFUL always
 */
static coreStatus_t InitLeds(void)
{
    coreStatus_t return_value        = CORE_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    // GPIO Ports Clock Enable
    LED_STATUS_CLK_ENABLE();
    LED_ERROR_CLK_ENABLE();

    // Configure GPIO pin Output Level
    HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_SET);

    // Configure GPIO pin Output Level
    HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_RESET);

    // Configure GPIO pin : RED LED
    GPIO_InitStruct.Pin   = LED_ERROR_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_ERROR_PORT, &GPIO_InitStruct);

    // Configure GPIO pin : BLUE LED
    GPIO_InitStruct.Pin   = LED_STATUS_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_STATUS_PORT, &GPIO_InitStruct);

    return return_value;
}

/**
 * @fn      InitUserBtn(void)
 * @brief   GPIO Initialization Function
 * @retval  #CORE_SUCCESSFUL always
 */
static coreStatus_t InitUserBtn(void)
{
    coreStatus_t return_value        = CORE_SUCCESSFUL;
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    // GPIO Ports Clock Enable
    USER_BUTTON_CLK_ENABLE();

    // Configure GPIO pin : USER BTN
    GPIO_InitStruct.Pin   = USER_BUTTON_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(USER_BUTTON_PORT, &GPIO_InitStruct);

    return return_value;
}