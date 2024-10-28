/**
 * @file    cache.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for CACHE functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "system/cache.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn     InitCache(void)
 * @brief  Function that initialises cache memories if available
 */
void InitCache(void)
{
#if defined(CONFIG_CACHE)
    // Enable Instruction Cache
    SCB_EnableICache();

    // Enable Data Cache
    SCB_EnableDCache();
    
#if !defined(CONFIG_MPU)
    // Force the cache policy to be 'write through'
    SCB->CACR |= SCB_CACR_FORCEWT_Msk;
#endif
#endif
}