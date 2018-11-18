#include "bit_macros.h"
#include "system_constants.h"
#include "CAN_driver.h"
#include "MCP_driver.h"
#include "MCP2515.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>


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
#define TX0IE 2

/* Bits in CANINTF */
#define RX0IF 0
#define RX1IF 1

/* Bits in TXBnSIDL / RXBnSIDL*/
#define SID0 5

void CAN_init_test_loopback_mode(void)
{
	MCP_init();
	_delay_ms(200);
	MCP_reset();
	_delay_ms(200);
	MCP_write(MCP_CANCTRL, MODE_LOOPBACK);
	printf("## INITING CAN TO TESTING MODE\n ##");

	while(1)
	{
		CAN_message * msg = CAN_message_constructor(2, 3);
		msg->data[0] = 2;
		msg->data[1] = 9;
		msg->data[2] = 254;
		CAN_send(msg);
		CAN_message_destructor(msg);
		_delay_ms(500);
		CAN_message * rec = CAN_receive();
		printf("%d %d\n",
			rec->id,
			rec->length);
		for (int i = 0; i < rec->length; ++i)
		{
			printf("Data %d\n", rec->data[i]);
			
		}
		CAN_message_destructor(rec);
	}
}

void CAN_init(void)
{
	MCP_init();
	// Turn mask filters off, receive any message
	MCP_write(MCP_RXB0CTRL, (3 << RXM0));
	// Enable rollover
	MCP_write(MCP_RXB0CTRL, (1 << BUKT));

	/* Disable rollover */
	//MCP_bit_modify(MCP_RXB0CTRL, (1 << BUKT), 0);

	// Enable interrupt for received message
	MCP_bit_modify(MCP_CANINTE, (1 << RX0IE), 1);

	// Disable interrupt for transmit
	MCP_bit_modify(MCP_CANINTE, (1 << TX0IE), 0);

	/* Set receive flag to zero */
	MCP_bit_modify(MCP_CANINTF, (1 << RX0IF), 0);

	/* Enable interrupt on interrupt pin (defined in system constants)
	   with interrupt on falling edge.
	   (defaults to input pin) */
	SET_BIT(RECEIVE_ISC_REG, RECEIVE_ISC1);
	CLEAR_BIT(RECEIVE_ISC_REG, RECEIVE_ISC0);

	/* Enable interrupt pin as interrupt */
	SET_BIT(RECEIVE_CONTROL_REG, RECEIVE_INTERRUPT);
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
	// Wait for transmit buffer to finish AND receive buffer to finish
	while(READ_BIT(MCP_read(MCP_TXB0CTRL), TXREQ));

	MCP_write(MCP_TXB0CTRL + SIDH_OFFSET, 0);
	MCP_write(MCP_TXB0CTRL + SIDL_OFFSET, message->id << SID0);
	MCP_write(MCP_TXB0CTRL + DLC_OFFSET, message->length);
	MCP_write_n(MCP_TXB0CTRL + D_OFFSET, message->data, message->length);
	MCP_request_to_send();
}


CAN_message * CAN_receive(void)
{
	if(!READ_BIT(MCP_read(MCP_CANINTF), RX0IF))
	{
		// Clear flag bit to clear interrupt
		//MCP_bit_modify(MCP_CANINTF, (1 << RX0IF), 0);

		return CAN_message_constructor(ID_NOT_READY, 0);
	}

	uint8_t id = (MCP_read(MCP_RXB0CTRL + SIDL_OFFSET)) >> SID0;
	uint8_t length = (MCP_read(MCP_RXB0CTRL + DLC_OFFSET)) & DLC_MASK;
	CAN_message * msg = CAN_message_constructor(id, length);
	
	MCP_read_n(MCP_RXB0CTRL + D_OFFSET, msg->data, msg->length);

	// Clear flag bit to clear interrupt
	MCP_bit_modify(MCP_CANINTF, (1 << RX0IF), 0);

	return msg;
	
}