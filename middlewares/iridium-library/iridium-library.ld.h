/* IRIDIUM_DRV contained sections */
#define IRIDIUM_TEXT_SEGMENT     . = ALIGN(4); \
                                    __iridium_text_start__ = .; \
                                    *libiridium*.a:*(.text .text.*) \
                                    . = ALIGN(4); \
                                    __iridium_text_end__ = .;

#define IRIDIUM_RODATA_SEGMENT   . = ALIGN(4); \
                                    __iridium_rodata_start__ = .; \
                                    *libiridium*.a:*(.rodata .rodata.*) \
                                    . = ALIGN(4); \
                                    __iridium_rodata_end__ = .;

#define IRIDIUM_DATA_SEGMENT     . = ALIGN(4); \
                                    __iridium_data_start__ = .; \
                                    *libiridium*.a:*(.data .data.*) \
                                    . = ALIGN(4); \
                                    __iridium_data_end__ = .;

#define IRIDIUM_BSS_SEGMENT      . = ALIGN(4); \
                                    __iridium_bss_start__ = .; \
                                    *libiridium*.a:*(.bss .bss.*) \
                                    . = ALIGN(4); \
                                    __iridium_bss_end__ = .;
