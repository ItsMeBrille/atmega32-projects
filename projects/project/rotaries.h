// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)

#define ENCODER_STEPS 3 // Value increment pr rotary step

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
	UNSET(*(rot->clk_port - 0x01), rot->clk_num);
	UNSET(*(rot->dt_port - 0x01), rot->dt_num);
	UNSET(*(rot->sw_port - 0x01), rot->sw_num);
	// Use pull-up
	SET(*rot->clk_port, rot->clk_num);
	SET(*rot->dt_port, rot->dt_num);
	SET(*rot->sw_port, rot->sw_num);
	
	// Reset value
	rot->value = 64;
}


void updateRotary(struct Rotary *rot){
	// Check if encoder is rotating
	if(!READ(*(rot->clk_port-0x02), rot->clk_num)){// *(pin -0x02) is because the PIN register is located with relative distance to the PORT register.
		// Check direction of rotation
		if(!READ(*(rot->dt_port-0x02), rot->dt_num)){
			// Counter clockwise
			if(rot->value-ENCODER_STEPS < 0){rot->value=0;} // Upper limit
			else {rot->value-=ENCODER_STEPS;} // Increment
			
			} else {
			// Clockwise
			if(rot->value+ENCODER_STEPS > 127){rot->value=127;} // Upper limit
			else {rot->value+=ENCODER_STEPS;} // Increment
		}
		while(!READ(*(rot->clk_port-0x02), rot->clk_num)){} // Wait until rotation is complete before continuing
	}
	// Click rotary to reset to middle
	else if(!READ(*(rot->sw_port-0x02), rot->sw_num)){
		rot->value=64;
	}
}