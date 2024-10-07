#include "application.ld.h"
#include "kernel.ld.h"
#include "pus-library.ld.h"
#include "iridium_driver.ld.h"
#include "common.ld.h"

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
        KERNEL_TEXT_SEGMENT
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
    } >ITCM_RAM

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        *(.rodata)
        *(.rodata*)
        *(.conf_tables)
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
    } >AXI_SRAM

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
        *(.desc_tables)
        . = ALIGN(4);
        __data_end__ = .;
    } >RAM AT> FLASH

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
        *(.task_tcbs)
        *(.task_stacks)
        *(.buffer_entities)
        *(.buffer_arrays)
        *(.mutex_queues)
        *(.tmpfs)
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
