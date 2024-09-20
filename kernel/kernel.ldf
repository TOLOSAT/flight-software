/* Put section macro */
#ifndef PUT_SECTION
#define PUT_SECTION(section)    . = ALIGN(4); \
                                _##section##_start_ = .; \
                                *(.section) \
                                . = ALIGN(4); \
                                _##section##_end_ = .;
#endif

/* Core contained sections */
#define CORE_TEXT_SEGMENT       PUT_SECTION(text_core)
#define CORE_DATA_SEGMENT       PUT_SECTION(data_core)