#ifndef IR_DRIVER_H_INCLUDED
#define IR_DRIVER_H_INCLUDED
#include <stdint.h>

uint8_t ir_check_goal(void);
uint8_t ir_get_goal_count(void);
void ir_enable(void);
void ir_disable(void);

#endif