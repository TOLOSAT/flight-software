/**
 * @file    memories.c
 * @author  Théo Bessel
 * @brief   Source file for memory driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"

#if !defined(CONFIG_MEMORY_NONE)
#if !defined(CONFIG_MEMORY_QSPI_NOR)
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif
#endif

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          MemoryInit(void)
 * @brief       Function that initializes the memory
 * @retval      #RET_SUCCESSFUL if the memory has been initialized
 * @retval      #RET_ERROR if the memory has not been initialized
 */
returnCode_t MemoryInit(void)
{
#if defined(CONFIG_MEMORY_NONE)
    return RET_SUCCESSFUL;
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif /* CONFIG_MEMORY_NONE */
}

/**
 * @fn          MemoryRead(uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads from the memory
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   len     Length of the data that will be read
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t MemoryRead(uint8_t *data, uint32_t addr, uint32_t len)
{
#if defined(CONFIG_MEMORY_NONE)
    (void)data;
    (void)addr;
    (void)len;
    return RET_SUCCESSFUL;
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif /* CONFIG_MEMORY_NONE */
}

/**
 * @fn          MemoryWrite(const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes into the memory
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   len     Length of the data that will be written
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t MemoryWrite(const uint8_t *data, uint32_t addr, uint32_t len)
{
#if defined(CONFIG_MEMORY_NONE)
    (void)data;
    (void)addr;
    (void)len;
    return RET_SUCCESSFUL;
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif /* CONFIG_MEMORY_NONE */
}

/**
 * @fn          MemoryErase(uint32_t addr, uint32_t len)
 * @brief       Function that erases the memory
 * @param[in]   addr    Address of the data that will be erased
 * @param[in]   len     Length of the data that will be erased
 * @retval      #RET_INVALID_PARAM if len equal zero
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t MemoryErase(uint32_t addr, uint32_t len)
{
#if defined(CONFIG_MEMORY_NONE)
    (void)addr;
    (void)len;
    return RET_SUCCESSFUL;
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif /* CONFIG_MEMORY_NONE */
}