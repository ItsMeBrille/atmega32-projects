#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <stdlib.h>

#define DDRCommand DDRB
#define PORTCommand PORTB
#define EN PB3
#define RW PB2
#define RS PB1

#define DDRData DDRA
#define PORTData PORTA
#define DB7 PA7
#define DB6 PA6
#define DB5 PA5
#define DB4 PA4
#define DB3 PA3
#define DB2 PA2
#define DB1 PA1
#define DB0 PA0

#include "LCD.h" // Custom LCD lib (in the libraries directory)

int main(void){
	// Initialize LCD
	LCD_Initialize();
	
	// Birth 1
	LCD_SendStringAt("Girls:", 0);
	LCD_SendNumberAt(2, 15);
	
	// Birth 2
	LCD_SendStringAt("Gingers:", 16);
	LCD_SendNumberAt(4, 31);
	
	while (1){
		// (?????? ???????
	}	
}