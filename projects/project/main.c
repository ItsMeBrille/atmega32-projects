#define F_CPU 1000000UL
#include "avr/io.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "channels.h" // Custom library for handling each channel
#include "rotaries.h" // Custom library for rotary encoders
#include "faders.h" // Custom library for the analog inputs/mux
#include "USART.h" // Custom library for USART COMS

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)

// Port and GPIO pin for select button, A0 mux address
struct Channel ch1 = {0, &PORTD, 5, 0};
struct Channel ch2 = {1, &PORTD, 4, 1};
struct Channel ch3 = {2, &PORTD, 3, 2};
struct Channel ch4 = {3, &PORTD, 2, 3};

// Port and GPIO pin, CLK, DT, SW, (value)
struct Rotary LO = {&PORTA, 7, &PORTC, 7, &PORTC, 6}; // Not working
struct Rotary MD = {&PORTA, 4, &PORTA, 5, &PORTA, 6};
struct Rotary HI = {&PORTA, 1, &PORTA, 2, &PORTA, 3};

struct Channel active; // Variable used when iterating the structs

// Update all sensors
void updateChannels(){
	// Selection buttons selects what channel to update
	if(isSelectedBtnChannel(&ch1)){active = ch1;}
	else if(isSelectedBtnChannel(&ch2)){active = ch2;}
	else if(isSelectedBtnChannel(&ch3)){active = ch3;}
	else if(isSelectedBtnChannel(&ch4)){active = ch4;}
	
	// Update eq from rotary encoders
	if(updateRotary(&LO)){
		active.lo_val = LO.value;
		sendData(active.channel_num, 0, active.lo_val);
	}
 	if(updateRotary(&MD)){
		active.md_val = MD.value;
		sendData(active.channel_num, 2, active.md_val);
	}
 	if(updateRotary(&HI)){
		active.hi_val = HI.value;
		sendData(active.channel_num, 1, active.hi_val);
	}
	
	// Update volume from fader (if mux worked this would be a loop)
	int check = faderRead(active.muxAddr);
	if (active.vol_val < check-1 || active.vol_val > check+1){
		active.vol_val = check;
		sendData(active.channel_num, 3, active.vol_val);
	}
	
}

// Initialize Timer2 for generating interrupt every 1 second
void counter_init(){
	// Use no prescaler CLK / 1
	SET(TCCR2, CS21);
	
	TCNT2 = 248; // 1 us -> 256-(delay_ms*8000 / 1)
	SET(TIMSK, TOIE2); // Enable overflow interrupt
}

// Counter to update continuously
ISR(TIMER2_OVF_vect){
	updateChannels();
}



int main(){
	active = ch1; // Set default channel
	
	// Init analog and mux
	fader_init();

	// Init rotary encoders
 	initRotary(&LO);
 	initRotary(&MD);
 	initRotary(&HI);
	
	// Init channels (select buttons and temp variables)
 	initChannel(&ch1);
 	initChannel(&ch2);
 	initChannel(&ch3);
 	initChannel(&ch4);

	USART_init(); // Start USART
	
	counter_init(); // Start counter
	sei();	// Enable interrupt
	
	while(1){}
}