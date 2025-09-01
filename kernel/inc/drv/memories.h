/**
 * @file    memories.h
 * @author  Théo Bessel
 * @author  Merlin Kooshmanian
 * @brief   Header file for memory driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup memories Memories Drivers
 * @brief Abstraction layer for managing memories.
 * @{
 */

#ifndef MEMORIES_H
#define MEMORIES_H

/******************************* Include Files *******************************/

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     MEMORY_CONF(memory_no)
 * @brief   Get memory conf from g_memories_conf_table
 */
#define MEMORY_CONF(memory_no) (g_memories_conf_table[(memory_no) - 1u])

/**
 * @def     MEMORY_DESC(memory_no)
 * @brief   Get memory conf from g_memories_desc_table
 */
#define MEMORY_DESC(memory_no) (g_memories_desc_table[(memory_no) - 1u])

/**
 * @def     IS_A_VALID_MEMORY(memory_no)
 * @brief   Indicates if the memory_no is valid
 */
#define IS_A_VALID_MEMORY(memory_no) \
    (((memory_no) != (memoryNo_t)NO_MEMORY) && ((memory_no) < (memoryNo_t)CONFIG_MAX_NB_MEMORIES) && (MEMORY_DESC(memory_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/** @brief Memory reference number type */
typedef uint32_t memoryNo_t;

/**
 * @enum    memoryType_t
 * @brief   memory type typedef enum
 */
typedef enum
{
    MEMORY_RAM       = 0u, /**< RAM type memory */
    MEMORY_SD        = 1u, /**< SD over SDIO type memory */
    MEMORY_SPISD     = 2u, /**< SD over SPI type memory */
    MEMORY_QSPIFLASH = 3u, /**< QSPI Flash type memory */
    MEMORY_NAND      = 4u, /**< FMC NAND type memory */
} memoryType_t;

/**
 * @enum    memoryClass_t
 * @brief   Memory classification for the API.
 */
typedef enum
{
    MEMORY_CLASS_STORAGE = 0u, /**< Filesystem-backed, user-visible mass storage */
    MEMORY_CLASS_CONTEXT = 1u, /**< Kernel-only non-volatile context */
} memoryClass_t;

/**
 * @struct  memoryConf_t
 * @brief   Struct type of a memory configuration
 */
typedef struct
{
    memoryNo_t memory;   /**< @brief memory reference number */
    memoryType_t type;   /**< @brief memory type (RAM, SD, QSPI, ...) */
    memoryClass_t class; /**< @brief memory class (storage vs. context) */
    const void *p_conf;  /**< @brief Pointer to the memory configuration */
} memoryConf_t;

/**
 * @struct  memoryDesc_t
 * @brief   Struct type of a memory descriptors
 */
typedef struct
{
    descStatus_t status; /**< @brief Indicates if the descriptor is free or used */
    void *p_inst;        /**< @brief Pointer to the memory instance */
} memoryDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_memories_conf_table
 * @brief   Configuration table where all memories configurations are stored
 */
extern const memoryConf_t g_memories_conf_table[CONFIG_MAX_NB_MEMORIES];

/**
 * @var     g_memories_desc_table
 * @brief   Configuration table where all memories descriptors are stored
 */
extern memoryDesc_t g_memories_desc_table[CONFIG_MAX_NB_MEMORIES];

/*************************** Functions Declarations **************************/

extern void InitMemories(void);
extern returnCode_t MemoryWrite(memoryNo_t memory, uint32_t sector, data_t data, length_t length);
extern returnCode_t MemoryRead(memoryNo_t memory, uint32_t sector, data_t data, length_t length);
extern returnCode_t MemoryIoctl(memoryNo_t memory, uint32_t cmd, void *data, uint32_t data_size);

#endif /* MEMORIES_H */

/**
 * @}
 * @}
 * @}
 */