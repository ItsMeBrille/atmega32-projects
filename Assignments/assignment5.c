#define F_CPU 8000000UL
#include "avr/io.h"
#include <util/delay.h>

// DDR for direction, PORT for setting value, PIN for reading value.
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)

int main (){
	
	while (1){
	}
}