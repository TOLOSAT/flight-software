#include "applications.ld.h"
#include "kernel.ld.h"
#include "pus-library.ld.h"
#include "iridium_driver.ld.h"
#include "common.ld.h"

/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
    FLASH       (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K /* Memory is divided. Actual start is 0x08000000 and actual length is 2048K */
    AXI_SRAM    (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
}

/* Stack & Heap definitions */
__stack_end__ = ORIGIN(AXI_SRAM) + LENGTH(AXI_SRAM);
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
        KERNEL_TEXT_SEGMENT
        SYSCALLS
        APP_TEXT_SEGMENT
        PUS_TEXT_SEGMENT
        IRIDIUMDRV_TEXT_SEGMENT
        HAL_TEXT_SEGMENT
        FATFS_TEXT_SEGMENT
        BSP_TEXT_SEGMENT
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
        KERNEL_RODATA_SEGMENT
        APP_RODATA_SEGMENT
        PUS_RODATA_SEGMENT
        IRIDIUMDRV_RODATA_SEGMENT
        HAL_RODATA_SEGMENT
        FATFS_RODATA_SEGMENT
        BSP_RODATA_SEGMENT
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
        KERNEL_DATA_SEGMENT
        APP_DATA_SEGMENT
        PUS_DATA_SEGMENT
        IRIDIUMDRV_DATA_SEGMENT
        HAL_DATA_SEGMENT
        FATFS_DATA_SEGMENT
        BSP_DATA_SEGMENT
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >AXI_SRAM AT> FLASH

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        KERNEL_BSS_SEGMENT
        APP_BSS_SEGMENT
        PUS_BSS_SEGMENT
        IRIDIUMDRV_BSS_SEGMENT
        HAL_BSS_SEGMENT
        FATFS_BSS_SEGMENT
        BSP_BSS_SEGMENT
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
