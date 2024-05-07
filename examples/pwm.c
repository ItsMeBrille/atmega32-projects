#define F_CPU 8000000UL
#include "avr/io.h"
#include <util/delay.h>

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg, pin) reg |= (1 << pin)
#define UNSET(reg, pin) reg &= ~(1 << pin)

int main(){

	// Waveform Generation Mode: Fast PWM
	SET(TCCR0, WGM00);
	SET(TCCR0, WGM01);

	SET(TCCR0, COM01);
	SET(TCCR0, CS00);
	SET(DDRB, PB3); // OC0 pin as output

	int i;
	while (1){
		for (i = 0; i < 255; i++)
		{
			OCR0 = i; /*increase the LED light intensity*/
			_delay_ms(8);
		}
		for (i = 255; i > 1; i--)
		{
			OCR0 = i; /*decrease the LED light intensity*/
			_delay_ms(8);
		}
	}
}