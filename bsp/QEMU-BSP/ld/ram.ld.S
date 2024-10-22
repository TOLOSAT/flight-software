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
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    DTCM_RAM    (rw)  : ORIGIN = 0x00200000, LENGTH = 128K
    AXI_SRAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 512k
    SRAM1       (rwx) : ORIGIN = 0x20200000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x20220000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x20240000, LENGTH = 32K
    PSRAM       (rw)  : ORIGIN = 0x60000000, LENGTH = 16M
}
// Note : These zones have been artificially created in the ZBT 
//        SSRAMs (with the exception of the PSRAM) of the cmsdk
//        in order to emulate the behaviour of STM32H7s.

/* Stack & Heap definitions */
__stack_end__       = ORIGIN(DTCM_RAM) + LENGTH(DTCM_RAM);
__Min_Heap_Size__   = 0x200;
__Min_Stack_Size__  = 0x400;

/* Sections */
SECTIONS
{
    /* The kernel code go into the .tcm_text section, which is faster. */
    .tcm_text :
    {
        . = ALIGN(4);
        __tcm_text_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        KERNEL_TEXT_SEGMENT
        . = ALIGN(4);
        __tcm_text_end__ = .;
    } >ITCM_RAM

    /* ARM exception handling indexes goes after TCM text memory */
    .ARM.eidx : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        . = ALIGN(4);
        __exidx_end = .;
    } >ITCM_RAM

    /* The kernel read only data go into the .tcm_rodata section, which is faster. */
    .tcm_rodata :
    {
        . = ALIGN(4);
        __tcm_rodata_start__ = .;
        KERNEL_RODATA_SEGMENT
        . = ALIGN(4);
        __tcm_rodata_end__ = .;
    } >DTCM_RAM

    /* ARM exception handling tables goes after tcm rodata memory */
    .ARM.extab : 
    {
        . = ALIGN(4);
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
        __extab_end = .;
    } >DTCM_RAM

    /* The kernel data go into the .tcm_data section, which is faster. */
    .tcm_data :
    {
        . = ALIGN(4);
        __tcm_data_start__ = .;
        KERNEL_DATA_SEGMENT
        . = ALIGN(4);
        __tcm_data_end__ = .;
    } >DTCM_RAM

    /* The kernel bss go into the .tcm_bss section, which is faster. */
    .tcm_bss (NOLOAD) :
    {
        __tcm_bss_start__ = .;
        . = ALIGN(4);
        KERNEL_BSS_SEGMENT
        . = ALIGN(4);
        __tcm_bss_end__ = .;
    } >DTCM_RAM

    /* Use of the rest of DTCM RAM for the stack(s) */
    .stack :
    {
        . = ALIGN(8);
        . = . + __Min_Stack_Size__;
        . = ALIGN(8);
    } >DTCM_RAM

    /* The program goes into the .text section */
    .text :
    {
        . = ALIGN(4);
        __text_start__ = .;
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
    } >AXI_SRAM

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
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
    } >AXI_SRAM

    /* Initialized data go into the .data section */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
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
    } >AXI_SRAM

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD):
    {
        __bss_start__ = .;
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
        __bss_end__ = .;
    } >AXI_SRAM

    /* Use of the rest of AXI_SRAM for the heap (should remain empty) */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + __Min_Heap_Size__;
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

    /* PSRAM zone used for RAM file system */
    .ramfs (NOLOAD):
    {
        . = ALIGN(4);
        __ramfs_start__ = .;
        . = . + LENGTH(PSRAM);
        __ramfs_end__ = .;
        . = ALIGN(4);
    } >PSRAM
}
