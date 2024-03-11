/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    FLASH       (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
    DTCM_RAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
    AXI_SRAM    (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    SRAM1       (rwx) : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x30020000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x30040000, LENGTH = 32K
    SRAM4       (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
}

/* Stack & Heap definitions */
__stack_end__                           = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM);
__Min_Heap_Size__                       = 0x200;
__Min_Stack_Size__                      = 0x400;

/* Variable used to know where the limit between code and data in AXI SRAM */
__AXI_SRAM_section_separation__         = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM)/2;

/* Variables used by FreeRTOS-MPU. */
__privileged_functions_region_size__    = 64K;
__privileged_data_region_size__         = 8K;

__FLASH_segment_start__                 = ORIGIN(AXI_SRAM);
__FLASH_segment_end__                   = __AXI_SRAM_section_separation__;

__SRAM_segment_start__                  = __AXI_SRAM_section_separation__;
__SRAM_segment_end__                    = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM);

/* Sections */
SECTIONS
{
    /* The preferred codes go into the .itcm_text section, which is faster. */
    .itcm_text :
    {
        . = ALIGN(4);
        __itcm_start__ = .;
        __privileged_functions_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        *(privileged_functions)
        . = ALIGN(4);
        FILL(0xDEAD);
        . = __privileged_functions_start__ + __privileged_functions_region_size__;
        __privileged_functions_end__ = .;
        __itcm_end__ = .;
    } >ITCM_RAM

    /* The preferred data go into the .dtcm_data section, which is faster. */
    .dtcm_data :
    {
        . = ALIGN(4);
        __dtcm_start__ = .;
        __privileged_data_start__ = .;
        *(privileged_data)
        . = ALIGN(4);
        FILL(0xDEAD);
        . = __privileged_data_start__ + __privileged_data_region_size__;
        __privileged_data_end__ = .;
        . = ALIGN(4);
        *(.dynamic_conf_table)
        *(.task_stacks)
        *(.buffer_data)
        __dtcm_end__ = .;
    } >DTCM_RAM

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        __text_start__ = .;
        __syscalls_flash_start__ = .;
        *(freertos_system_calls)
        __syscalls_flash_end__ = .;
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
        __text_end__ = .;
    } >AXI_SRAM

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        *(.rodata)
        *(.rodata*)
        *(.static_conf_table)
        . = ALIGN(4);
        __rodata_end__ = .;
    } >AXI_SRAM

    /* Initialized data go into the .data section */
    .data __AXI_SRAM_section_separation__ :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >AXI_SRAM

    /* Uninitialised data goes into .bss section */
    .bss :
    {
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } >AXI_SRAM

    /* Use of the rest of RAM for the stack and heap */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >AXI_SRAM

    /* The temporary files of the file system goes into .tmpfs to avoid saturating the AXI SRAM. */
    .tmpfs :
    {
        . = ALIGN(4);
        __tmpfs_start__ = .;
        *(.tmpfs)
        . = ALIGN(4);
        __tmpfs_end__ = .;
    } >SRAM2

    /* The dma buffers goes into .dmabuff to avoid saturating the AXI SRAM. */
    .dmabuff :
    {
        . = ALIGN(4);
        __dmabuff_start__ = .;
        *(.dmabuff)
        . = ALIGN(4);
        __dmabuff_end__ = .;
    } >SRAM3
}

