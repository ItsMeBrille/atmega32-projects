void USART_init(){
    // USART configuration
    UCSRC |= (1 << UPM1);   // Enable even parity
    UCSRC &= ~(1 << UPM0);  // Even parity
    UCSRC |= (1 << USBS);   // Two stop bits
    UCSRC |= (1 << UCSZ0);  // 8-bit character size
    UCSRC |= (1 << UCSZ1);  // 8-bit character size
    UCSRC &= ~(1 << UCSZ2); // 8-bit character size
    UBRRL = 6;              // 9600 b/s

    // Enable USART transmitter
    UCSRB |= (1 << TXEN) | (1 << RXEN);
}


void sendData(int channel, int setting, uint8_t value){
	// Send USART message
    // 0: 0  -  1,2: Channel - 3,4: Setting - 5-7: Unused
	while (!(UCSRA & (1 << UDRE))){}; // Wait for empty transmit buffer
	UDR = (1 << 7) | (channel << 5) | (setting << 3);
	
    // 0: 1  -  1-7: Value
	while (!(UCSRA & (1 << UDRE))){}; // Wait for empty transmit buffer
	UDR = value &~(1<<7); // Value
}