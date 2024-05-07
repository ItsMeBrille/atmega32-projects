#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL
#include <util/delay.h>

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))

// Interrupt for LED 2
ISR(INT0_vect){
	// Ensure button release
	if (!READ(PIND, PB2)){
		_delay_ms(50);
		if (!READ(PIND, PB2)){
			TOGGLE(PORTB, PB0); // TOGGLE LED
		}
	}
}

// Main
int main(void){

	SET(DDRB, PB0); // Init LED
	SET(PORTD, PD2); // Init button
	
	// Rising edge interrupt
	SET(GICR, INT0);
	SET(MCUCR, ISC01);
	UNSET(MCUCR, ISC00);
	sei();
	
	return 0;
}