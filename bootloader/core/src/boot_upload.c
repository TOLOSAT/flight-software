/**
 * @file    boot_upload.c
 * @author  Merlin Kooshmanian
 * @brief   Source file that includes function for SW upload
 * @date    27/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>
#include <elf.h>
#include <ff.h>

#include "boot_upload.h"
#include "boot_misc.h"
#include "boot_fdir.h"

/***************************** Macros Definitions ****************************/

#define FSW_FILE_PATH       "PROGRAM.ELF"   /**< Flight Software file path */
#define BUFFER_SIZE         1024u           /**< Buffer Size used for copying data */
#define ISR_VECTOR_ADDR     D1_ITCMRAM_BASE /**< ISR Vector of the flight software (not the bootloader) */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static uint32_t g_entry_point_addr = 0u;
static uint32_t g_stack_pointer_addr = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @fn      UploadSoftware(void)
 * @brief   Upload software from File System to RAM
 * @return  Nothing
 */
void UploadSoftware(void)
{
    // Variable Initialisation
    uint32_t status = 0u;
    FIL file;
    UINT bytes_read;
    Elf32_Ehdr elf_header;
    Elf32_Phdr prog_header;
    uint8_t buffer[BUFFER_SIZE];

    // Opens the ELF file.
    status = f_open(&file, FSW_FILE_PATH, FA_READ);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Reads the ELF header.
    f_read(&file, &elf_header, sizeof(elf_header), &bytes_read);

    // Check the magic number ELF.
    status = memcmp(elf_header.e_ident, ELFMAG, SELFMAG);
    CheckErrors(status, FDIR_ERROR_HANDLER);

    // Reads and processes each programme header.
    for (uint32_t i = 0u; i < elf_header.e_phnum; ++i)
    {
        f_lseek(&file, elf_header.e_phoff + (i * sizeof(prog_header)));
        f_read(&file, &prog_header, sizeof(prog_header), &bytes_read);

        // Checks whether this segment should be loaded into memory
        if (prog_header.p_type == PT_LOAD)
        {
            f_lseek(&file, prog_header.p_offset);

            // Read and copy the segment in chunks of BUFFER_SIZE bytes
            uint32_t chunk_size;
            uint32_t remaining = prog_header.p_filesz;
            uint32_t destination_addr = prog_header.p_paddr;
            while (remaining > 0u)
            {
                // Update Chunk Size according to the remaining data number
                if (remaining < BUFFER_SIZE)
                {
                    chunk_size = remaining;
                }
                else
                {
                    chunk_size = BUFFER_SIZE;
                }
                // Read the chunk in the file
                f_read(&file, buffer, chunk_size, &bytes_read);

                // Copy data
                uint8_t *destination = (uint8_t *)destination_addr; // cppcheck-suppress misra-c2012-11.4; Is one of the exception of the rule because we need to address memory 
                (void)memcpy(destination, buffer, chunk_size);

                // Update destination addresse and remaining data number
                destination_addr += chunk_size;
                remaining -= chunk_size;
            }
        }
    }

    // Close file
    f_close(&file);

    // Copy the entrypoint addresse and main stack pointer
    g_stack_pointer_addr = *((uint32_t*)ISR_VECTOR_ADDR);
    g_entry_point_addr = elf_header.e_entry;
}

/**
 * @fn      StartSoftware(void)
 * @brief   Start the newly updated software
 * @return  Nothing
 */
void StartSoftware(void)
{
    // Calls the entry point of the ELF program.
    void (*entry_point)(void) = (void (*)(void))g_entry_point_addr; // cppcheck-suppress misra-c2012-11.6; Exception because we need to cast this address as a void function

    // Set main stack pointer (MSP)
    __set_MSP(g_stack_pointer_addr);

    // Starts the loaded programme
    entry_point();
}