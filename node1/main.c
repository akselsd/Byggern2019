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
#define N_DIFFS 4

#define N_PROG_TICKS 200

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
	"Beginner",
	"Amateur",
	"Professional",
	"World class",
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

	// Currently does nothing
	//joystick_calibrate_joystick();

	usb_multifunction_buttons_init();
	usb_multifunction_joystick_init();

	menu_init();

	oled_init();
	CAN_init();
	program_timer_init();
	//CAN_init_test_loopback_mode();
}



void control_game_board(void)
{
	uint8_t score = 0;
	uint8_t ticks = 0;
	while(true)
	{

		if (program_timer_check(N_PROG_TICKS))
		{
			program_timer_restart();

				

			joystick_transmit_position();
			//_delay_ms(20);
			slider_transmit_position();
			//_delay_ms(20);
			usb_multifunction_buttons_transmit_status();
			//printf("%d\n", TCNT0);
			//_delay_ms(100);
			if (++ticks == 30){
				ticks = 0;
				menu_display_score(++score);
			}
		}
		//_delay_ms(10);
	}
}

void main_action_loop(void)
{
	game_state state = MENU_GAMES;
	uint16_t score = 0;

	while(1)
	{
		//printf("State: %d\n", state);
		switch(state)
	    {
	        case MENU_GAMES:
	        {
	            menu_draw_options(menu_games, 4);
	            uint8_t result = menu_select_option(4);
	            //printf("Result %d\n", result);
	            switch(result)
	            {
	            	case 0:
	            		state = MENU_CHARACTERS;
	            		//TODO: implenet selected char variable.
	            		//oled_display_image("mario64", 64, 0, 0);
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
	            	default:
	            		return;
	            }
	            break;
	        }
	        case MENU_CHARACTERS:
	        {
	            menu_draw_options(menu_chars, 4);
	            uint8_t result = menu_select_option(4);
	            switch(result)
	            {
	            	case 0:
		            	state = PLAY; //change to MENU_GAMES
		            	oled_display_image("mario64", 64, 0, 0);
		            	break;
	            	case 1:
	            		state = PLAY;
	            		oled_display_image("dv64", 64, 0, 0);
		            	break;
	            	case 2:
	            		state = PLAY;
	            		oled_display_image("kimk64", 64, 0, 0);
		            	break;
	            	case 3:
	            		oled_display_image("kimy64", 64, 0, 0);
	            		state = PLAY;
		            	break;
		            //implement default (?)
		            default:
		            	return;
	            }
		        _delay_ms(1000);
	            break;
	        }
	        case MENU_DIFFICULTY:
	        {
	        	menu_draw_options(menu_diffs, 4);
	        	uint8_t result = menu_select_option(4);
	        	switch(result)
	        	{
	        		case 0:
	        			//difficulty=easy
	        			state = MENU_GAMES; //change to MENU_GAMES
	        			break;
	        		case 1:
	        			//difficulty=amateur
	        			state = MENU_GAMES; //change to MENU_GAMES
	        			break;
	        		case 2:
	        			//difficulty=professional
	        			state = MENU_GAMES; //change to MENU_GAMES
	        			break;
	        		case 3:
	        			//difficulty=world class
	        			state = MENU_GAMES; //change to MENU_GAMES
	        			break;
	        		default:
	        			return;
	        	}
	        	break;
	        }
	        case PLAY:
		        menu_display_score(0);
				control_game_board();
				//_delay_ms(50);
				//++score;
	        	break;
	        //implement default (?)
	        default:
	        	return;
	    }
	    //_delay_ms(20);
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