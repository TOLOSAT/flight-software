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
    DTCM_RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
    AXI_SRAM    (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    SRAM1       (rwx) : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x30200000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x30400000, LENGTH = 32K
    SRAM4       (rwx) : ORIGIN = 0x38000000, LENGTH = 64K
}

/* Variables used by FreeRTOS-MPU. */
__privileged_functions_region_size__  = 64K;
__privileged_data_region_size__       = 8K;

__FLASH_segment_start__               = ORIGIN( FLASH );
__FLASH_segment_end__                 = __FLASH_segment_start__ + LENGTH( FLASH );

__SRAM_segment_start__                = ORIGIN( AXI_SRAM );
__SRAM_segment_end__                  = __SRAM_segment_start__ + LENGTH( AXI_SRAM );

/* Sections */
SECTIONS
{
    /* ARM Interrupt Vectors and FreeRTOS kernel code are placed into privileged memory. */
    .privileged_functions :
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
    } >AXI_SRAM

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
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
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >AXI_SRAM

    /* Used by the startup to initialize data */
    __data_start_initialize__ = LOADADDR(.privileged_data);

    /* FreeRTOS kernel data. */
    .privileged_data :
    {
        . = ALIGN(4);
        __privileged_data_start__ = .;
        __data_start__ = .;
        *(privileged_data)
        . = ALIGN(4);
        FILL(0xDEAD);
        . = __privileged_data_start__ + __privileged_data_region_size__;
        __privileged_data_end__ = .;
    } >AXI_SRAM

    /* Initialized data go into the .bss section */
    .data :
    {
        . = ALIGN(4);
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >AXI_SRAM

    /* Uninitialised data goes into .bss section */
    .bss :
    {
        . = ALIGN(4);
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
}
