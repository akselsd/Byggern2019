#ifndef USB_MULTIFUNCTION_OLED_H
#define USB_MULTIFUNCTION_OLED_H

void oled_init(void);
void oled_test(void);
void oled_printf(const char* string);
void oled_set_page(unsigned int page);
void oled_set_column(unsigned int column);
void oled_clear_screen(void);

#endif
