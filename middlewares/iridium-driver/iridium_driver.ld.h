/* IRIDIUM_DRV contained sections */
#define IRIDIUMDRV_TEXT_SEGMENT     . = ALIGN(4); \
                                    _iridiumdrv_text_start_ = .; \
                                    *libiridiumdrv*.a:*(.text .text.*) \
                                    . = ALIGN(4); \
                                    _iridiumdrv_text_end_ = .;

#define IRIDIUMDRV_RODATA_SEGMENT   . = ALIGN(4); \
                                    _iridiumdrv_rodata_start_ = .; \
                                    *libiridiumdrv*.a:*(.rodata .rodata.*) \
                                    . = ALIGN(4); \
                                    _iridiumdrv_rodata_end_ = .;

#define IRIDIUMDRV_DATA_SEGMENT     . = ALIGN(4); \
                                    _iridiumdrv_data_start_ = .; \
                                    *libiridiumdrv*.a:*(.data .data.*) \
                                    . = ALIGN(4); \
                                    _iridiumdrv_data_end_ = .;

#define IRIDIUMDRV_BSS_SEGMENT      . = ALIGN(4); \
                                    _iridiumdrv_bss_start_ = .; \
                                    *libiridiumdrv*.a:*(.bss .bss.*) \
                                    . = ALIGN(4); \
                                    _iridiumdrv_bss_end_ = .;
