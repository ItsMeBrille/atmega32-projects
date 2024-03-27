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
			TOGGLE(PORTB, PB0); // LED 2 TOGGLE
		}
	}
}

// Main
int main(void){
	
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
	// Output Compare Register timing
	OCR1A = 4883; 

	SET(DDRD, PD5); // Init LED
	
	// LED 1 END
	
	
	// LED 2 START
	
	SET(DDRB, PB0); // Init LED
	SET(PORTD, PD2); // Init button
	
	// Rising edge interrupt
	SET(GICR, INT0);
	SET(MCUCR, ISC01);
	UNSET(MCUCR, ISC00);
	sei();
	
	// LED 2 END
	
	
	while (1){
	}
}