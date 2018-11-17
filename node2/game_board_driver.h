#ifndef GAME_BOARD_DRIVER_H_INCLUDED
#define GAME_BOARD_DRIVER_H_INCLUDED
#include "can/CAN_driver.h"
#include "dc_motor/motor_box_driver.h"

void game_board_init(void);
void game_board_reset(void);
void game_board_handle_msg(CAN_message * msg);
void game_board_shoot(uint8_t r_pressed, uint8_t joystick_dir);
uint8_t game_board_check_goal(void);
void game_board_transmit_goal();

#endif