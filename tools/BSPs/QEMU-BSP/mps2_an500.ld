/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    DTCM_RAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 64K
    PSRAM       (rw)  : ORIGIN = 0x60000000, LENGTH = 16M
}

/* Stack & Heap definitions */
__stack_end__ = ORIGIN(DTCM_RAM) + LENGTH(DTCM_RAM);
__Min_Heap_Size__ = 0x200;
__Min_Stack_Size__ = 0x400;

/* Sections */
SECTIONS
{
    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        __text_start__ = .;
        KEEP(*(.isr_vector))
        *(.text)
        *(.text*)
        . = ALIGN(4);
        __text_end__ = .;
    } >ITCM_RAM

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        __rodata_end__ = .;
    } >DTCM_RAM

    /* Used by the startup to initialize data */
    __data_start_initialize__ = LOADADDR(.data);

    /* Initialized data go into the .data section */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >DTCM_RAM

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } >DTCM_RAM

    /* Use of the rest of RAM for the stack and heap */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >DTCM_RAM
}
