#ifndef MENU_H
#define MENU_H

void menu_draw_options(const char ** options, uint8_t n_options);
uint8_t menu_select_option(uint8_t n_options);
void menu_display_score(uint16_t score);
void menu_init(void);

#endif
