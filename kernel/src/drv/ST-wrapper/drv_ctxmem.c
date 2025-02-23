/**
 * @file    drv_ctxmem.c
 * @author  Théo Bessel
 * @brief   Source file for QSPI disk driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/drv_ctxmem.h"
#include <string.h>

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_CTX_MEM_QSPI_NAND)
extern uint32_t __qspi_start__;

static uint32_t *qspi_ptr = &__qspi_start__; /**< Pointer to the QSPI memory */
#endif                                       /* CONFIG_CTX_MEM_QSPI_NAND */

/*************************** Functions Definitions ***************************/

/**
 * @fn          CtxMemRead(uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
 * @brief       Function that reads the context memory
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   offset  Offset in the data that will be read
 * @param[in]   len     Number of block that will be read
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t CtxMemRead(uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

#if defined(CONFIG_CTX_MEM_QSPI_NAND)
    // Check parameter(s)
    if ((len != 0u) && (data != NULL))
    {
        (void)memcpy(data, (void *)&qspi_ptr[addr + offset], len);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }
#else
    (void)data;
    (void)addr;
    (void)offset;
    (void)len;
#endif /* CONFIG_CTX_MEM_QSPI_NAND */

    return return_value;
}

/**
 * @fn          CtxMemWrite(const uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
 * @brief       Function that writes into the context memory
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   offset  Offset in the data that will be written
 * @param[in]   len     Length of the data that will be written
 * @retval      #RET_INVALID_PARAM if len equal zero or pointer is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t CtxMemWrite(const uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

#if defined(CONFIG_CTX_MEM_QSPI_NAND)
    // Check parameter(s)
    if ((len != 0u) && (data != NULL))
    {
        (void)memcpy((void *)&qspi_ptr[addr + offset], data, len);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }
#else
    (void)data;
    (void)addr;
    (void)offset;
    (void)len;
#endif /* CONFIG_CTX_MEM_QSPI_NAND */

    return return_value;
}
