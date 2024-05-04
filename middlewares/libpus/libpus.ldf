/* Put section macro */
#ifndef PUT_SECTION
#define PUT_SECTION(section)    . = ALIGN(4); \
                                _##section##_start_ = .; \
                                *(.section) \
                                . = ALIGN(4); \
                                _##section##_end_ = .;
#endif

/* PUS contained sections */
#define PUS_TEXT_SEGMENT        PUT_SECTION(text_pus)
#define PUS_DATA_SEGMENT        PUT_SECTION(data_pus)