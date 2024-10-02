/* Kernel contained sections */
#define KERNEL_TEXT_SEGMENT     . = ALIGN(4); \
                                _kernel_text_start_ = .; \
                                *libkernel*.a:*(.text .text.*) \
                                *libos*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                _kernel_text_end_ = .;

#define KERNEL_DATA_SEGMENT     . = ALIGN(4); \
                                _kernel_data_start_ = .; \
                                *libkernel*.a:*(.data .data.*) \
                                *libos*.a:*(.data .data.*) \
                                . = ALIGN(4); \
                                _kernel_data_end_ = .;

#define KERNEL_BSS_SEGMENT      . = ALIGN(4); \
                                _kernel_bss_start_ = .; \
                                *libkernel*.a:*(.bss .bss.*) \
                                *libos*.a:*(.bss .bss.*) \
                                . = ALIGN(4); \
                                _kernel_bss_end_ = .;
