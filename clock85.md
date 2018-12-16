# clock85

Setting the ATTiny85 clock to 8MHz in the IDE. These steps are sufficient.

In the IDE: `Tools : Clock : Internal 8MHz`

In the file:
```
 #define F_CPU 8'000'000UL

void setup() {
    CLKPR = 0x80;
    CLKPR = 0;
    ...
}    
```
