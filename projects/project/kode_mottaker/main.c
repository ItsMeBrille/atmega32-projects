#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define F_CPU 1000000UL

// LCD constants
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

#include "LCD.h"   // Custom LCD lib
#include "USART.h" // Custom USART lib

volatile int receiveData; // volatile to ensure its value is not optimized out

// Variables to store selector-bits
uint8_t firstBit, secondBit, thirdBit, fourthBit, fifthBit;

// Variables to store the decoded selector values
int combinedIntNames;
int combinedIntCH;
int integerVolume;

// Array for the different audio frequencies
char *LowerNames[4] = {"LOW :", "HIGH:", "MID:", "XX"}; // XX is a placeholder for more effects

// Function to update the lower part of the LCD
void UpdateLower(const char **names, int value, int selector){
    // Writes FL instead of 100 due to space constraints
    if (value > 99){
        LCD_SendStringAt("FL", 28);
        return;
    }

    // corrects valuePos if number is < 10
    int offset = 0;
    if (value < 10){
        int offset = 1;
        LCD_SendStringAt(" ", 28);
    }

    // Default command if number is within normal range
    LCD_SendStringAt(names[selector], 22);
    LCD_SendNumberAt(value, 28 + offset);
}

// Function to update upper part of LCD
void UpdateTop(int ch, int value){
    int placement;

    // changes the placement variable based on the channel variable
    switch (ch){
    case 1:
        placement = 0;
        break;
    case 2:
        placement = 4;
        break;
    case 3:
        placement = 8;
        break;
    case 4:
        placement = 12;
        break;
    }

    // Writes FL instead of 100 due to space constraints
    if (value > 99){
        LCD_SendStringAt("FL", placement);
        return;
    }

    // Corrects if the number is < 10
    if (value < 10){
        placement += 1;
        LCD_SendStringAt(" ", placement - 1);
    }

    // Write number
    LCD_SendNumberAt(value, placement);
}

// Intertupt function that triggers when the RX pin recieves data
ISR(USART_RXC_vect){
    receiveData = UDR; // Read from USART

    // we use 0xFF to achieve a clear LCD screen once the controller AVR starts up
    if (receiveData == 0xFF){
        LCD_Initialize();
    }

    // Read first bit
    firstBit = (receiveData >> 7) & 1;

    // if the first bit is 1 i knoow that the transfer is for selecting the correct channels and effect
    if (firstBit == 1){
        // Exctract bits
        secondBit = (receiveData >> 6) & 1;
        thirdBit  = (receiveData >> 5) & 1;
        fourthBit = (receiveData >> 4) & 1;
        fifthBit  = (receiveData >> 3) & 1;
        // Combine bits
        combinedIntCH = (secondBit << 1) | thirdBit; // Channel
        combinedIntCH += 1; // More user friendly to start counting at 1 at the display
        combinedIntNames = (fourthBit << 1) | fifthBit; // Effect eg. "HIGH", "LOW", "MID"
    }

    // If not first bit set
    integerVolume = receiveData;

    // Update channel with the correct channel value
    LCD_SendNumberAt(combinedIntCH, 19);

    if (combinedIntNames != 3){
        // update the lower part of the LCD with volume value and effect
        UpdateLower(LowerNames, integerVolume, combinedIntNames); //!!!!!!!!!
    } else {
        // update top part of the LCD with total volume for given channel
        UpdateTop(combinedIntCH, integerVolume); //
    }

    LCD_SendStringAt(" ", 31); // Shortfix to get rid of cursor
}

int main(){
    USART_init(); // Initilize USART
    sei(); // Enable global interrupts

    // Initialize LCD
    LCD_Initialize();

    while (1){} // Empty while loop

    return 0;
}