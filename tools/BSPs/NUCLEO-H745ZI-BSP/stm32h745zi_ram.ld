/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
__stack_end__ = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM); /* end of "AXI_SRAM" Ram type memory */

__Min_Heap_Size__ = 0x200; /* required amount of heap  */
__Min_Stack_Size__ = 0x400; /* required amount of stack */

/* Memories definition */
MEMORY
{
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    FLASH       (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K /* Memory is divided. Actual start is 0x08000000 and actual length is 2048K */
    DTCM_RAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
    AXI_SRAM    (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    SRAM1       (rwx) : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x30020000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x30040000, LENGTH = 32K
    SRAM4       (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
}

/* Variable used to know where the limit between code and data in AXI SRAM */
__AXI_SRAM_section_separation__ = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM)/2;

/* Sections */
SECTIONS
{
    /* The preferred codes go into the .itcm_text section, which is faster. */
    .itcm_text :
    {
        . = ALIGN(4);
        __itcm_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        *libos*.a:*(.text .text*);
        . = ALIGN(4);
        *libcore*.a:*(.text .text*);
        . = ALIGN(4);
        __itcm_end__ = .;
    } >ITCM_RAM

    /* The preferred data go into the .dtcm_data section, which is faster. */
    .dtcm_data :
    {
        . = ALIGN(4);
        __dtcm_start__ = .;
        *libos*.a:*(.rodata .rodata*);
        . = ALIGN(4);
        *libcore*.a:*(.rodata .rodata*);
        . = ALIGN(4);
        *libos*.a:*(.data .data*);
        . = ALIGN(4);
        *libcore*.a:*(.data .data*);
        . = ALIGN(4);
        *libos*.a:*(.bss .bss*);
        . = ALIGN(4);
        *libcore*.a:*(.bss .bss*);
        . = ALIGN(4);
        __dtcm_end__ = .;
    } >DTCM_RAM

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        __text_start__ = .;
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
    .tmpfs (NOLOAD) :
    {
        . = ALIGN(4);
        __tmpfs_start__ = .;
        *(.tmpfs)
        . = ALIGN(4);
        __tmpfs_end__ = .;
    } >SRAM1

    /* The dma buffers goes into .dmabuff to avoid saturating the AXI SRAM. */
    .dmabuff (NOLOAD) :
    {
        . = ALIGN(4);
        __dmabuff_start__ = .;
        *(.dmabuff)
        . = ALIGN(4);
        __dmabuff_end__ = .;
    } >SRAM3
}
