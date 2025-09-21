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

#include "memories/drv_ram.h"
#if defined(CONFIG_HAS_SD_MEMORY)
#include "memories/drv_sd.h"
#endif
#if defined(CONFIG_HAS_SPISD_MEMORY)
#include "memories/drv_spisd.h"
#endif
#if defined(CONFIG_HAS_QSPI_NOR_MEMORY)
#include "memories/drv_qspinor.h"
#endif
#if defined(CONFIG_HAS_QSPI_MRAM_MEMORY)
#include "memories/drv_qspimram.h"
#endif
#if defined(CONFIG_HAS_FMC_NAND_MEMORY)
#include "memories/drv_nand.h"
#endif

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

/** @brief Number of sector count type (used for type size checking) */
typedef uint32_t memorySectorCount_t;

/** @brief Number of sector size type (used for type size checking) */
typedef uint16_t memorySectorSize_t;

/** @brief Number of block size type (used for type size checking) */
typedef uint32_t memoryBlockSize_t;

/**
 * @enum    memoryStatus_t
 * @brief   memory status typedef enum
 */
typedef enum
{
    MEMORY_READY   = 0u, /**< Memory ready to be used */
    MEMORY_NO_INIT = 1u, /**< Memory not initialised */
    MEMORY_NO_DISK = 2u, /**< Memory disconnected*/
} memoryStatus_t;

/**
 * @enum    memoryType_t
 * @brief   memory type typedef enum
 */
typedef enum
{
    MEMORY_RAM      = 0u, /**< RAM type memory */
    MEMORY_SD       = 1u, /**< SD over SDIO type memory */
    MEMORY_SPISD    = 2u, /**< SD over SPI type memory */
    MEMORY_QSPINOR  = 3u, /**< QSPI NOR flash type memory */
    MEMORY_NAND     = 4u, /**< FMC NAND type memory */
    MEMORY_QSPIMRAM = 5u, /**< QSPI MRAM flash type memory */
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
 * @var     g_fs_mem
 * @brief   File system memory definition
 */
extern const memoryNo_t g_fs_mem;

/**
 * @var     g_context_mem
 * @brief   Context memory definition
 */
extern const memoryNo_t g_context_mem;

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
extern returnCode_t MemoryWrite(memoryNo_t memory, memorySector_t sector, data_t data, length_t length);
extern returnCode_t MemoryRead(memoryNo_t memory, memorySector_t sector, data_t data, length_t length);
extern returnCode_t MemoryIoctl(memoryNo_t memory, uint32_t cmd, void *data, uint32_t data_size);

#endif /* MEMORIES_H */

/**
 * @}
 * @}
 * @}
 */