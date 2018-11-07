#include "bit_macros.h"
#include "system_constants.h"
#include "CAN_driver.h"
#include "MCP_driver.h"
#include "MCP2515.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

/* Bits in TXBnCTRL */
#define TXREQ 3

/* TX buffer address offsets from TXBnCTRL / RXBnCTRL */
#define SIDH_OFFSET 1
#define SIDL_OFFSET 2
#define DLC_OFFSET 5
#define DLC_MASK 15
#define D_OFFSET 6

/* Bits in RXB0CTRL */
#define BUKT 2
#define RXM0 5

/* Bits in CANINTE*/
#define RX0IE 0

/* Bits in CANINTF */
#define RX0IF 0
#define RX1IF 1

/* Bits in TXBnSIDL / RXBnSIDL*/
#define SID0 5

void CAN_init(void)
{
	//TODO will later be expanded to work with multiple modes
	MCP_init();
	MCP_write(MCP_RXB0CTRL, (3 << RXM0));
	MCP_write(MCP_RXB0CTRL, (1 << BUKT));

	// Enable interrupt for received message
	MCP_bit_modify(MCP_CANINTE, (1 << RX0IE), 1);

	MCP_bit_modify(MCP_CANINTF, (1 << RX0IF), 0);
}

CAN_message * CAN_message_constructor(uint8_t id, uint8_t length)
{
	CAN_message * msg = malloc(sizeof(CAN_message));
	
	if (msg == NULL)
		return NULL;
	
	msg->id = id;
	msg->data = malloc(sizeof(uint8_t) * length);
	
	if (msg->data == NULL){
		free(msg);
		return NULL;
	}
	msg->length = length;
	
	return msg;
}

void CAN_message_destructor(CAN_message * msg)
{
	if (msg != NULL){
		if (msg->data != NULL)
			free(msg->data);
		free(msg);
	}
}

void CAN_send(CAN_message * message)
{
	/* Check if buffer is ready */
	while(READ_BIT(MCP_read(MCP_TXB0CTRL), TXREQ)){
		printf("MCP: %d\n", MCP_read(MCP_TXB0CTRL));
		printf("MCP IS TRYING TO SEND\n");
		printf("MSG ID: %d\n", message->id);
		_delay_ms(50);
	}

	/* TODO: Assign priority? */

	MCP_write(MCP_TXB0CTRL + SIDH_OFFSET, 0);
	MCP_write(MCP_TXB0CTRL + SIDL_OFFSET, message->id << SID0);
	MCP_write(MCP_TXB0CTRL + DLC_OFFSET, message->length);
	MCP_write_n(MCP_TXB0CTRL + D_OFFSET, message->data, message->length);
	MCP_request_to_send();
}

CAN_message * CAN_receive(void)
{
	// Wait for RX0 interrupt flag
	while(!READ_BIT(MCP_read(MCP_CANINTF), RX0IF)){
		printf("MCP: %d\n", MCP_read(MCP_CANINTF));
		printf("MCP IS TRYING TO READ\n");
		_delay_ms(50);
	}

	uint8_t id = (MCP_read(MCP_RXB0CTRL + SIDL_OFFSET)) >> SID0;
	uint8_t length = (MCP_read(MCP_RXB0CTRL + DLC_OFFSET)) & DLC_MASK;
	CAN_message * msg = CAN_message_constructor(id, length);
	
	MCP_read_n(MCP_RXB0CTRL + D_OFFSET, msg->data, msg->length);

	// Clear flag bit
	MCP_bit_modify(MCP_CANINTF, (1 << RX0IF), 0);
	return msg;
}