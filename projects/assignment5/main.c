#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL

#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)

// Button interrupt
ISR(INT0_vect){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA) | (1<<TWIE); // Start connection (triggers the first TWI interrupt)
}

// I2C interrupt
ISR(TWI_vect){
	switch (TWSR & 0xF8){
		// Connection started
		case 0x08:
			TWDR = (18 << 1)| (1<<0); // SLA + W
			TWCR = (1<<TWINT) | (1<<TWEN); // Clear all bits and set only TWINT and TWEN
			break; // Interrupt triggers next event. No need to force waiting using while.
			
		// SLA + W has been transmitted
		case 0x40:
			TWCR = (1<<TWINT) | (1<<TWEN);
			break;
			
		// All data has been sent
		case 0x58:
			if(TWDR == 0xF1){
				SET(PORTB, PB3); // Turn on LED
			} else {
				UNSET(PORTB, PB3); // Turn off LED
			}
			TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); // Close connection
			break;
	}
	// TWI interrupt
	SET(TWCR, TWIE);
}


int main(void){
	
	// INIT //
	SET(DDRB, PB3); // LED
	// I2C Pullup
	SET(PORTC, PC0); // SCL
	SET(PORTC, PC1); // SDA
	
	TWBR = 2;
	
	// Button interrupt (INT0 / PD2)
	SET(GICR, INT0);
	SET(PORTD, PD2);
	SET(MCUCR, ISC01); // Rising edge
	UNSET(MCUCR, ISC00);
	
	// TWI interrupt
	SET(TWCR, TWIE);
	
	// Enable all interrupts
	sei();
	
	while (1){
		// (っ・ω・）っ ≡≡≡≡≡≡☆
	}	
}