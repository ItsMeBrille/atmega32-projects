# Coding tips

## Table of Contents

1. [Ports](#ports)
2. [Registers](#registry-macros)
3. [Interrupt](#interrupt)
4. [Template](#template)


## Ports

| Port A | Alternate Function |
|---|---|
| PAx | ADCx (input only) |

| Port B | Alternate Function | Port C | Alternate Function | Port D | Alternate Function |
|---|---|---|---|---|---|
| PB0 | T0 | PC0 | SCL | PC1 | SDA |
| PB1 | T1 | PC2 | TCK | PC3 | TMS |
| PB2 | AIN0 | PC4 | TDO | PC5 | TDI |
| PB3 | AIN1 | PC6 | TOSC1 | PC7 | TOSC2 |
| PB4 | SS | PD0 | RXD | PD1 | TXD |
| PB5 | MOSI | PD2 | INT0 | PD3 | INT1 |
| PB6 | MISO | PD4 | OC1B | PD5 | OC1A |
| PB7 | SCK | PD6 | ICP1 | PD7 | OC2 |


## Registers

### Register macros

These macros improve readability of bitshift operations in registers. Since they use the `#define` syntax it compiles before the code runs. Therefore it doesn't affect code efficiency.

```c
#define SET(reg,pin) reg |= (1 << pin)
#define UNSET(reg,pin) reg &= ~(1 << pin)
#define READ(reg,pin) ((0x00 == ((reg & (1<<pin))>> pin))?0x00:0x01)
#define TOGGLE(reg,pin) (reg ^= (1<<pin))
```

### Useful registers

#### I/O Ports
| DDRx | PORTx | I/O |
|---|---|---|
| 0 | 0 | Pull-down |
| 0 | 1 | Pull-up |
| 1 | 0 | Output LOW |
| 1 | 1 | Output HIGH |

#### Other registries
| Name | 0 | 1 | READ |
|---|---|---|---|
| DDRx | Input | Output |-|
| PORTx | Sets LOW | Sets HIGH |-|
| PINx | - | - | HIGH/LOW |


## Interrupt

Interrupt can be used on port `INT0` and `INT1`. To use the interrupts you first need to set the bit `INTx` on the `GICR` register. The `ISCx1` and `ISCx0` bits in the `MCUCR` register controls what scenarios triggers the interrupt:

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

	// Rising edge interrupt
    SET(PORTD, PD2); // Init trigger
	SET(GICR, INT0);
	SET(MCUCR, ISC01); // ISCx1
	UNSET(MCUCR, ISC00); // ISCx0
	sei(); //Enable interrupts
	
	return 0;
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