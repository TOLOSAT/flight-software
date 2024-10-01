/* Kernel contained sections */
#define KERNEL_TEXT_SEGMENT     . = ALIGN(4); \
                                _text_kernel_start_ = .; \
                                *libkernel*.a:*(.text .text.*) \
                                *libos*.a:*(.text .text.*) \
                                . = ALIGN(4); \
                                _text_kernel_end_ = .;

#define KERNEL_DATA_SEGMENT     . = ALIGN(4); \
                                _data_kernel_start_ = .; \
                                *libkernel*.a:*(.data .data.* .bss .bss.*) \
                                *libos*.a:*(.data .data.* .bss .bss.*) \
                                . = ALIGN(4); \
                                _data_kernel_end_ = .;
