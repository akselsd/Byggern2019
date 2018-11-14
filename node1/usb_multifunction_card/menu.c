#include "system_constants.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "oled.h"
#include "joystick.h"

#define CURSOR "->"
#define CURSOR_SPACE 16


static void display_character(const char * imgname)
{
    oled_display_image(imgname, 64, 0, 0);
}

static void move_cursor(const int menu_row)
{
    oled_clear_area(0, 7, 0, CURSOR_SPACE);
    oled_set_column(0);
    oled_set_page(menu_row);
    oled_printf(CURSOR);
}

void menu_draw_options(const char ** options, uint8_t n_options)
{
    oled_clear_screen();
    for (int i = 0; i < n_options; i++)
    {
        oled_set_column(CURSOR_SPACE);
        oled_set_page(i);
        oled_printf(options[i]);
    }
}

int menu_select_option(uint8_t n_options)
{
    int current_menu_choice = 0;
    while (true)
    {
        static joystick_status status;
        joystick_get_status(&status);

        if (status.pressed)
            return current_menu_choice;

        switch(status.dir)
        {
            case UP:
                --current_menu_choice;
                break;
            case DOWN:
                ++current_menu_choice;
                break;
            default:
                break;
        }

        if (current_menu_choice > n_options - 1) {
           current_menu_choice = 0;
        }

        else if (current_menu_choice < 0) {
           current_menu_choice = n_options - 1;
        }

        move_cursor(current_menu_choice);
        _delay_ms(300);
    }
}

void menu_display_score(uint8_t score)
{
    char sc[]="SCORE ";
    char str[10];
    strcpy(str, sc);    
    sprintf(&str[6], "%u", score);
    oled_set_column(65);
    oled_set_page(4);

    oled_printf(str);
}

