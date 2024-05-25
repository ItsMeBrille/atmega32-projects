// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)


struct Channel{
	uint8_t channel_num;
	volatile uint8_t *sel_btn_port; // Button for selecting channel
	uint8_t sel_btn_num; // Button for selecting channel
	uint8_t muxAddr; // Addr on the mux 0-7
	uint8_t vol_val;
	uint8_t lo_val;
	uint8_t md_val;
	uint8_t hi_val;
};


void initChannel(struct Channel *ch){
	// Channel select button
	// Use as inputs
	UNSET(*(ch->sel_btn_port - 0x01), ch->sel_btn_num); // PORT-0x01 = DDR
	// Use pull-up
	SET(*(ch->sel_btn_port), (ch->sel_btn_num)); // PORT
	_delay_ms(1); // To ensure its finished before startup continues
	// Reset values
	ch->lo_val = 50;
	ch->md_val = 50;
	ch->hi_val = 50;
}


_Bool isSelectedBtnChannel(struct Channel *ch){
	// Check if encoder is rotating
	if(!READ(*(ch->sel_btn_port-0x02), ch->sel_btn_num)){ // PORT-0x02 = PIN
		// Check direction of rotation
		return 1;
	}
	return 0;
}