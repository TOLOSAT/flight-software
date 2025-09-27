/* Entry Point */
ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
    ITCM_RAM    (rwx) : ORIGIN = 0x00000000, LENGTH = 64K
    DTCM_RAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
    AXI_SRAM    (rwx) : ORIGIN = 0x24000000, LENGTH = 512K
    SRAM1       (rwx) : ORIGIN = 0x30000000, LENGTH = 128K
    SRAM2       (rwx) : ORIGIN = 0x30020000, LENGTH = 128K
    SRAM3       (rwx) : ORIGIN = 0x30040000, LENGTH = 32K
    SDRAM       (rw)  : ORIGIN = 0xC0000000, LENGTH = 32M
}

/* Stack & Heap definitions */
__stack_end__       = ORIGIN(DTCM_RAM) + LENGTH(DTCM_RAM);
__Min_Heap_Size__   = 0x200;
__Min_Stack_Size__  = 0x400;

/* Sections */
SECTIONS
{
    /* Par of the kernel code go into the .tcm_text section, which is faster. */
    .tcm_text :
    {
        . = ALIGN(4);
        __tcm_text_start__ = .;
        KEEP(*(.isr_vector))
        . = ALIGN(4);
        __kernel_itm_text_start__ = .;
        *(.text.Default_Handler)
        *(.text.Reset_Handler)
        *(.text.SystemInit)
        *(.text.SystemClock_Config)
        *libkernel*.a:*(.text .text.*)
        . = ALIGN(4);
        __kernel_itm_text_end__ = .;
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

    /* Part of the kernel read only data go into the .tcm_rodata section, which is faster. */
    .tcm_rodata :
    {
        . = ALIGN(4);
        __tcm_rodata_start__ = .;
        __kernel_dtm_rodata_start__ = .;
        *libkernel*.a:*(.rodata .rodata.*)
        *(.conf_tables)
        . = ALIGN(4);
        __kernel_dtm_rodata_end__ = .;
        . = ALIGN(4);
        __tcm_rodata_end__ = .;
    } >DTCM_RAM

    /* ARM exception handling tables goes after tcm rodata memory */
    .ARM.extab :
    {
        . = ALIGN(4);
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(0x4000); /* Align on 16KB for MPU RO region */
        __extab_end = .;
    } >DTCM_RAM

    /* Part of the kernel data go into the .tcm_data section, which is faster. */
    .tcm_data :
    {
        . = ALIGN(4);
        __tcm_data_start__ = .;
        __kernel_dtm_data_start__ = .;
        *(.data.SystemCoreClock)
        *(.data.SystemD2Clock)
        *libkernel*.a:*(.data .data.*)
        *(.desc_tables)
        *(.task_tcbs)
        *(.buffer_queues)
        *(.buffer_arrays)
        *(.mutex_queues)
        *(.tim_buffers)
        . = ALIGN(4);
        __kernel_dtm_data_end__ = .;
        . = ALIGN(4);
        __tcm_data_end__ = .;
    } >DTCM_RAM

    /* Part of the kernel bss go into the .tcm_bss section, which is faster. */
    .tcm_bss (NOLOAD) :
    {
        __tcm_bss_start__ = .;
        . = ALIGN(4);
        __kernel_dtm_bss_start__ = .;
        *libkernel*.a:*(.bss .bss.*)
        . = ALIGN(4);
        __kernel_dtm_bss_end__ = .;
        . = ALIGN(4);
        __tcm_bss_end__ = .;
    } >DTCM_RAM

    /* Use of the rest of DTCM RAM for the main stack */
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
        __kernel_text_start__ = .;
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
    } >AXI_SRAM

    /* Constant data goes into .rodata section */
    .rodata :
    {
        . = ALIGN(4);
        __rodata_start__ = .;
        __kernel_rodata_start__ = .;
        *libfreertos*.a:*(.rodata .rodata.*)
        *libfatfs*.a:*(.rodata .rodata.*)
        *libhal*.a:*(.rodata .rodata.*)
        . = ALIGN(4);
        __kernel_rodata_end__ = .;
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
        __kernel_data_start__ = .;
        *libfreertos*.a:*(.data .data.*)
        *libfatfs*.a:*(.data .data.*)
        *libhal*.a:*(.data .data.*)
        . = ALIGN(4);
        __kernel_data_end__ = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end__ = .;
    } >AXI_SRAM

    /* Uninitialised data goes into .bss section */
    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __bss_start__ = .;
        __kernel_bss_start__ = .;
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

    /* SDRAM zone used for RAM file system */
    .ramfs (NOLOAD):
    {
        . = ALIGN(4);
        __ramfs_start__ = .;
        . = . + LENGTH(SDRAM);
        __ramfs_end__ = .;
        . = ALIGN(4);
    } >SDRAM
}
