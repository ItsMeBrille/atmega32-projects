// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)

#define ENCODER_STEPS 3 // Value increment pr rotary step


// *(port - hex) is because the PIN, PORT and DDR register is located with relative
// distance to each other in memory. Therefore we need _port and we can calc the rest.

struct Rotary{
	volatile uint8_t *clk_port;
	uint8_t clk_num;
	volatile uint8_t *dt_port;
	uint8_t dt_num;
	volatile uint8_t *sw_port;
	uint8_t sw_num;
	int value;
};



void initRotary(struct Rotary *rot){
	// Use as inputs
	UNSET(*(rot->clk_port - 0x01), rot->clk_num); // PORT-0x01 = DDR
	UNSET(*(rot->dt_port - 0x01), rot->dt_num);
	UNSET(*(rot->sw_port - 0x01), rot->sw_num);
	// Use pull-up
	SET(*rot->clk_port, rot->clk_num); // PORT
	SET(*rot->dt_port, rot->dt_num);
	SET(*rot->sw_port, rot->sw_num);
	// Reset value
	rot->value = 50;
	_delay_ms(1); // To ensure its finished before startup continues
}


_Bool updateRotary(struct Rotary *rot){
	// Check if encoder is rotating
	if(!READ(*(rot->clk_port-0x02), rot->clk_num)){ // PORT-0x02 = PIN
		// Check direction of rotation
		if(!READ(*(rot->dt_port-0x02), rot->dt_num)){
			// Counter clockwise
			if(rot->value-ENCODER_STEPS < 0){rot->value=0;} // Upper limit
			else {rot->value-=ENCODER_STEPS;} // Decrement
		} else {
			// Clockwise
			if(rot->value+ENCODER_STEPS > 127){rot->value=127;} // Upper limit
			else {rot->value+=ENCODER_STEPS;} // Increment
		}
		while(!READ(*(rot->clk_port-0x02), rot->clk_num)){} // Wait until rotation is complete before continuing
		return 1; // Changed
	}
	// Click rotary to reset to middle
	else if(!READ(*(rot->sw_port-0x02), rot->sw_num)){
		rot->value = 50;
		return 1; // Changed
	}
	return 0;
}