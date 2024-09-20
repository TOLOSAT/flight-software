/* Put section macro */
#ifndef PUT_SECTION
#define PUT_SECTION(section)    . = ALIGN(4); \
                                _##section##_start_ = .; \
                                *(.section) \
                                . = ALIGN(4); \
                                _##section##_end_ = .;
#endif

/* Application contained sections */
#define APP_TEXT_SEGMENT    PUT_SECTION(text_application) \

#define APP_DATA_SEGMENT    PUT_SECTION(data_application)