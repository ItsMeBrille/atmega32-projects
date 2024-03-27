#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))


int main(void)
{

	// LED 1 START

	// Select the unit time CLK / 1024
	SET(TCCR1B, CS12);
	UNSET(TCCR1B, CS11);
	SET(TCCR1B, CS10);
	
	// Set timer into Compare Output Mode (CTC)
	SET(TCCR1B, WGM12);
	UNSET(TCCR1B, WGM11);
	UNSET(TCCR1B, WGM10);
	
	// Toggle OC1A/OC1B on compare match
	UNSET(TCCR1A, COM1A1);
	SET(TCCR1A, COM1A0);
	
	
	SET(DDRD, PD5); // Init LED
	
	
	// Calculated count into the Output Compare Register
	OCR1A = 4883;
	
	// LED 1 END
	

	while(1){
	}
}