/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM); /* end of "AXI_SRAM" Ram type memory */

_Min_Heap_Size = 0x200; /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Memories definition */
MEMORY
{
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    DTCM_RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    AXI_SRAM    (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    SRAM1       (rwx) : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x30200000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x30400000, LENGTH = 32K
    SRAM4       (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
    FLASH       (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
}

/* Sections */
SECTIONS
{
    /* ARM interrupt vectors go into the .isr_vector section. */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >AXI_SRAM

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
        _etext = .;
    } >AXI_SRAM

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >AXI_SRAM

    /* Used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* Initialized data go into the .bss section */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >AXI_SRAM

    /* Uninitialised data goes into .bss section */
    .bss :
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >AXI_SRAM

    /* Use of the rest of RAM for the stack and heap */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >AXI_SRAM
}
