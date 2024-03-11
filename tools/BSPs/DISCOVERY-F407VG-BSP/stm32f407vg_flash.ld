/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
  FLASH(rx)   : ORIGIN = 0x08000000, LENGTH = 1024K
  SRAM(rwx)   : ORIGIN = 0x20000000, LENGTH = 128K
  CCMRAM(rwx) : ORIGIN = 0x10000000, LENGTH = 64K
}

/* Stack & Heap definitions */
__stack_end__ = ORIGIN(SRAM) + LENGTH(SRAM);
__Min_Heap_Size__ = 0x200;
__Min_Stack_Size__ = 0x400;

/* Variables used by FreeRTOS-MPU. */
__privileged_functions_region_size__  = 64K;
__privileged_data_region_size__       = 8K;

__FLASH_segment_start__               = ORIGIN( FLASH );
__FLASH_segment_end__                 = __FLASH_segment_start__ + LENGTH( FLASH );

__SRAM_segment_start__                = ORIGIN( SRAM );
__SRAM_segment_end__                  = __SRAM_segment_start__ + LENGTH( SRAM );

/* Sections */
SECTIONS
{
    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        __privileged_functions_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        *(privileged_functions)
        . = ALIGN(4);
        FILL(0xDEAD);
        . = __privileged_functions_start__ + __privileged_functions_region_size__;
        __privileged_functions_end__ = .;
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
    } >FLASH

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
    } >FLASH

    /* Used by the startup to initialize data */
    __data_start_initialize__ = LOADADDR(.data);

    /* Initialized data go into the .data section */
    .data :
    {
        . = ALIGN(4);
        __privileged_data_start__ = .;
        __data_start__ = .;
        *(privileged_data)
        . = ALIGN(4);
        FILL(0xDEAD);
        . = __privileged_data_start__ + __privileged_data_region_size__;
        __privileged_data_end__ = .;
        . = ALIGN(4);
        *(.data)
        *(.data*)
        *(.dynamic_conf_table)
        . = ALIGN(4);
        __data_end__ = .;
    } >SRAM AT> FLASH

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        *(.task_stacks)
        *(.buffer_data)
        *(.tmpfs)
        *(.dmabuff)
        . = ALIGN(4);
        __bss_end__ = .;
    } >SRAM

    /* Use of the rest of SRAM for the stack and heap */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >SRAM
}
