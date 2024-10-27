/* PUS contained sections */
#define PUS_TEXT_SEGMENT    . = ALIGN(4); \
                            __pus_text_start__ = .; \
                            *libpus*.a:*(.text .text.*) \
                            . = ALIGN(4); \
                            __pus_text_end__ = .;

#define PUS_RODATA_SEGMENT  . = ALIGN(4); \
                            __pus_rodata_start__ = .; \
                            *libpus*.a:*(.rodata .rodata.*) \
                            . = ALIGN(4); \
                            __pus_rodata_end__ = .;

#define PUS_DATA_SEGMENT    . = ALIGN(4); \
                            __pus_data_start__ = .; \
                            *libpus*.a:*(.data .data.*) \
                            . = ALIGN(4); \
                            __pus_data_end__ = .;

#define PUS_BSS_SEGMENT    . = ALIGN(4); \
                            __pus_bss_start__ = .; \
                            *libpus*.a:*(.bss .bss.*) \
                            . = ALIGN(4); \
                            __pus_bss_end__ = .;