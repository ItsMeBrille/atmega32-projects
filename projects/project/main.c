#define F_CPU 8000000UL
#include "avr/io.h"
#include <util/delay.h>
#include "rotaries.h" // Custom library for rotary encoders

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))


// Testing LED
void PWM_init(){
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	SET(DDRB, PB3); // OC0 pin as output
}


int main (){
	// Port and GPIO pin, CLK, DT, SW, (value)
	struct Rotary HI = {&PORTA, 1, &PORTA, 2, &PORTA, 3};
	initRotary(&HI);

	struct Rotary MD = {&PORTA, 4, &PORTA, 5, &PORTA, 6};
	initRotary(&MD);
	
	struct Rotary LO = {&PORTC, 7, &PORTC, 6, &PORTC, 5}; // Not working
	initRotary(&LO);

	PWM_init();	// Test LED

	while (1){
		updateRotary(&LO);
		updateRotary(&MD);
		updateRotary(&HI);
		OCR0=MD.value;
	}
}