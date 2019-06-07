# atlast

atlast is adapted from John Walker's public domain 
[Forth](https://www.fourmilab.ch/atlast/).

I have targetted it towards mcus (microcontrollers), but with
a little massaging, it should work just fine as a Forth on
the desktop.

Advantages:
* pure C implementation, so it should be highly portable and
more readable
* compiles on Arduino. So no more faffing around getting
obscure toolchains working. If you can upload a sketch to your mcu,
then you should be able to get this working, subject to some
caveats.

Disdvantages:
* probably slower than an assembly-written Forth
* VERY rough around the edges. It's a proof of concept, which seems
to have merit.
* It doesn't have the libraries you really want, like fiddling with
GPIO pins. HOWEVER, the Forth is in place, so it's a fairly simple
matter of adding in the hooks to get all the hardware stuff going
* It uses malloc/free in places, which isn't ideal. Maybe there's a way
around that.

## GPIO pins

A simple blink example:
```
2 constant pin \ the built-in LED. Yours will probably be different
pin output pinm \ set pinmode of pin to output
pin high digw   \ turn the LED on
pin low  digw   \ and off again
20 delay-ms     \ delay for 20 milliseconds

\ now let's blink 5 times
: set pin swap digw 500 delay-ms ; \ turn the LED on/off, and wait 500 ms
: blink high set low set ; \ turn it on, then turn it off
: blink5 blink blink blink blink blink ; \ blink 5 times, obviously
blink5 \ now actually do it
```

The above is quite verbose and low-level. We can be more succinct:
```
2 output: led \ equivalent to the first two lines above
led on \ turn it on. ON is just shorthand for HIGH DIGW
led off \ no prizes for guessing what this does
: pause 500 delay-ms ;
: blink led on pause led off pause ; \ implemented slightly differently
blink blink blink blink blink \ 5 times, as before
```
