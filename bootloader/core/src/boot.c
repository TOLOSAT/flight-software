/**
 * @file    boot.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for bootloader
 * @date    24/03/2024
 *
 * @copyright Copyright (c) TOLOSAT
 */

/******************************* Include Files *******************************/

#include <string.h>
#include <elf.h>
#include <ff.h>

#include "boot.h"
#include "boot_init.h"
#include "boot_fdir.h"
#include "boot_misc.h"

/***************************** Macros Definitions ****************************/

#define FSW_FILE_PATH   "PROGRAM.ELF"  /**< Flight Software file path */
#define BUFFER_SIZE     1024u           /**< Buffer Size used for copying data */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      main(void)
 * @brief   Bootloader main is the entrypoint of the code
 * @return  0
 */
int main(void)
{
    // Variable Initialisation
    uint32_t status = 0u;
    FIL file;
    UINT bytes_read;
    Elf32_Ehdr elf_header;
    Elf32_Phdr prog_header;
    uint8_t buffer[BUFFER_SIZE];

    // First Init Boot Software
    init_boot();

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
            uint32_t chunk_size = 0u;
            uint32_t remaining = prog_header.p_filesz;
            uint32_t destination = prog_header.p_paddr;
            while (remaining > 0u)
            {
                if (remaining < BUFFER_SIZE)
                {
                    chunk_size = remaining;
                }
                else
                {
                    chunk_size = BUFFER_SIZE;
                }
                f_read(&file, buffer, chunk_size, &bytes_read);
                memcpy((void *)destination, buffer, chunk_size);
                destination += chunk_size;
                remaining -= chunk_size;
            }
        }
    }

    // Close file now
    f_close(&file);

    // Turn off blue LED
    HAL_GPIO_WritePin(BLUE_LED_GPIO_PORT, BLUE_LED_PIN, GPIO_PIN_SET);

    // Calls the entry point of the ELF program.
    void (*entry_point)(void) = (void (*)(void))elf_header.e_entry;

    // // Starts the loaded programme
    entry_point();

    return 0;
}

