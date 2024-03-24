/**
 * @file    boot.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for bootloader
 * @date    24/03/2024
 * 
 * @copyright Copyright (c) TOLOSAT
 */

/******************************* Include Files *******************************/

#include "boot.h"

/***************************** Macros Definitions ****************************/

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
    FIL file;
    UINT bytes_read;
    elfFileHeader_t elf_header;
    elfProgramHeader_t prog_header;

    // Opens the ELF file.
    if (f_open(&file, "SW_00000.elf", FA_READ) != FR_OK) 
    {
        // Error occured file cannot be opened
        while (1)
        {
            /* Do Nothing */
        }
    }

    // Reads the ELF header.
    (void)f_read(&file, &elf_header, sizeof(elfFileHeader_t), &bytes_read);

    // Check the magic number ELF.
    if (memcmp(elf_header.magic, "\177ELF", 4) != 0) 
    {
        // Error occured file is not an ELF file
        (void)f_close(&file);
        while (1)
        {
            /* Do Nothing */
        }
    }

    // Reads and processes each programme header.
    for (int i = 0; i < elf_header.phnum; i++) 
    {
        // Sets the file pointer to the program header.
        (void)f_lseek(&file, elf_header.phoff + (i * sizeof(elfProgramHeader_t)));
        (void)f_read(&file, &prog_header, sizeof(elfProgramHeader_t), &bytes_read);

        // Loads only the segments in memory.
        if (prog_header.type == 1u) // PT_LOAD, memory load required.
        {
            void* segment = malloc(prog_header.memsz);  // Allocates memory for the segment.
            (void)memset(segment, 0, prog_header.memsz);  // Initializes memory with zeros.
            (void)f_lseek(&file, prog_header.offset);  // Moves to the offset of the segment in the file.
            (void)f_read(&file, segment, prog_header.filesz, &bytes_read);  // Reads the file segment.
            (void)memcpy((void*)prog_header.vaddr, segment, prog_header.filesz);  // Copies the segment to RAM.
            (void)free(segment);  // Releases the memory allocated for the segment.
        }
    }

    // Close file now
    (void)f_close(&file);

    // Calls the entry point of the ELF program.
    void (*entry_point)(void) = (void (*)(void))elf_header.entry;

    // // Starts the loaded programme
    entry_point();  

    return 0;
}
