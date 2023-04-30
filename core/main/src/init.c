/**
 * @file init.c
 * @author Merlin Kooshmanian
 * @brief Source file initialising tools and HAL
 * @date 21/01/2023
 *
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <cmsis_os2.h>

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#endif
#if defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#endif

#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

static void I2C1_Init(void);

void EnableUserButtonIt(void);
extern void Error_Handler(void);
extern void initialise_monitor_handles(UART_HandleTypeDef *huart);

/************************** Variable Definitions *****************************/

I2C_HandleTypeDef hi2c1;

uartInst_t uart2_inst = {
    .drive_type = UART_POLLING_DRIVE,
    .uart_ref = USART2,
    .baud_rate = 115200,
};
gpioInst_t led2_inst = {
    .mode = GPIO_MODE_OUTPUT_PP, 
    .pull = GPIO_NOPULL, 
    .speed = GPIO_SPEED_FREQ_LOW
};
gpioInst_t user_button_inst = {
    .mode = GPIO_MODE_IT_FALLING, 
    .pull = GPIO_NOPULL, 
    .speed = GPIO_SPEED_FREQ_LOW
};

/************************* Functions Definitions *****************************/

/**
 * @fn      init(void)
 * @brief   Init tools and HAL
 * @param   void
 * @return  0
 * 
 * Error management needs to be improved
 */
uint32_t init(void)
{
    // HAL Initialisation
    InitHal();

    // GPIOs Initialisation
    GpioOpen(&led2_inst, LED2_GPIO_PORT, LED2_PIN);
    GpioOpen(&user_button_inst, USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN);
    EnableUserButtonIt();

    // UARTs Initialisation
    UartOpen(&uart2_inst);

    // I2Cs Initialisation
    I2C1_Init();
    
    // Monitor Initialisation
    initialise_monitor_handles(&uart2_inst.handle_struct);

    // OS Kernel Initialisation
    osKernelInitialize();

    return (0);
}

/**
 * @brief Enable Interruption for user button
 * @param None
 * @retval None
 */
void EnableUserButtonIt(void)
{
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void I2C1_Init(void)
{
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }
}