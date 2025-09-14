/* SYSCALLS sections */
#define SYSCALLS                . = ALIGN(4); \
                                __syscalls_start__ = .; \
                                *(.syscalls) \
                                . = ALIGN(4); \
                                __syscalls_end__ = .;

/* BSP sections */
#define BSP_TEXT_SEGMENT        . = ALIGN(4); \
                                __bsp_text_start__ = .; \
                                *libbsp*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                __bsp_text_end__ = .;

#define BSP_RODATA_SEGMENT      . = ALIGN(4); \
                                __bsp_rodata_start__ = .; \
                                *libbsp*.a:*(.rodata .rodata.*) \
                                . = ALIGN(4); \
                                __bsp_rodata_end__ = .;

#define BSP_DATA_SEGMENT        . = ALIGN(4); \
                                __bsp_data_start__ = .; \
                                *libbsp*.a:*(.data .data.*) \
                                . = ALIGN(4); \
                                __bsp_data_end__ = .;

#define BSP_BSS_SEGMENT         . = ALIGN(4); \
                                __bsp_bss_start__ = .; \
                                *libbsp*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                __bsp_bss_end__ = .;

/* HAL sections */

#define HAL_TEXT_SEGMENT        . = ALIGN(4); \
                                __hal_text_start__ = .; \
                                *libhal*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                __hal_text_end__ = .;

#define HAL_RODATA_SEGMENT      . = ALIGN(4); \
                                __hal_rodata_start__ = .; \
                                *libhal*.a:*(.rodata .rodata.*) \
                                . = ALIGN(4); \
                                __hal_rodata_end__ = .;

#define HAL_DATA_SEGMENT        . = ALIGN(4); \
                                __hal_data_start__ = .; \
                                *libhal*.a:*(.data .data.*) \
                                . = ALIGN(4); \
                                __hal_data_end__ = .;

#define HAL_BSS_SEGMENT         . = ALIGN(4); \
                                __hal_bss_start__ = .; \
                                *libhal*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                __hal_bss_end__ = .;

/* FREERTOS sections */

#define FREERTOS_TEXT_SEGMENT      . = ALIGN(4); \
                                __freertos_text_start__ = .; \
                                *libos*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                __freertos_text_end__ = .;

#define FREERTOS_RODATA_SEGMENT    . = ALIGN(4); \
                                __freertos_rodata_start__ = .; \
                                *libos*.a:*(.rodata .rodata.*) \
                                . = ALIGN(4); \
                                __freertos_rodata_end__ = .;

#define FREERTOS_DATA_SEGMENT      . = ALIGN(4); \
                                __freertos_data_start__ = .; \
                                *libos*.a:*(.data .data.*) \
                                . = ALIGN(4); \
                                __freertos_data_end__ = .;

#define FREERTOS_BSS_SEGMENT       . = ALIGN(4); \
                                __freertos_bss_start__ = .; \
                                *libos*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                __freertos_bss_end__ = .;

/* FATFS sections */

#define FATFS_TEXT_SEGMENT      . = ALIGN(4); \
                                __fatfs_text_start__ = .; \
                                *libfatfs*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                __fatfs_text_end__ = .;

#define FATFS_RODATA_SEGMENT    . = ALIGN(4); \
                                __fatfs_rodata_start__ = .; \
                                *libfatfs*.a:*(.rodata .rodata.*) \
                                . = ALIGN(4); \
                                __fatfs_rodata_end__ = .;

#define FATFS_DATA_SEGMENT      . = ALIGN(4); \
                                __fatfs_data_start__ = .; \
                                *libfatfs*.a:*(.data .data.*) \
                                . = ALIGN(4); \
                                __fatfs_data_end__ = .;

#define FATFS_BSS_SEGMENT       . = ALIGN(4); \
                                __fatfs_bss_start__ = .; \
                                *libfatfs*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                __fatfs_bss_end__ = .;