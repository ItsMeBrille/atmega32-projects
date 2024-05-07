#include <avr/io.h>
#define F_CPU 1000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#define DEBOUNCE_TIME 5000

uint8_t debounce(char pinName, uint8_t pinNumber);

int main(void)
{
    uint8_t buttonPressedInstruction = 0;
	DDRB |= 1 << PINB0;
	PORTB |= 1 << PINB0;
	DDRB &= ~(1 << PINB1);
	PORTB |= 1 << PINB1;
	
	UCSRB |= (1 << RXEN);	//Enable the receiver
	UCSRB |= (1 << TXEN);	//Enable the transmitter
    
	unsigned char receiveData;
    while (1) 
    {
		if (debounce(PINB, 1))
		{
			if (buttonPressedInstruction == 0)
			{
				while ((UCSRA & (1 << UDRE)) == 0) //While data register is NOT empty
				{
													//Do nothing 
				}
				
				UDR = 0b00110000;				//Put the data in the register ready to be sent 
				PORTB ^= 1 << PINB0;
				buttonPressedInstruction = 1;
				
			}
		} 
		else
		{
			buttonPressedInstruction = 0;
		}
		if ((UCSRA & (1 << RXC)) != 0)			//If the data has been received 
		{
			receiveData = UDR;
			if (receiveData == 0b00110000)		//If the received data is the command for toggling the light 
			{
				PORTB ^= (1 << PINB0);
			}
		}
    }
}

uint8_t debounce(char pinName, uint8_t pinNumber)
{
	if (bit_is_clear(pinName, pinNumber)) /* button is pressed now */
	{
		_delay_us(DEBOUNCE_TIME);
		if (bit_is_clear(pinName, pinNumber)) /* still pressed */
		{
			return (1);
		}
	}
	return (0);
}

