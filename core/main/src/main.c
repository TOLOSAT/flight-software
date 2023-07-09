/**
 * @file main.c
 * @author Merlin Kooshmanian
 * @brief Main source file
 * @date 26/12/2022
 *
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/
#include <stdio.h>
#include <cmsis_os2.h>

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#elif defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#elif defined(STM32H745xx)
#include "stm32h7xx_nucleo_bsp.h"
#else
#error "Board is not supported"
#endif
#include "main.h"
#include "initialisation.h"

/*************************** Macros Definitions ******************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      main(void)
 * @brief   Flight Software Main is the entrypoint of the code
 * @return  0
 */
int main(void)
{
    // Initialisation
    init();

    printf("[Main] Hello\n");

    // Function Core
    osKernelStart();
    while (1)
    {
        // Scheduler runs
    }

    return (0);
}

/**
 * @brief       EXTI line detection callback.
 * @param[in]   GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == USER_BUTTON_PIN) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
        printf("\nHello from interrupt !\n\n");
    }
}
