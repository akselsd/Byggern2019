#ifndef CAN_DRIVER_INCLUDED_H
#define CAN_DRIVER_INCLUDED_H
#include <stdint.h>

typedef struct CAN_message_struct
{
	//TODO change datatypes?
	uint8_t id;
	uint8_t * data;
	uint8_t length;

} CAN_message;

void CAN_init(void);
CAN_message * CAN_message_constructor(uint8_t id, uint8_t length);
void CAN_message_destructor(CAN_message * msg);
void CAN_send(CAN_message * message);
void CAN_receive(CAN_message * message);
#endif
