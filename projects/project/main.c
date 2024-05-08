#define F_CPU 8000000UL
#include "avr/io.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "rotaries.h" // Custom library for rotary encoders

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))


// Port and GPIO pin, CLK, DT, SW, (value)
struct Rotary LO = {&PORTC, 7, &PORTC, 6, &PORTC, 5}; // Not working
struct Rotary MD = {&PORTA, 4, &PORTA, 5, &PORTA, 6};
struct Rotary HI = {&PORTA, 1, &PORTA, 2, &PORTA, 3};




// Testing LED
void PWM_init(){
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	SET(DDRB, PB3); // OC0 pin as output
}


// Initialize Timer1 for generating interrupt every 1 second
void counter_init(){
	// Select prescaler CLK / 1024
	SET(TCCR0, CS02);
	SET(TCCR0, CS00);
		
	TCNT0 = 248; // 256?(delay_ms*8000 / 1024) -> 1ms
	SET(TIMSK, TOIE0); // Enable overflow interrupt

	// Enable global interrupts
	sei();
}


// Update all sensors
void updateChannels(){
	updateRotary(&LO);
	updateRotary(&MD);
	updateRotary(&HI);
	OCR0=MD.value;
}


// Counter to update continuously
ISR(TIMER0_OVF_vect) {
	updateChannels();
}



int main (){

	PWM_init();	// Test LED
	
	counter_init();
	
	initRotary(&LO);
	initRotary(&MD);
	initRotary(&HI);

	while (1){
// 		updateRotary(&LO);
// 		updateRotary(&MD);
// 		updateRotary(&HI);
// 		OCR0=MD.value;
	}
}