/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
  RAM    (xrw)   : ORIGIN = 0x20000000,   LENGTH = 128K
  FLASH  (rx)    : ORIGIN = 0x8000000,   LENGTH = 512K
}

/* Stack & Heap definitions */
__stack_end__ = ORIGIN(RAM) + LENGTH(RAM);
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
        . = ALIGN(4);
        __kernel_text_start__ = .;
        *(.text.Default_Handler)
        *(.text.Reset_Handler)
        *(.text.SystemInit)
        *(.text.SystemClock_Config)
        *libkernel*.a:*(.text .text.*)
        *libfreertos*.a:*(.text .text.*)
        *libfatfs*.a:*(.text .text.*)
        *libhal*.a:*(.text .text.*)
        . = ALIGN(4);
        __kernel_text_end__ = .;
        __syscalls_start__ = .;
        *(.syscalls)
        . = ALIGN(4);
        __syscalls_end__ = .;
        *(.text)
        *(.text*)
        . = ALIGN(4);
        __text_end__ = .;
    } >FLASH

    /* ARM exception handling indexes goes after text memory */
    .ARM.eidx : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        . = ALIGN(4);
        __exidx_end = .;
    } >FLASH

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        __kernel_rodata_start__ = .;
        *libkernel*.a:*(.rodata .rodata.*)
        *libfreertos*.a:*(.rodata .rodata.*)
        *libfatfs*.a:*(.rodata .rodata.*)
        *libhal*.a:*(.rodata .rodata.*)
        . = ALIGN(4);
        __kernel_rodata_end__ = .;
        *(.config)
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        __rodata_end__ = .;
    } >FLASH

    /* ARM exception handling tables goes after rodata memory */
    .ARM.extab :
    {
        . = ALIGN(4);
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
        __extab_end = .;
    } >FLASH

    /* Used by the startup to initialize data */
    __data_start_initialize__ = LOADADDR(.data);

    /* Initialized data go into the .data section */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        __kernel_data_start__ = .;
        *(.data.SystemCoreClock)
        *(.data.SystemD2Clock)
        *libkernel*.a:*(.data .data.*)
        *libfreertos*.a:*(.data .data.*)
        *libfatfs*.a:*(.data .data.*)
        *libhal*.a:*(.data .data.*)
        . = ALIGN(4);
        __kernel_data_end__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM AT> FLASH

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        __kernel_bss_start__ = .;
        *libkernel*.a:*(.bss .bss.*)
        *libfreertos*.a:*(.bss .bss.*)
        *libfatfs*.a:*(.bss .bss.*)
        *libhal*.a:*(.bss .bss.*)
        . = ALIGN(4);
        __kernel_bss_end__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        *(.task_stacks)
        . = ALIGN(4);
        *(.tmpfs)
        . = ALIGN(4);
        *(.dmabuff)
        . = ALIGN(4);
        __bss_end__ = .;
    } >RAM

    /* Use of the rest of RAM for the stack and heap */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >RAM
}
