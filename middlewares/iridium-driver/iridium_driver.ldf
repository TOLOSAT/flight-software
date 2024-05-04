/* Put section macro */
#ifndef PUT_SECTION
#define PUT_SECTION(section)    . = ALIGN(4); \
                                _##section##_start_ = .; \
                                *(.section) \
                                . = ALIGN(4); \
                                _##section##_end_ = .;
#endif

/* IRIDIUM_DRV contained sections */
#define IRIDIUM_DRV_TEXT_SEGMENT        PUT_SECTION(text_iridium_drv)
#define IRIDIUM_DRV_DATA_SEGMENT        PUT_SECTION(data_iridium_drv)