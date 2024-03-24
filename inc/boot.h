/**
 * @file    boot.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for bootloader
 * @date    24/03/2024
 * 
 * @copyright Copyright (c) TOLOSAT
 */

#ifndef BOOT_H
#define BOOT_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @struct  elfFileHeader_t
 * @brief   Struct type of a elf file header
 */
typedef struct {
    uint8_t  magic[4];      /**< @brief Magic Number (must be {0x7f, 'E', 'L', 'F') */
    uint8_t  format;        /**< @brief This byte is set to either 1 or 2 to signify 32- or 64-bit format, respectively. */
    uint8_t  data;          /**< @brief This byte is set to either 1 or 2 to signify little or big endianness, respectively. */
    uint8_t  version;       /**< @brief Set to 1 for the original and current version of ELF. */
    uint8_t  osabi;         /**< @brief Identifies the target operating system ABI. */
    uint8_t  abiversion;    /**< @brief Further specifies the ABI version. */
    uint8_t  padding[7];    /**< @brief Reserved padding bytes. */
    uint16_t type;          /**< @brief Identifies object file type. */
    uint16_t machine;       /**< @brief Specifies target instruction set architecture. */
    uint32_t version2;      /**< @brief Set to 1 for the original version of ELF. */
    uint32_t entry;         /**< @brief This is the memory address of the entry point from where the process starts executing. */
    uint32_t phoff;         /**< @brief Points to the start of the program header table. */
    uint32_t shoff;         /**< @brief Points to the start of the section header table. */
    uint32_t flags;         /**< @brief Interpretation of this field depends on the target architecture. */
    uint16_t ehsize;        /**< @brief Contains the size of this header, normally 64 Bytes for 64-bit and 52 Bytes for 32-bit format. */
    uint16_t phentsize;     /**< @brief Contains the size of a program header table entry. */
    uint16_t phnum;         /**< @brief Contains the number of entries in the program header table. */
    uint16_t shentsize;     /**< @brief Contains the size of a section header table entry. */
    uint16_t shnum;         /**< @brief Contains the number of entries in the section header table. */
    uint16_t shstrndx;      /**< @brief Contains index of the section header table entry that contains the section names. */
} elfFileHeader_t;

/** 
 * @struct  elfProgramHeader
 * @brief   Struct type of a elf program header
 */
typedef struct {
    uint32_t type;      /**< @brief Identifies the type of the segment. */
    uint32_t offset;    /**< @brief Offset of the segment in the file image. */
    uint32_t vaddr;     /**< @brief Virtual address of the segment in memory. */
    uint32_t paddr;     /**< @brief On systems where physical address is relevant, reserved for segment's physical address. */
    uint32_t filesz;    /**< @brief Size in bytes of the segment in the file image. May be 0. */
    uint32_t memsz;     /**< @brief Size in bytes of the segment in memory. May be 0. */
    uint32_t flags;     /**< @brief Segment-dependent flags. */
    uint32_t align;     /**< @brief Segment alignment */
} elfProgramHeader;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

int main(void);

#endif /* BOOT_H */