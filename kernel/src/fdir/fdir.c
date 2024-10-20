/**
 * @file    fdir.c
 * @author  Merlin Kooshmanian
 * @brief   Error Management functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "fdir/fdir.h"
#include "core/tasks.h"
#include "system/console.h"
#include "system/sysleds.h"
#include "utils/log.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void Reset_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          CheckError(returnCode_t retcode)
 * @brief       This function check if an error occured and execute the sanction
 * @param[in]   retcode     Return code of a function.
 * @return      Nothing
 */
void CheckError(returnCode_t retcode)
{
    if (retcode == RET_ERROR)
    {
        // Indicates an error occured and system goes into error handler
        LOG("System : KO\n");

        // Go to error handler
        ErrorHandler();
    }
    else
    {
        // No errors
    }
}

/**
 * @fn      ErrorHandler(void)
 * @brief   This function is executed in case of error occurrence.
 * @warning Real FDIR has to be done.
 * 
 * Normally this function has to save some context (what happened before error). 
 * But currently no real fdir has been done. For debugging purposes, Error handler 
 * is just a while loop that hangs processor indefinitly.
 */
void ErrorHandler(void)
{
    __disable_irq();
    LEDErrorOn();
    while (1)
    {
        // Do nothing
    }
}

/*************************** Interruption Handlers ***************************/

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}
