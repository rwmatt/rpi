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

## Communicating with the device

Communication with atlast's repl happens over the serial line at
a speed of 115200, and expects and emits \n at the end of the line.
The best way to do this is:
```
python3 ser.py
```

Note that this probably won't work on Windows due to the way that
keyboard input is detected.

You can send files over the serial line:
```
python3 ser.py < foo.4th
```
or several:
```
cat *4th | python3 ser.py
```

You need to enter Ctl-c to exit, though. I hope to improve upon this.


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

Let's create a demo which blinks until the user presses a key:
```
: loop begin blink key? if "Finished" type exit then again ;
loop \ start the loop
```

## Arduino-specific words

dacw ( vol pin -- ): set the pin usually 25 or 26) to a volume (0-255 inc)
See `dac.4th` for generating a tone at 440Hz using a sine wave.

delay-ms ( n --): delay for n milliseconds

digr (pin --): digitalRead(pin)

digw (bool pin --): set pin high or low

format: format the SPIFFs filesystem ready for reading/writing

hi: prints a friendly greeting

key?: returns if the serial is avaliable

msecs ( -- ms): returns  millis()

info - print out info about the system

pinm ( pin mode --): set the pin mode

usecs ( -- us): returns micros()



## Filesystem

`atlast` uses the `SPIFFS` file-system. It needs to be formatted first.
You can test the filesystem using the word `testfs`. Also refer to
it (`P_testfs`) for clues as to how it is implemented internally.

[Link](https://github.com/espressif/arduino-esp32/blob/master/libraries/SPIFFS/examples/SPIFFS_Test/SPIFFS_Test.ino)

## Macrology

Ah, the dark arts of extending the Forth compiler. Now then, now then, now then ...

Forth doesn't have "macros" per se, but there are ways of embedding code within other code, with the same logic
as exists in Lisp. It allows you to abstract away common lexical patterns.

I'm going to suggest an approach that works for `atlast`. It seems to have a very Forth79 feel to it. Some
might say that's the best feeling to have. The standard has enough in there to do what we want. Your Forth may
act a little different, esp. wrt the way branching works.

I'd say that a good strategy when developing macros which branching in them to forget about `IF`, `THEN`, `ELSE`, etc.,
and think in terms of `BRANCH`, `?BRANCH`, `[`, `]`, `HERE` and `COMPILE`. Sometimes `[COMPILE]` might be useful,
but I haven't wrapped my brain around it yet.

Before writing a macro, write it non-macro form to see if you can get the thing working for starters. 

Suppose we want a word that prints out `5 4 3 2 1` using a block-like approach. Here's a solution:
```
: foo 5 [ here ] dup . 1- dup 0= ?branch [ here - cell / , ] drop ;
```

Note that vanilla `atlast` does not define `CELL`. It's a word that I've added myself. It is simple `sizeof(void*)`. 
So on 64-bit systems, it will be 8, on 32-bit systems like for the ESP32, it will be 8.

So here's how `FOO` works
```
5        \ put 5 on stack. Obvious enough

[ here ] \ [ puts us into immediate mode,  
         \ HERE puts the current heap address on the stack during the compilation stage
         \ ] puts us back into compilation mode

dup .    \ copy the top of the stack and print

1- dup 0= \ have we finished?

?branch   \ if the top of the stack is 0, jump relative to the cell offset indicated by ....
[ here - cell / \ when compiling, go into immediate mode, work out the offset we need
,         \ embed that offset in the word being compiled     
]         \ go back into compilation mode

drop      \ clean up the stack 
```

Simple? OK, maybe not. Now let's take things up a notch. Let's abstract away the `[ here ]`. Actually,
Forth already has a word for that called `BEGIN`, so we won't bother to define out own terms.

Now we want to abstract away a pattern that we think is common, namely:
```
0= ?branch [ here - cell / , ]
```
It means that we go back to where the `BEGIN`is if the top of the stack isn't 0, Let's call that word `?AGAIN`.
Here's how we'd define it:
```
: ?AGAIN compile 0= compile ?branch here - cell / , ; immediate
```

You following this? `?AGAIN` is declared `IMMEDIATE` because we want to embed it in whatever word we use it in.
`COMPILE` means that we want the next word compiled into the word being defined. The rest of the word happens
at compile time, immediately, so we don't need the `[` or the `]`.

So here' how we'd rewrite `foo`:
```
: foo 5 begin dup . 1- dup ?again drop ;
```

See how much simpler it becomes? We can now use that pattern repeatedly. Praise the Lord!

