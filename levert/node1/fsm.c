#include "system_constants.h"
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "fsm.h"
#include "bit_macros.h"
#include "can/CAN_driver.h"
#include "usb_multifunction_card/buttons.h"
#include "usb_multifunction_card/joystick.h"
#include "usb_multifunction_card/usb_multifunction_card_io.h"
#include "usb_multifunction_card/oled.h"
#include "usb_multifunction_card/slider.h"
#include "usb_multifunction_card/menu.h"

#define CAN_REC_INT_VECT INT0_vect

#define N_TITLES 4
#define N_CHOICES 4 //5
#define N_CHARS 4
#define N_DIFFS 3
#define N_LIVES 2

#define N_IO_DATA_LENGTH 8

typedef enum game_state_enum
{
    MAIN_MENU = 0,
    LEADERBOARD,
    MENU_CHARACTERS,
    MENU_DIFFICULTY,
    PLAY,
    GAME_OVER,
} game_state;

static const char * titles[N_TITLES] = {
	"MAIN MENU",
	"LEADERBOARD",
	"CHARACTERS",
	"DIFFICULTY",
};

static const char * main_menu[N_CHOICES] = {
    "Play",
    "Leaderboard",
    "Character",
    "Difficulty",
};

static const char * menu_chars[N_CHARS] = {
    "Mario",
    "Darth Vader",
    "Kim Kardashian",
    "Kim Young-Un",
};

static const char * menu_diffs[N_DIFFS] = {
	"Easy",
	"Medium",
	"Hard",
};

static volatile uint8_t ticks;
static volatile uint16_t score;
static volatile uint8_t n_lives;
static volatile CAN_message io_msg;

static joystick_status joystick;
static slider_status slider;
static buttons_status buttons; 

static void send_reset_msg(uint8_t player_diff)
{
	CAN_message msg_reset;
	msg_reset.id = ID_RESET_GB;
	msg_reset.data[0] = 1;
	msg_reset.data[1] = player_diff;
	CAN_send(&msg_reset);
}

static void fetch_io_values(void)
{
	usb_multifunction_buttons_get_status(&buttons);
	joystick_get_status(&joystick);
	slider_get_status(&slider);
}

static void populate_io_msg(CAN_message * io_msg)
{
	io_msg->data[0] = joystick.x;
	io_msg->data[1] = joystick.y;
	io_msg->data[2] = joystick.pressed;
	io_msg->data[3] = joystick.dir;

	io_msg->data[4] = buttons.left;
	io_msg->data[5] = buttons.right;

	io_msg->data[6] = slider.left;
	io_msg->data[7] = slider.right;
}

static void game_timer_init(void)
{
    // Set prescaler to 1024
    TCCR0 = 0;
    TCCR0 = (1 << CS02) | (1 << CS00);
}

static uint8_t fsm_play_game(const uint8_t player_diff)
{
	score = 0;
	n_lives = N_LIVES;
	uint8_t game_over = 0;

	io_msg.id = ID_IO;

	fetch_io_values();

	game_timer_init();

	// Enable game timer overflow interrupt
	SET_BIT(TIMSK, TOIE0);

	// Game loop
	while(1)
	{
		fetch_io_values();
		populate_io_msg(&io_msg);

		if (buttons.left)
			break;

		menu_display_game_state(score, n_lives, menu_diffs[player_diff]);

		if (!n_lives)
		{	
			game_over = 1;
			break;
		}
	}
	// Disable game timer interrupt when game is done
	CLEAR_BIT(TIMSK, TOIE0);
	return game_over;
}


void fsm_main_loop(void)
{
	game_state state = MAIN_MENU;

	char * player_img = "mario64";
	uint8_t player_diff = DIFF_MEDIUM;

	while(1)
	{
		switch(state)
	    {
	        case MAIN_MENU:
	        {	
	            menu_draw_options(main_menu, N_CHOICES, titles[MAIN_MENU]);
	            uint8_t result = menu_select_option(N_CHOICES);
	            switch(result)
	            {
	            	case 0:
	            		state = PLAY;
	            		break;
	            	case 1:
    					oled_clear_screen();
	            		state = LEADERBOARD;
	            		break;
	            	case 2:
	            		oled_clear_screen();
	            		state = MENU_CHARACTERS;
	            		break;
	            	case 3:
	            		state = MENU_DIFFICULTY;
	            		break;
	            	case 99:
	            		state = MAIN_MENU;
	            		break;
	            	default:
	            		break;
	            }
	            break;
	        }
	        case PLAY:
	        {
			send_reset_msg(player_diff);
	        	oled_display_image(player_img, 64, 0, 0);
	        	_delay_ms(2000); // Wait for image to load

				if (fsm_play_game(player_diff))
				{
					// GAME OVER
	        		menu_game_over(score);
					state = GAME_OVER;
					break;
				}
				// PLAYER QUIT
				state = MAIN_MENU;
	        	break;
	        }
	        case GAME_OVER:
	        {
	        	fetch_io_values();
	        	if (buttons.left)
	        		state = MAIN_MENU;
	        	if (buttons.right)
	        	{
	        		menu_save_score(score);
    				_delay_ms(4000);
	        		state = MAIN_MENU;
	        	}
	        	break;
	        }
	        case LEADERBOARD:
	        {	
	        	menu_leaderboard(main_menu[LEADERBOARD]);
	        	fetch_io_values();
	        	if (buttons.left || joystick.dir == LEFT)
	        		state = MAIN_MENU;
	        	break;
	        }
	        case MENU_CHARACTERS:
	        {
	            menu_draw_options(menu_chars, N_CHARS, titles[MENU_CHARACTERS]);
	            uint8_t result = menu_select_option(N_CHARS);
	            switch(result)
	            {
	            	case 0:
		            	player_img = "mario64";
		            	break;
	            	case 1:
	            		player_img = "dv64";
		            	break;
	            	case 2:
	            		player_img = "kimk64";
		            	break;
	            	case 3:
	            		player_img = "kimy64";
		            	break;
	            	case 99:
	            		state = MAIN_MENU;
	            		break;
		            default:
		            	break;
	            }
	            state = MAIN_MENU;
	            break;
	        }
	        case MENU_DIFFICULTY:
	        {
	        	menu_draw_options(menu_diffs, N_DIFFS, titles[MENU_DIFFICULTY]);
	        	uint8_t result = menu_select_option(N_DIFFS);
	        	switch(result)
	        	{
	        		case 0:
	        			player_diff = DIFF_EASY;
	        			break;
	        		case 1:
	        			player_diff = DIFF_MEDIUM;
	        			break;
	        		case 2:
	        			player_diff = DIFF_HARD;
	        			break;
	        		case 99:
	            		state = MAIN_MENU;
	            		break;
	        		default:
	        			break;
	        	}
           		state = MAIN_MENU;
	        	break;
	        }
	    default:
	       	break;
	    }
    }
}

// Game score timer
ISR(TIMER0_OVF_vect)
{
	if (++ticks == 30) {
		ticks = 0;
		++score;
	}
	CAN_send(&io_msg);

}

// CAN receive interrupt
ISR(CAN_REC_INT_VECT)
{
	CAN_message receive_msg;
	CAN_receive(&receive_msg);
	if (receive_msg.id == ID_GOAL)
		--n_lives;
	else
		printf("%d\n", receive_msg.id);
}
