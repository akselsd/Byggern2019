#ifndef USB_MULTIFUNCTION_OLED_H
#define USB_MULTIFUNCTION_OLED_H
#include <stdint.h>

/* Init module */
void oled_init(void);

/* Print @param string to OLED screen at current location */
void oled_printf(const char* string);

/* Print multiple lines to OLED */
void oled_printf_lines(const char* string, uint8_t column_start);

/* Set VRAM input page */
void oled_set_page(uint8_t page);

/* Set VRAM input column */
void oled_set_column(uint8_t column);

/* Clear whole creen, (black) */
void oled_clear_screen(void);

/* Clear specified area of screen (black) */
void oled_clear_area(const uint8_t page_start, const uint8_t page_end, const uint8_t column_start, const uint8_t column_end);

/* Display an size x size image on the OLED screen with top left corner at (column, page * 8)
   Requires UART communication and the python terminal running
    */
void oled_display_image(const char* imgname, uint8_t size, uint8_t page, uint8_t column);
#endif
