#define F_CPU 8000000UL
#include "avr/io.h"
#include <util/delay.h>

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)

int main (){
	
	// Init PWM
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	SET(DDRB, PB3); // OC0 pin as output
	
	// Init ADC channel 0
	UNSET(DDRA,PA0); // Make ADC port as input
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC with prescaler of 128
	SET(ADMUX, REFS0); // Set reference voltage to AVcc (supply voltage)
	
	int value;
	
	while (1){
		
		// Read data
		SET(ADCSRA, ADSC); // Start conversion
		while(!(ADCSRA & (1 << ADIF))); // Wait for conversion to complete
		value = ADCL | (ADCH << 8); // Read high and low bits
		
		// Update PWM
		OCR0=value/4;
	}
}