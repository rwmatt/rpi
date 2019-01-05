# sleep85

Putting the ATTiny85 to sleep for 8 seconds, conserving a lot of power in the process.

Without the cbi()/sbi() lines, the chip uses about 0.27mA at 4.5V when it is asleep. When the lines are included, it uses about 5uA, a massive reduction! According to some sites, it should be able to get it down to 0.1uA.
To test the circuit, set up the ATTiny85 in the usual way. PB1 (pin 6) goes through a 330R and LED and ground in series. 