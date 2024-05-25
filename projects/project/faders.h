#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)

#define S0 PC2
#define S1 PC1
#define S2 PC0
	
void analog_init(){
	// Enable ADC (Analog to Digital Converter) module
	SET(ADCSRA, ADEN); // Set the ADEN (ADC Enable) bit in the ADCSRA register
	// Configure ADC prescaler for ADC clock frequency
	SET(ADCSRA, ADPS2);
	SET(ADCSRA, ADPS1);
	SET(ADCSRA, ADPS0);
	// Set reference voltage to AVcc (supply voltage)
	SET(ADMUX, REFS0);
}


int analogRead(){
	// Read data
	SET(ADCSRA, ADSC); // Start conversion
	while(!(ADCSRA & (1 << ADIF))); // Wait for conversion to complete
	return (ADCL | (ADCH << 8)); // Read high and low bits and return value (0-1023)
}


void fader_init(){
	analog_init();
	SET(DDRC, S0);
	SET(DDRC, S1);
	SET(DDRC, S2);
}


int faderRead(int mux){	
	// Multiplexer
	if (mux & 1){SET(PORTC, S0);} // S0
	else {UNSET(PORTC, S0);}
	if ((mux >> 1) & 1){SET(PORTC, S1);} // S1
	else {UNSET(PORTC, S1);}
	if ((mux >> 2) & 1){SET(PORTC, S2);} // S2
	else {UNSET(PORTC, S2);}
	UNSET(PORTC, S0);
	UNSET(PORTC, S1);
	UNSET(PORTC, S2);
	_delay_ms(10);
	return analogRead() / 10; // Return value from A0
}