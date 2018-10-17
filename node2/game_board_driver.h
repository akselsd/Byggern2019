#ifndef GAME_BOARD_DRIVER_H_INCLUDED
#define GAME_BOARD_DRIVER_H_INCLUDED
#include "can/CAN_driver.h"

void game_board_receive(CAN_message * msg);

#endif