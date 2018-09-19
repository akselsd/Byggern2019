#include <stdbool.h>

#include "menu.h"
#include "oled.h"
#include "../usb_multifunction_card/joystick.h"

#define MENU_ACTIONS 4
#define CURSOR "->"
#define CURSOR_SPACE 20

const char * const menu[MENU_ACTIONS] = {
    "Singleplayer",
    "Multiplayer",
    "Game 3",
    "Game 4",
};

static void move_cursor(const int menu_row)
{
    oled_clear_area(0, MENU_ACTIONS, 0, CURSOR_SPACE);

    oled_set_column(0);
    oled_set_page(menu_row);
    oled_printf(CURSOR);
}

void menu_select(int * current_menu_choice)
{
    joystick_status joystick = joystick_get_status();  

    switch(joystick.dir)
    {
	case UP:
	    --(*current_menu_choice);
	    break;
	case DOWN:
	    ++(*current_menu_choice);
	    break;
	default:
	    break;
    }
    
    if (*current_menu_choice > MENU_ACTIONS - 1) {
	*current_menu_choice = 0;
    }
    else if (*current_menu_choice < 0) {
	*current_menu_choice = MENU_ACTIONS - 1;
    }

    move_cursor(*current_menu_choice);

}

void menu_init(void)
{
    move_cursor(0);

    for (int i = 0; i < MENU_ACTIONS; i++)
    {
	// Make space for the cursor
        oled_set_column(CURSOR_SPACE);
        oled_set_page(i);
        oled_printf(menu[i]);
    }
}

