/*
DEPENDENCIES:
* <avr/io.h>
* <util/delay.h>
* <stdlib.h>

FUNCTIONS:
* LCD_Command(char command)
* LCD_Clear()
* LCD_Initialize()
* LCD_SendChar(char letter)
* LCD_SendString(char *str)
* LCD_SendStringAt(char *str, int pos)
* LCD_SendNumber(int number)
* LCD_SendNumberAt(int number, int pos)
*/

void LCD_Command(char command){
	PORTData = command;
	
	PORTCommand &= ~(1<<RS);
	PORTCommand &= ~(1<<RW);
	
	PORTCommand |= (1<<EN);
	_delay_us(1);
	PORTCommand &= ~(1<<EN);
	_delay_us(1530);
}

void LCD_Clear(){
	// Send CLEAR DISPLAY command
	LCD_Command(1<<DB0);
}

void LCD_Initialize(){
	// Set ports as output
	DDRCommand|= (1<<RS)|(1<<RW)|(1<<EN);
	DDRData|= 0xFF;
	_delay_us(1530);
	LCD_Command((1<<DB5)|(0b11100)); // Send FUNCTION SET command
	LCD_Command((1<<DB3)|(0b111)); // Send DISPLAY ON/OFF command
	LCD_Command((1<<DB2)|(0b10)); // Send ENTRY MODE command
	LCD_Command(1<<DB1); // Send RETURN HOME command

	// Clear LCD
	LCD_Clear();
}

void LCD_SendChar(char letter){
	PORTData = letter;
	
	PORTCommand |= (1<<RS);
	PORTCommand &= ~(1<<RW);
	
	PORTCommand |= (1<<EN);
	_delay_us(1);
	PORTCommand &= ~(1<<EN);
	_delay_us(1530);
}

void LCD_SendCharAt(char letter, int pos){
	if (pos/16 == 0)
	LCD_Command((pos%16 & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (pos/16 == 1)
	LCD_Command((pos%16 & 0x0F)|0xC0);

	LCD_SendChar(letter);
}

void LCD_SendString(char *str){
	for (int i=0;str[i]!=0;i++){
		LCD_SendChar(str[i]);
	}
}

void LCD_SendStringAt(char *str, int pos){
	if (pos/16 == 0)
	LCD_Command((pos%16 & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (pos/16 == 1)
	LCD_Command((pos%16 & 0x0F)|0xC0);
	
	LCD_SendString(str);
}

void LCD_SendNumber(int number){
	char N[16];
	itoa(number,N,10);
	LCD_SendString(N);
}

void LCD_SendNumberAt(int number, int pos){
	if (pos/16 == 0)
	LCD_Command((pos%16 & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (pos/16 == 1)
	LCD_Command((pos%16 & 0x0F)|0xC0);

	LCD_SendNumber(number);
}
