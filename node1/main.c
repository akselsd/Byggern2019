#include "system_constants.h"
#define BAUDRATE 9600
#define UBRR F_CPU/16/BAUDRATE - 1
#include <stdio.h> //Trengs denne?
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
#define N_LIVES 5

#define N_IO_DATA_LENGTH 8


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

    /* Set prescaler to 1024 */
    TCCR0 = (1 << CS02) | (1 << CS00);
}

typedef enum transmit_step_enum
{
	TRANSMIT_JOYSTICK = 0,
	TRANSMIT_SLIDER,
	TRANSMIT_BUTTONS,
	TRANSMIT_REQ_GOAL,
} transmit_step;


void init_all(void)
{
	//check further SRAM
	SET_BIT(MCUCR, SRE);

	uart_init(UBRR);
	usb_multifunction_buttons_init();
	usb_multifunction_joystick_init();
	menu_init();
	oled_init();
	CAN_init();
}

static uint8_t ticks = 0;
static uint8_t score = 0;
static uint8_t n_lives = N_LIVES;
static uint8_t curr_transmit_step = TRANSMIT_JOYSTICK;
static CAN_message * io_msg;

void play_game(uint8_t player_diff)
{
	score = 0;
	n_lives = N_LIVES;

	program_timer_init();

	/* Enable overflow interrupt */
	SET_BIT(TIMSK, TOIE0);

	joystick_status joystick;
	slider_status slider;
	buttons_status buttons; 

	io_msg = CAN_message_constructor(ID_IO, N_IO_DATA_LENGTH);

	while(1)
	{
		usb_multifunction_buttons_get_status(&buttons);
		joystick_get_status(&joystick);
		slider_get_status(&slider);

		if (buttons.left)
			break;

		io_msg->data[0] = joystick.x;
		io_msg->data[1] = joystick.y;
		io_msg->data[2] = joystick.pressed;
		io_msg->data[3] = joystick.dir;

		io_msg->data[4] = buttons.left;
		io_msg->data[5] = buttons.right;

		io_msg->data[6] = slider.left;
		io_msg->data[7] = slider.right;

		CAN_send(io_msg);

		menu_display_game_state(score, n_lives, menu_diffs[player_diff]);

		_delay_ms(100);
	}

	/* Disable interrupt */
	CLEAR_BIT(TIMSK, TOIE0);
	CAN_message_destructor(io_msg);
}

ISR(TIMER0_OVF_vect)
{
	//CAN_send(io_msg);

	if (++ticks == 30) {
		ticks = 0;
		++score;
	}
}

void main_action_loop(void)
{
	game_state state = MENU_GAMES;
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
	        	_delay_ms(2000);
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
	oled_clear_screen();
	main_action_loop();
}

ISR(INT0_vect)
{
	printf("Received interrupt.\n");
}
