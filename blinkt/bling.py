#!/usr/bin/env python3

import argparse
import shlex

from blinkt import set_brightness, set_pixel, show, clear

def main():
    p = argparse.ArgumentParser(description= 'Blinkt controller')
    p.add_argument('--pos', action = "store", help = "Light to (de)activate")
    p.add_argument('--off', action = "store_true", default = False,
            help = "Turn the light off")
    args = p.parse_args()
    print(args)
    b = 0 if args.off else 0.1
    set_brightness(0.1)
    if args.pos:
            set_pixel(int(args.pos), 0, 255, 0)
    show()


def set_led(led, on): 
    #set_brightness(br)
    try:
        led = int(led)
        if led < 0 or led > 7: raise ValueError
    except ValueError:
        print("Not an int [0-7]:", led)
        return

    green = 255 if on else 0
    set_pixel(led, 0, green, 0)
    show()

def main1():
    #on = False
    set_brightness(0.05)
    while True:
        inp = input("")
        fields = list(shlex.shlex(inp))
        if len(fields) == 0: continue
        cmd = fields[0]
        if cmd == "stop": 
            break
        elif cmd == "on":
            set_led(fields[1], True)
        elif cmd == "off":
            set_led(fields[1], False)
        else:
            print("Unknown: inp")

    clear()
    show()


if __name__ == "__main__":
    main1()
