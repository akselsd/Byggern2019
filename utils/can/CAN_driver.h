#ifndef CAN_DRIVER_INCLUDED_H
#define CAN_DRIVER_INCLUDED_H
#include <stdint.h>

typedef struct CAN_message_struct
{
	//TODO change datatypes?
	uint8_t id;
	uint8_t data[8];
	uint8_t length;

} CAN_message;

void CAN_init(void);
void CAN_send(CAN_message * message);
void CAN_receive(CAN_message * message);
#endif
