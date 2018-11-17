#ifndef CAN_DRIVER_INCLUDED_H
#define CAN_DRIVER_INCLUDED_H
#include <stdint.h>

typedef enum CAN_message_id_enum
{
	ID_JOYSTICK = 0,
	ID_SLIDERS,
	ID_BUTTONS,
	ID_UPDATE_FSM,
	ID_RESET_GB,
	ID_GOAL,
	ID_REQ_GOAL,
} CAN_message_id;

typedef struct CAN_message_struct
{
	uint8_t id;
	uint8_t * data;
	uint8_t length;

} CAN_message;

void CAN_init_test_loopback_mode(void);
void CAN_init(void);
CAN_message * CAN_message_constructor(uint8_t id, uint8_t length);
void CAN_message_destructor(CAN_message * msg);
void CAN_send(CAN_message * message);
CAN_message * CAN_receive(void);
#endif
