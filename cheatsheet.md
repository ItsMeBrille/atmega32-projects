# Cheatsheet for ATmega32 programming in C

## Table of Contents

1. [Ports](#ports)
1. [Registers](#register-macros)
1. [Interrupt](#interrupt)
1. [Timers/counters](#counter)
1. [PWM](#pwm)
1. [Analog Inputs](#analog-inputs)
1. [Template](#template)


## Ports

| Port A | Alternate Function |
|---|---|
| PA0 - PA7 | ADC0 - ADC7 (analog input) |

| Port B | Alternate Function | Port C | Alternate Function | Port D | Alternate Function |
|---|---|---|---|---|---|
| `PB0` | `T0` | `PC0` | `SCL` | `PC1` | `SDA` |
| `PB1` | `T1` | `PC2` | `TCK` | `PC3` | `TMS` |
| `PB2` | `AIN0` | `PC4` | `TDO` | `PC5` | `TDI` |
| `PB3` | `AIN1` | `PC6` | `TOSC1` | `PC7` | `TOSC2` |
| `PB4` | `SS` | `PD0` | `RXD` | `PD1` | `TXD` |
| `PB5` | `MOSI` | `PD2` | `INT0` | `PD3` | `INT1` |
| `PB6` | `MISO` | `PD4` | `OC1B` | `PD5` | `OC1A` |
| `PB7` | `SCK` | `PD6` | `ICP1` |` PD`7 | `OC2` |


### I/O Ports

To set the mode for the basic I/O ports, use these registers. `x` is port `A`, `B`, `C` or `D`:

| DDRx | PORTx | I/O |
|---|---|---|
| 0 | 0 | Pull-down |
| 0 | 1 | Pull-up |
| 1 | 0 | Output LOW |
| 1 | 1 | Output HIGH |

To read a ports value when the port is in one of the input modes, use the `PINx` registers.

### Code example

Function to read a button at 
```c
uint8_t debounce(char pinName, uint8_t pinNumber)
{
	if (bit_is_clear(pinName, pinNumber)) // If the button is being pressed
	{
		_delay_ms(5);						  // wait 5 ms
		if (bit_is_clear(pinName, pinNumber)) // If the button is still being pressed
		{
			return (1); // return yes the button was pressed
		}
	}
	return (0);
}
```


## Registers

### Register macros

These macros improve readability of bitshift operations in registers. Since they use the `#define` syntax it compiles before the code runs. Therefore it doesn't affect code efficiency.

```c
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))
```


### List of useful registers

| Name | 0 | 1 | READ |
|---|---|---|---|
| DDRx | Input | Output |-|
| PORTx | Sets LOW | Sets HIGH |-|
| PINx | - | - | HIGH/LOW |


## Interrupt

Interrupt can be used on port `INT0` and `INT1`. In order to use the interrupts you first need to set the bit `INTx` on the `GICR` register. The `ISCx1` and `ISCx0` bits in the `MCUCR` register controls what scenarios triggers the interrupt:

| ISCx1 | ISCx0 | INTx state |
|---|---|---|
| 0 | 0 | Low |
| 0 | 1 | Any change |
| 1 | 0 | Falling edge |
| 1 | 1 | Rising edge |

There is also an interrupt function on `INT2`, but it can only be used with edge triggers. The trigger scenarios for `INT2` is set using the `ISC2` bit in the `MCUCSR` register:

| ISC2 | INT2 state |
|---|---|
| 0 | Falling edge |
| 1 | Rising edge |


### Code syntax

- `ISR(INTx_vect){}` is the function that triggers at an interrupt.
- `sei();` **enables** all interrupts.
- `cli();` **disables** all interrupts.


### Code example

```c
#include <avr/interrupt.h>
```
```c
ISR(INT0_vect){
    // What happens at interrupt
}
```
```c
int main(void){
    // Activate interrupt on INT0
    SET(GICR, INT0);
    // Init trigger
    SET(PORTD, PD2);
    // Rising edge interrupt
    SET(MCUCR, ISC01);
    UNSET(MCUCR, ISC00);
    //Enable interrupts
    sei();

    return 0;
}
```


## Counter

There are three timers / counters:

- Timer0: 8-bit timer
- Timer1: 16-bit timer
- Timer2: 8-bit timer

The TCCRx registers controls the counter modes, where x can be `0`, `1A`, `1B` or `2`.

Waveform Generation Mode:
| WGM00 | WGM01 | Timer0 |
|---|---|---|
| 0 | 0 | Normal |
| 0 | 1 | CTC (Clear timer on Compare Match) |
| 1 | 0 | PWM, Phase correct |
| 1 | 1 | Fast PWM |

Clock Source Select:
| CS12 | CS11 | CS10 | Prescaled clock |
|---|---|---|---|
| 0 | 0 | 0 | Clock stopped |
| 0 | 0 | 1 | Clk (no prescaling) |
| 0 | 1 | 0 | Clk / 8 |
| 0 | 1 | 1 | Clk / 64  |
| 1 | 0 | 0 | Clk / 256 |
| 1 | 0 | 1 | Clk / 1024 |
| 1 | 1 | 0 | T1 pin, falling edge |
| 1 | 1 | 1 | T1 pin, rising edge |

The `TCNTx` registers count each clock pulse. The value in the `OCRx` registers is compared with the content of the `TCNTx` registers. When they are equal, the `OCFx` flag will get set. The flags is located within the Timer Counter Interrupt Flag register `TIFR`.

What happens when the flags is set is determined by `COMx1` and `COMx0` in the 

| COMx1 | COMx0 | Description |
|---|---|---|
| 0 | 0 | Disable counter |
| 0 | 1 | Toggle `OCx` |
| 1 | 0 | Unset `OCx` |
| 1 | 1 | Set `OCx` |


### Code syntax
- `#define F_CPU 8000000` sets the base clock frequenzy
- `OCRx = (7813);` controlls the register the counter tries to match. To find compare value from seconds use this: **(seconds * pre-scaled clock)**


### Code example

```c
#define F_CPU 1000000 // Base clock (hz)
```
```c
int main(void){
    // Select the unit time CLK / 1024
    SET(TCCR1B, CS12);
    UNSET(TCCR1B, CS11);
    SET(TCCR1B, CS10);
    // Set timer into Compare Output Mode (CTC)
    SET(TCCR1B, WGM12);
    UNSET(TCCR1B, WGM11);
    UNSET(TCCR1B, WGM10);
    // Toggle OC1A on compare match
    UNSET(TCCR1A, COM1A1);
    SET(TCCR1A, COM1A0);

    // Output Compare Register timing
    OCR1A = 4883; // 5s * (1000000/1024)hz = 4883

    // Init OC1A port
    SET(DDRD, PD5); 

    return 0;
}
```


## PWM

Pulse width modulation (PWM) on the ATmega use the counter in fast PWM mode set by `TCCRx`to generate pulses.


### Code syntax

- `OCR0 = i;` determines the voltage generated.


### Code example

```c
#define F_CPU 1000000 // Base clock (hz)
```
```c
int main(){
    // Waveform Generation Mode: Fast PWM
    SET(TCCR0, WGM00);
    SET(TCCR0, WGM01);

    // Toggle at Compare match
    SET(TCCR0, COM01);

    // No prescaler
    SET(TCCR0, CS00);
    
    // OC0 port as output
    SET(DDRB, PB3);

    while (1){
        // Set PWM 'voltage' 0 - 255
        OCR0 = i;
    }
}
```


## Analog Inputs

The ADC (Analog to Digital converter) on the ATmega can read the voltage on port `PA0 - PA7` with an accuracy of 10 bits. Since each register can only hold 8 bits, the value have to be stored in 2 registers, `ADCH` and `ADCL`. The ADC acts like a MUX, so you are **only able to meassure 1 of the 8 ports at ay one time.**

| REFS1 | REFS0 | Vref to ADC |
|---|---|---|
| 0 | 0 | AREF pin |
| 0 | 1 | AVCC pin i.e. Vcc 5 V |
| 1 | 0 | Reserved |
| 1 | 1 | Internal 2 |


### Code Syntax

- `SET(ADMUX, REFS0);` sets the reference voltage equal to the supply voltage
- `while(!(ADCSRA & (1 << ADIF)));` waits for conversion to complete
- `value = ADCL | (ADCH << 8);` reads the value by reading combining high and low bits
- `#define MAP(x, a, b, c, d) ((x - a) * (d - c) / (b - a) + c)` can be used as macro for mapping values between ranges. Use like this, `MAP(value, low_in, hi_in, low_out, hi_out)`

# Code example

```c
int main (){
    
    // Enable ADC (Analog to Digital Converter) module
    SET(ADCSRA, ADEN); // Set the ADEN (ADC Enable) bit in the ADCSRA register

    // Configure ADC prescaler for ADC clock frequency
    SET(ADCSRA, ADPS2);
    SET(ADCSRA, ADPS1);
    SET(ADCSRA, ADPS0);

    // Set reference voltage to AVcc (supply voltage)
    SET(ADMUX, REFS0);
    
    int value;
    
    while (1){
        // Read data
        SET(ADCSRA, ADSC); // Start conversion
        while(!(ADCSRA & (1 << ADIF))); // Wait for conversion to complete
        value = ADCL | (ADCH << 8); // Read high and low bits
        
        // 'value' now contains a number between 0 and 1023
    }
}
```


# Template

```c
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))

int main(void){
    while(1){
        break;
    }
    return 0;
}
```