/**
 * @file errors_mgmt.c
 * @author Merlin Kooshmanian
 * @brief Error Management functions
 * @date 05/05/2023
 *
 * Last Update : 05/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <cmsis_os2.h>

#include "errors_mgmt.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      CheckErrors(uint32_t status, errorsSanction_t sanction)
 * @brief   This function check if an error occured and execute the sanction
 * @param   status Return value of a function.
 * @param   sanction The sanction that has to be performed in order to solve the problem
 * @retval  None
 */
void CheckErrors(uint32_t status, errorsSanction_t sanction)
{
    if(status != 0)
    {
        if(sanction == ERROR_HANDLER)
        {
            Error_Handler();
        }
        else if (sanction == RESET_HANDLER)
        {
            Reset_Handler();
        }
        else
        {
            /* No Sanction needed */
        }
    }
    else
    {
        /* No errors */
    }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        // Do nothing
    }
}