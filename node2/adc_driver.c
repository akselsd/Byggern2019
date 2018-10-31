#include <avr/io.h>
#include "adc_driver.h"
#include "bit_macros.h"

/*#define MUX0 0
#define MUX1 1
#define MUX2 2
#define MUX3 3
#define MUX4 4
#define MUX5 3*/

void adc_init(void) {
	/* Enable input */
	/*SET_BIT(DDRK, DDK7);*/

	/* Prescale clock */
	SET_BIT(ADCSRA, ADPS0);
	SET_BIT(ADCSRA, ADPS1);
	SET_BIT(ADCSRA, ADPS2);

	/* Select ADC 0 as single ended input */
	CLEAR_BIT(ADCSRB, MUX5);
	CLEAR_BIT(ADMUX, MUX4);
	CLEAR_BIT(ADMUX, MUX3);
	CLEAR_BIT(ADMUX, MUX2);
	CLEAR_BIT(ADMUX, MUX1);
	CLEAR_BIT(ADMUX, MUX0);

	/* Set Vref to GND */
	CLEAR_BIT(ADMUX, REFS0);
	SET_BIT(ADMUX, REFS1);

	/* Disable auto trigger */
	CLEAR_BIT(ADCSRA, ADATE);

	/* Enable ADC */
	SET_BIT(ADCSRA, ADEN);
}

uint16_t adc_read(void) {
	/* Start conversion and wait */
	SET_BIT(ADCSRA, ADSC);
	while(ADCSRA & (1 << ADSC));

	return ADC;
}