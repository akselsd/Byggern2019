#ifndef GAME_BOARD_DRIVER_H_INCLUDED
#define GAME_BOARD_DRIVER_H_INCLUDED
#include "can/CAN_driver.h"

void game_board_handle_msg(CAN_message * msg);
void game_board_init(void);

#endif