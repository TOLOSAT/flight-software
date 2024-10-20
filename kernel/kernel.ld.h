/* Kernel contained sections */
#define KERNEL_TEXT_SEGMENT     . = ALIGN(4); \
                                _kernel_text_start_ = .; \
                                *(.text.Default_Handler) \
                                *(.text.Reset_Handler) \
                                *(.text.SystemInit) \
                                *(.text.SystemClock_Config) \
                                *libkernel*.a:*(.text .text.*) \
                                *libos*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                _kernel_text_end_ = .;

#define KERNEL_RODATA_SEGMENT   . = ALIGN(4); \
                                _kernel_rodata_start_ = .; \
                                *libkernel*.a:*(.rodata .rodata.*) \
                                *libos*.a:*(.rodata .rodata.*) \
                                *(.conf_tables) \
                                . = ALIGN(4); \
                                _kernel_rodata_end_ = .;

#define KERNEL_DATA_SEGMENT     . = ALIGN(4); \
                                _kernel_data_start_ = .; \
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
                                _kernel_data_end_ = .;

#define KERNEL_BSS_SEGMENT      . = ALIGN(4); \
                                _kernel_bss_start_ = .; \
                                *libkernel*.a:*(.bss .bss.*) \
                                *libos*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                _kernel_bss_end_ = .;
