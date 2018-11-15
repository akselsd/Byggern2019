#include "system_constants.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include "menu.h"
#include "oled.h"
#include "joystick.h"

#define CURSOR "->"
#define CURSOR_SPACE 16
// 4800 ticks per sec
#define MENU_DELAY 2400

static joystick_status prev;
static joystick_status curr;


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

void menu_timer_restart(void)
{
    /* Reset timer register */
    TCNT3 = 0;
}

static void timer_init(void)
{
    /* Normal operation */
    TCCR3A = 0;

    /* 1024 prescalar */
    TCCR3B = (1 << CS10 | (1 << CS12));

    menu_timer_restart();
}

void menu_init(void)
{
    timer_init();
    joystick_get_status(&curr);
    joystick_get_status(&prev);
}

uint8_t menu_timer_check(void)
{
    return TCNT3 > MENU_DELAY;
}

int menu_select_option(uint8_t n_options)
{
    int current_menu_choice = 0;
    move_cursor(current_menu_choice);


    while (true)
    {
        joystick_get_status(&curr);

        if ((curr.dir != prev.dir) ||
            (curr.pressed != prev.pressed) ||
            (menu_timer_check()))
        {

            prev = curr;
            menu_timer_restart();

            if (curr.pressed)
                return current_menu_choice;

            switch(curr.dir)
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
        }
        
    }
}

void menu_display_score(uint16_t score)
{
    char sc[]="SCORE ";
    char str[10];
    strcpy(str, sc);    
    sprintf(&str[6], "%u", score);
    oled_set_column(65);
    oled_set_page(4);

    oled_printf(str);
}

