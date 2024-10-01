/* IRIDIUM_DRV contained sections */
#define IRIDIUM_DRV_TEXT_SEGMENT    . = ALIGN(4); \
                                    _text_iridiumdrv_start_ = .; \
                                    *libiridiumdrv*.a:*(.text .text.*) \
                                    . = ALIGN(4); \
                                    _text_iridiumdrv_end_ = .;

#define IRIDIUM_DRV_DATA_SEGMENT    . = ALIGN(4); \
                                    _data_iridiumdrv_start_ = .; \
                                    *libiridiumdrv*.a:*(.data .data.* .bss .bss.*) \
                                    . = ALIGN(4); \
                                    _data_iridiumdrv_end_ = .;
