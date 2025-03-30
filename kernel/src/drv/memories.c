/**
 * @file    memories.c
 * @author  Théo Bessel
 * @brief   Source file for memory driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"
#include "fdir/fdir.h"

#if !defined(CONFIG_MEMORY_NONE)
#if defined(CONFIG_MEMORY_QSPI_NOR)
#include "drv/memory/memdrv_qspi.h"
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
 */
void MemoryInit(void)
{
#if defined(CONFIG_MEMORY_NONE)
    // Do Nothing
#else
#if defined(CONFIG_MEMORY_QSPI_NOR)
    returnCode_t return_value = QSPI_MemoryInit();
    if (return_value != RET_SUCCESSFUL)
    {
        KernelPanic();
    }
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif
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
#if defined(CONFIG_MEMORY_QSPI_NOR)
    return QSPI_MemoryRead(data, addr, len);
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif
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
#if defined(CONFIG_MEMORY_QSPI_NOR)
    return QSPI_MemoryWrite(data, addr, len);
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif
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
#if defined(CONFIG_MEMORY_QSPI_NOR)
    return QSPI_MemoryErase(addr, len);
#else
#error Please #define CONFIG_MEMORY_QSPI_NOR or CONFIG_MEMORY_NONE
#endif
#endif /* CONFIG_MEMORY_NONE */
}