# clock85

## Set to 1MHz

Setting the ATTiny85 clock to 1MHz in the IDE. These steps are sufficient.

In the IDE: `Tools : Clock : Internal 1MHz`

In the file:
```
 #define F_CPU 1'000'000UL

void setup() {
	// set clock to 1MHz
	cli(); // disable interrupts
	CLKPR = 1 << CLKPCE; // Tell chip we want to scale the clock
	CLKPR = 3; // prescaler gives 1MHz
	sei(); // enable interrupts
	...
}    
```
## Set to 8MHz

Setting the ATTiny85 clock to 8MHz in the IDE. These steps are sufficient.

In the IDE: `Tools : Clock : Internal 8MHz`

In the file:
```
 #define F_CPU 8'000'000UL

void setup() {
	// set clock to 8MHz
	cli(); // disable interrupts
	CLKPR = 1 << CLKPCE; // Tell chip we want to scale the clock
	CLKPR = 0; // prescaler is 1, giving 8MHz
	sei(); // enable interrupts
	...
}    
```
