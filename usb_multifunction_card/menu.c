#include <stdbool.h>

#include "menu.h"
#include "oled.h"
#include "../usb_multifunction_card/joystick.h"

#define MENU_ACTIONS 4
#define CURSOR "->"

const char * const menu[MENU_ACTIONS] = {
    "Singleplayer",
    "Multiplayer",
    "Game 3",
    "Game 4",
};

static void move_cursor(const int menu_row)
{
    oled_set_column(0);
    oled_set_page(menu_row);
    oled_printf(CURSOR);
}

bool menu_select(void)
{
    int current_menu_row = 0;
    joystick_status joystick = joystick_get_status(); 

    switch(joystick.dir)
    {
	case UP:
	    --current_menu_row;
	    break;
	case DOWN:
	    ++current_menu_row;
	    break;
	default:
	    break;
    }

    move_cursor(current_menu_row );

    // TODO implement button functionality
    return false;
}

void menu_init(void)
{
    move_cursor(0);

    for (int i = 0; i < MENU_ACTIONS; i++)
    {
	// Make space for the cursor
        oled_set_column(20);
        oled_set_page(i);
        oled_printf(menu[i]);
    }
}

