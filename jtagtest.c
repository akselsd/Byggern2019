#define F_CPU 4915200
#include <avr/io.h>
#include <util/delay.h>

#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))

int main()
{
	SET_BIT(DDRC, PIN0);

	while(1){
		SET_BIT(PORTC, PIN0);
		_delay_ms(50);
		CLEAR_BIT(PORTC, PIN0);
		_delay_ms(50);
	}
}