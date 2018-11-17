#include "system_constants.h"
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "bit_macros.h"
#include "can/CAN_driver.h"
#include "tests/tests.h"
#include "tests/sram_test.h"
#include "uart/uart.h"
#include "usb_multifunction_card/buttons.h"
#include "usb_multifunction_card/joystick.h"
#include "usb_multifunction_card/usb_multifunction_card_io.h"
#include "usb_multifunction_card/oled.h"
#include "usb_multifunction_card/slider.h"
#include "usb_multifunction_card/menu.h"

#define N_GAMES 4
#define N_CHARS 4
#define N_DIFFS 3

#define N_PROG_TICKS 200
#define N_LIVES 5

typedef enum game_state_enum
{
    MENU_GAMES,
    MENU_CHARACTERS,
    HIGHSCORE,
    MENU_DIFFICULTY,
    PLAY,
    GAME_OVER
} game_state;

static const char * menu_games[N_GAMES] = {
    "Play",
    "Highscore",
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

void program_timer_restart(void)
{
    TCNT0 = 0;
}

void program_timer_init(void)
{
    TCCR0 = 0;

    TCCR0 = (1 << CS10 | (1 << CS12));
    program_timer_restart();
}


uint8_t program_timer_check(uint8_t value)
{
	return TCNT0 >= value;
}


void init_all(void)
{
	SET_BIT(MCUCR, SRE);

	uart_init(UBRR);
	usb_multifunction_buttons_init();
	usb_multifunction_joystick_init();
	menu_init();
	oled_init();
	CAN_init();
	program_timer_init();
}

void receive_goal(uint8_t * n_lives)
{
	CAN_message * msg_req_goal = CAN_message_constructor(ID_REQ_GOAL, 0);
	CAN_send(msg_req_goal);
	CAN_message_destructor(msg_req_goal);


	CAN_message * msg_check_goal = CAN_receive();
	if((msg_check_goal->id == ID_GOAL) && (msg_check_goal->data[0]))
	{	
		--*n_lives;
	}
	CAN_message_destructor(msg_check_goal);	
}


void play_game(uint8_t player_diff)
{
	uint8_t ticks = 0;
	uint8_t score = 0;
	uint8_t n_lives = N_LIVES;

	buttons_status buttons;

	while(1)
	{
		usb_multifunction_buttons_get_status(&buttons);

		if (buttons.left)
			return;

		if (program_timer_check(N_PROG_TICKS))
		{
			program_timer_restart();

			joystick_transmit_position();
			slider_transmit_position();
			receive_goal(&n_lives);
			usb_multifunction_buttons_transmit_status();

			menu_display_game_state(score, n_lives, menu_diffs[player_diff]);

			if (++ticks == 30) {
				ticks = 0;
				++score;
			}
		}
	}
}

void main_action_loop(void)
{
	game_state state = MENU_GAMES;
	uint16_t score = 0;
	char * player_img = "mario64";
	CAN_message * msg_reset;
	uint8_t player_diff = 0;


	while(1)
	{
		switch(state)
	    {
	        case MENU_GAMES:
	        {
	            menu_draw_options(menu_games, N_GAMES);
	            uint8_t result = menu_select_option(4);
	            switch(result)
	            {
	            	case 0:
	            		state = PLAY;
	            		break;
	            	case 1:
	            		state = HIGHSCORE;
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
	        case MENU_CHARACTERS:
	        {
	            menu_draw_options(menu_chars, N_CHARS);
	            uint8_t result = menu_select_option(4);
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
	        	uint8_t result = menu_select_option(4);
	        	switch(result)
	        	{
	        		case 0:
	        			player_diff = 0;
	        			state = MENU_GAMES;
	        			break;
	        		case 1:
	        			player_diff = 1;
	        			state = MENU_GAMES;
	        			break;
	        		case 2:
	        			player_diff = 2;
	        			state = MENU_GAMES;
	        			break;
	        		case 3:
	        			player_diff = 3;
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
	        case PLAY:
	        	msg_reset = CAN_message_constructor(ID_RESET_GB, 0);
	        	CAN_send(msg_reset);
				CAN_message_destructor(msg_reset);

	        	oled_display_image(player_img, 64, 0, 0);
	        	//playsong(player_song)
	        	_delay_ms(500);
				play_game(player_diff);
				state = MENU_GAMES;
	        	break;
	        default:
	        	return;
	    }
    }
}

int main()
{
	/* Initialize system */
	init_all();
	_delay_ms(1000);
	printf("\n\n\nInitialized\n");
	joystick_status s;
	oled_clear_screen();
	uint8_t oldscore;

	main_action_loop();
}