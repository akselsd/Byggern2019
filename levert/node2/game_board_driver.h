#ifndef GAME_BOARD_DRIVER_H_INCLUDED
#define GAME_BOARD_DRIVER_H_INCLUDED
#include "can/CAN_driver.h"

/* Init module */
void game_board_init(void);

/* Reset game state and initialize a new game with
    @param player_diff difficulty
 */
void game_board_reset(uint8_t player_diff);

/* Handle input from the multifunction card */
void game_board_handle_msg(CAN_message * msg);

/* Shoot solenoid if the button is pressed or the joystick is pointing up
  @param r_pressed, the current status of the right button
  @param joystick_dir, the current direction of the joystick */
void game_board_shoot(uint8_t r_pressed, uint8_t joystick_dir);

/* Chech if a goal is detected
    @return 0 if no goal is detected */
uint8_t game_board_check_goal(void);

/* Transmit CAN message to node1 notifying about a goal */
void game_board_transmit_goal();

#endif