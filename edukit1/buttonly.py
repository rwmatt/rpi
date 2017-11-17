
#import timeit
import time
from time import sleep
from os import system
#from enum import Enum
from collections import deque
from gpiozero import Button, LED
#import datetime
from timeit import default_timer as timer
import sys

import click

red = LED(18)
yellow = LED(23)
green = LED(24)
button = Button(25)

leds = [0, green, yellow, red]
ledn = 0

def blink(led):
    for i in range(12):
        led.on()
        sleep(0.2)
        led.off()
        sleep(0.2)
    led.on()

for clc in click.gen(button):
    if clc == 1:
        if ledn != 0: leds[ledn].off()
        ledn = (ledn +1) % len(leds)
        if ledn != 0: leds[ledn].on()
    if clc == 2 and ledn != 0:
        blink(leds[ledn])
        if ledn == 3: system('aplay nostromo.wav')




