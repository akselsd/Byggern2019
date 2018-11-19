#ifndef USB_MULTIFUNCTION_OLED_H
#define USB_MULTIFUNCTION_OLED_H
#include <stdint.h>

void oled_init(void);
void oled_test(void);
void oled_printf(const char* string);
void oled_printf_lines(const char* string, uint8_t column_start);
void oled_set_page(uint8_t page);
void oled_set_column(uint8_t column);
void oled_clear_screen(void);
void oled_clear_area(const uint8_t page_start, const uint8_t page_end, const uint8_t column_start, const uint8_t column_end);
void oled_display_image(const char* imgname, uint8_t size, uint8_t page, uint8_t column);
#endif
