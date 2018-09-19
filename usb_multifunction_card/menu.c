#include "menu.h"
#include "oled.h"

#define MENU_ACTIONS 4

const char * const menu[MENU_ACTIONS] = {
    "Singleplayer",
    "Multiplayer",
    "Game 3",
    "Game 4",
};

void menu_init(void) {
	for (int i = 0; i < MENU_ACTIONS; i++) {
        oled_set_column(20);
        oled_set_page(i);
        oled_printf(menu[i]);
    }
}