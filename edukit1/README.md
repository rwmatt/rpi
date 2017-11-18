# edukit1

Set up your breadboard as described in your edukit.

## click.py

This is a handy utility for detecting if a button has been given a short
click, or a long click. Import module `click.py`, and use class `Click` 
for this purpose. If a button is depressed for less than a `delay` time
(default 1 second), then it is a short click. Otherwise it is classified
as a long click.

The idea is that you set up a tight polling loop in which you call
Click's `update()` function at each loop. Call `pop()` to obtain a
value. It returns `0` if there is nothing to process, `1` is the button
was short-clicked, and `2` if the button was long-clicked. Look at the 
function `main_loop()` as an example.

`click.py` can also be used at the command line. It prints either `S` (
for "short") of `L` (for "long"). It needs a number for the GPIO position
of the button. If you set up the edukit as per the instructions, then
this is `25`. So, a typical invocation would be:
```
sudo python3 click.py 25
```

## dave

The `dave` script shows how you might use `click.py` in a Unix pipeline.
It reads the output from `click.py` (which is either `S` or `L`) and
reacts accordingly. If it's `S`, it says "I don't think you want to
do that, Dave". If it's a long click, it says "Self destruct sequence
activated".

The script uses the `festival` utility to produce vocal output that you can 
hear on your speaker. To install `festival`:
```
sudo apt-get install festival
```

Running the `dave` script is straightforward enough:
```
dave
```



## toggling demo

A demo of the whole thing in action is available as a 
[YouTube video](https://youtu.be/gjO9wRU6jf4).

Run the demo by typing: 
```
sudo python3 buttonly.py
```

Adapt it to suit your needs.
