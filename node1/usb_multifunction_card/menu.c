#include "system_constants.h"
#include "util/delay.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include "menu.h"
#include "oled.h"
#include "joystick.h"

#define CURSOR ">"
#define CURSOR_SPACE 16
// 4800 ticks per sec
#define MENU_DELAY 2400

#define TEXT_LEFT_ALIGN 65
#define DIFF_POS_PAGE 0
#define LIVES_POS_PAGE 1
#define SCORE_POS_PAGE 4
#define QUIT_POS_PAGE 7

static joystick_status prev;
static joystick_status curr;

static void display_character(const char * imgname)
{
    oled_display_image(imgname, 64, 0, 0);
}

static void move_cursor(const int8_t menu_row)
{
    oled_clear_area(0, 7, 0, CURSOR_SPACE);
    oled_set_column(0);
    oled_set_page(menu_row);
    oled_printf(CURSOR);
}

void menu_draw_options(const char ** options, uint8_t n_options)
{
    oled_clear_screen();
    for (int8_t i = 0; i < n_options; i++)
    {
        //Tight between cursor and text. Trying CURSOR_SPACE +1 -Helped!
        oled_set_column(CURSOR_SPACE+1);
        //top of letters appear on bottom. Trying i+1 - did not help! Went too far.
        oled_set_page(i);
        oled_printf(options[i]);
    }
}

static void menu_timer_restart(void)
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

static uint8_t menu_timer_check(void)
{
    return TCNT3 > MENU_DELAY;
}

uint8_t menu_select_option(uint8_t n_options)
{
    int8_t current_menu_choice = 0;
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

            if (curr.pressed || curr.dir == RIGHT)
                return current_menu_choice;

            switch(curr.dir)
            {
                case UP:
                    --current_menu_choice;
                    break;
                case DOWN:
                    ++current_menu_choice;
                    break;
                case LEFT:
                    return 99;
                default:
                    break;
            }

            if (current_menu_choice > n_options - 1) {
               current_menu_choice = 0;
            }

            else if (current_menu_choice < 0) {
               current_menu_choice = n_options - 1;
            }
            printf("%d\n", current_menu_choice);
            move_cursor(current_menu_choice);
        }

    }
}

void menu_display_game_state(uint16_t score, uint8_t n_lives, char * player_diff)
{   
    oled_set_column(TEXT_LEFT_ALIGN);
    oled_set_page(DIFF_POS_PAGE);
    oled_printf(player_diff);

    oled_set_column(TEXT_LEFT_ALIGN);
    oled_set_page(LIVES_POS_PAGE);
    oled_printf("LIVES:");

    char str_lives[10];
    sprintf(&str_lives[0], "%u", n_lives);
    oled_set_column(TEXT_LEFT_ALIGN);
    oled_set_page(LIVES_POS_PAGE + 1);
    oled_printf(str_lives);

    oled_set_column(TEXT_LEFT_ALIGN);
    oled_set_page(SCORE_POS_PAGE);
    oled_printf("SCORE:");

    oled_set_column(TEXT_LEFT_ALIGN);
    oled_set_page(SCORE_POS_PAGE + 1);
    char str_score[10];
    sprintf(&str_score[0], "%u", score);
    oled_printf(str_score);

    oled_set_column(TEXT_LEFT_ALIGN);
    oled_set_page(QUIT_POS_PAGE);
    oled_printf("QUIT: L");
}

