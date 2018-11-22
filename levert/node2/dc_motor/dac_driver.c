#include "dac_driver.h"
#include <stdio.h>
#include "TWI_Master.h"

#define DAC_ADDRESS 0x50 //0101 0000
#define DAC_OUTPUT_PIN 0

/* DAC Command Bits */
#define RST 4
#define PD 3

#define DAC_MESSAGE_SIZE 3

static uint8_t DAC_message[DAC_MESSAGE_SIZE];

static void transmit_message(void)
{
	TWI_Start_Transceiver_With_Data(DAC_message, 3);	
}

static void confirm_succsess(void)
{
	while (1){
		while (TWI_Transceiver_Busy());
		if (TWI_statusReg.lastTransOK)
			return;

		printf("#### TWI TRANSMITION FAILED, RESENDING ####");
		TWI_Start_Transceiver();
	}

}

void dac_init(void)
{
	TWI_Master_Initialise();
	DAC_message[0] = DAC_ADDRESS;
	DAC_message[1] = (1 << RST);
	DAC_message[2] = 0;
	transmit_message();
	
	/* Important message, ensure success */
	confirm_succsess();
}

void dac_disable(void)
{
	DAC_message[1] = (1 << PD);
	DAC_message[2] = 0;
	transmit_message();

	/* Important message, ensure success */
	confirm_succsess();
}


void dac_set_output(uint8_t value)
{
	/* Set up command byte */
	DAC_message[1] = (DAC_OUTPUT_PIN << 0) |
					 (0 << PD)|
					 (0 << RST);

	/* Set ouput byte */
	DAC_message[2] = value;
	transmit_message();
}

