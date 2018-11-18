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

#define N_GAMES 4
#define N_CHARS 4
#define N_DIFFS 3
#define N_LIVES 2

#define N_IO_DATA_LENGTH 8


static const char * menu_games[N_GAMES] = {
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

static uint8_t ticks;
static uint16_t score;
static uint8_t n_lives;

static CAN_message * io_msg;

static joystick_status joystick;
static slider_status slider;
static buttons_status buttons; 

static void send_reset_msg(uint8_t player_diff)
{
	CAN_message * msg_reset;
	msg_reset = CAN_message_constructor(ID_RESET_GB, 2);
	msg_reset->data[0] = 1; // Reset (true/false)
	msg_reset->data[1] = player_diff;
	CAN_send(msg_reset);
	CAN_message_destructor(msg_reset);
}

static void fetch_io_values(void)
{
	usb_multifunction_buttons_get_status(&buttons);
	joystick_get_status(&joystick);
	slider_get_status(&slider);
}

static void populate_io_msg(void)
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
	/*// Clear timer register
    TCNT0 = 0;*/

    // Set prescaler to 1024
    TCCR0 = 0;
    TCCR0 = (1 << CS02) | (1 << CS00);
}

uint8_t fsm_play_game(const uint8_t player_diff)
{
	score = 0;
	n_lives = N_LIVES;
	uint8_t game_over = 0;
	fetch_io_values();

	game_timer_init();

	// Enable game timer overflow interrupt
	SET_BIT(TIMSK, TOIE0);

	// Game loop
	while(1)
	{
		io_msg = CAN_message_constructor(ID_IO, N_IO_DATA_LENGTH);
		fetch_io_values();
		populate_io_msg();
		//printf("%u\n", io_msg->id);
		CAN_send(io_msg);
		CAN_message_destructor(io_msg);


		if (buttons.left)
			break;

		menu_display_game_state(score, n_lives, menu_diffs[player_diff]);

		_delay_ms(100);

		if (!n_lives)
		{	
			game_over = 1;
			break;
		}
	}

	CAN_message_destructor(io_msg);

	// Disable game timer interrupt when game is done
	CLEAR_BIT(TIMSK, TOIE0);

	return game_over;
}



void fsm_main_loop(void)
{
	game_state state = MENU_GAMES;

	char * player_img = "mario64";
	uint8_t player_diff = DIFF_EASY;

	while(1)
	{
		switch(state)
	    {
	        case MENU_GAMES:
	        {
	            menu_draw_options(menu_games, N_GAMES);
	            uint8_t result = menu_select_option(N_GAMES);
	            switch(result)
	            {
	            	case 0:
	            		state = PLAY;
	            		break;
	            	case 1:
	            		state = LEADERBOARD;
    					oled_clear_screen();
	        			menu_leaderboard();
	            		break;
	            	case 2:
	            		state = MENU_CHARACTERS;
	            		break;
	            	case 3:
	            		state = MENU_DIFFICULTY;
	            		break;
	            	case 99:
	            		state = MENU_GAMES;
	            		break;
	            	default:
	            		return;
	            }
	            break;
	        }
	        case PLAY:
	        {
				send_reset_msg(player_diff);

	        	oled_display_image(player_img, 64, 0, 0);
	        	//playsong(player_song)
	        	_delay_ms(2000); // Wait for image to load

				if (fsm_play_game(player_diff))
				{	
					state = GAME_OVER;
    				oled_clear_screen();
					break;
				}
				state = MENU_GAMES;
	        	break;
	        }
	        case GAME_OVER:
	        {
	        	menu_save_score(score);
	        	menu_game_over(score);

				buttons_status buttons;
				usb_multifunction_buttons_get_status(&buttons);

	        	if (buttons.left)
	        		state = MENU_GAMES;

	        	break;
	        }
	        case LEADERBOARD:
	        {	
	        	_delay_ms(100);
	        	fetch_io_values();
	        	if (buttons.left || joystick.dir == LEFT)
	        		state = MENU_GAMES;
	        	break;
	        }
	        case MENU_CHARACTERS:
	        {
	            menu_draw_options(menu_chars, N_CHARS);
	            uint8_t result = menu_select_option(N_CHARS);
	            switch(result)
	            {
	            	case 0:
		            	state = MENU_GAMES;
		            	player_img = "mario64";
		            	break;
	            	case 1:
	            		state = MENU_GAMES;
	            		player_img = "dv64";
		            	break;
	            	case 2:
	            		state = MENU_GAMES;
	            		player_img = "kimk64";
		            	break;
	            	case 3:
	            		state = MENU_GAMES;
	            		player_img = "kimy64";
		            	break;
		            case 99:
	            		state = MENU_GAMES;
	            		break;
		            default:
		            	return;
	            }
	            break;
	        }
	        case MENU_DIFFICULTY:
	        {
	        	menu_draw_options(menu_diffs, N_DIFFS);
	        	uint8_t result = menu_select_option(N_DIFFS);
	        	switch(result)
	        	{
	        		case 0:
	        			player_diff = DIFF_EASY;
	        			state = MENU_GAMES;
	        			break;
	        		case 1:
	        			player_diff = DIFF_MEDIUM;
	        			state = MENU_GAMES;
	        			break;
	        		case 2:
	        			player_diff = DIFF_HARD;
	        			state = MENU_GAMES;
	        			break;
	        		case 99:
	            		state = MENU_GAMES;
	            		break;
	        		default:
	        			return;
	        	}
	        	break;
	        }
	        default:
	        	return;
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
}

// CAN receive interrupt
ISR(CAN_REC_INT_VECT)
{
	CAN_message * receive_msg = CAN_receive();
	if (receive_msg->id == ID_GOAL)
		--n_lives;
	else
		printf("%d\n", receive_msg->id);
}