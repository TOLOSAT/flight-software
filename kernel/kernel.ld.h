/* Kernel sections */
#define KERNEL_TEXT_SEGMENT     . = ALIGN(4); \
                                __kernel_text_start__ = .; \
                                *(.text.Default_Handler) \
                                *(.text.Reset_Handler) \
                                *(.text.SystemInit) \
                                *(.text.SystemClock_Config) \
                                *libkernel*.a:*(.text .text.*) \
                                *libos*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                __kernel_text_end__ = .;

#define KERNEL_RODATA_SEGMENT   . = ALIGN(4); \
                                __kernel_rodata_start__ = .; \
                                *libkernel*.a:*(.rodata .rodata.*) \
                                *libos*.a:*(.rodata .rodata.*) \
                                *(.conf_tables) \
                                . = ALIGN(4); \
                                __kernel_rodata_end__ = .;

#define KERNEL_DATA_SEGMENT     . = ALIGN(4); \
                                __kernel_data_start__ = .; \
                                *(.data.SystemCoreClock) \
                                *(.data.SystemD2Clock) \
                                *libkernel*.a:*(.data .data.*) \
                                *libos*.a:*(.data .data.*) \
                                *(.desc_tables) \
                                *(.task_tcbs) \
                                *(.buffer_entities) \
                                *(.buffer_arrays) \
                                *(.mutex_queues) \
                                . = ALIGN(4); \
                                __kernel_data_end__ = .;

#define KERNEL_BSS_SEGMENT      . = ALIGN(4); \
                                __kernel_bss_start__ = .; \
                                *libkernel*.a:*(.bss .bss.*) \
                                *libos*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                __kernel_bss_end__ = .;
