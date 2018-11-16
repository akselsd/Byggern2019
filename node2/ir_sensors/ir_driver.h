#ifndef IR_DRIVER_H_INCLUDED
#define IR_DRIVER_H_INCLUDED
#include <stdint.h>

void ir_init(void);
uint8_t ir_check_block(void);
void ir_enable(void);
void ir_disable(void);

#endif