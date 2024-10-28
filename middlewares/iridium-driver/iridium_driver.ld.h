/* IRIDIUM_DRV contained sections */
#define IRIDIUMDRV_TEXT_SEGMENT     . = ALIGN(4); \
                                    __iridiumdrv_text_start__ = .; \
                                    *libiridiumdrv*.a:*(.text .text.*) \
                                    . = ALIGN(4); \
                                    __iridiumdrv_text_end__ = .;

#define IRIDIUMDRV_RODATA_SEGMENT   . = ALIGN(4); \
                                    __iridiumdrv_rodata_start__ = .; \
                                    *libiridiumdrv*.a:*(.rodata .rodata.*) \
                                    . = ALIGN(4); \
                                    __iridiumdrv_rodata_end__ = .;

#define IRIDIUMDRV_DATA_SEGMENT     . = ALIGN(4); \
                                    __iridiumdrv_data_start__ = .; \
                                    *libiridiumdrv*.a:*(.data .data.*) \
                                    . = ALIGN(4); \
                                    __iridiumdrv_data_end__ = .;

#define IRIDIUMDRV_BSS_SEGMENT      . = ALIGN(4); \
                                    __iridiumdrv_bss_start__ = .; \
                                    *libiridiumdrv*.a:*(.bss .bss.*) \
                                    . = ALIGN(4); \
                                    __iridiumdrv_bss_end__ = .;
