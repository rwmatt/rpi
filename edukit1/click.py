#!/usr/bin/env python3

"""
Button on CamJam EduKit 1 is pin 25
"""

#import timeit
import time
#import os
#from enum import Enum
from collections import deque
from gpiozero import Button
#import datetime
from timeit import default_timer as timer
import sys

class Click:
    def  __init__(self, button, delay = 1):
        self.button = button
        self.delay = delay
        self.state = 0
        self.start_time = 0
        self.stop_time = 0
        self.q = deque()
        pass

    def update(self):
        if self.state == 0 and self.button.is_pressed:
            self.state = 1
            self.start_time = timer()
        elif self.state == 1 and not self.button.is_pressed:
            self.state = 0
            self.stop_time = timer()
            time_pressed = self.stop_time - self.start_time
            #print("Pressed for", time_pressed)
            typ = 2 if time_pressed >= self.delay else 1
            self.q.append(typ)

    def pop(self):
        if self.q:
            return self.q.popleft()
        else:
            return 0

def gen(button):
    c = Click(button)
    while True:
        c.update()
        val = c.pop()
        time.sleep(0.1)
        if val != 0: yield val


def main_loop(button):
    #print("Hit Ctl-C to stop")
    c = Click(button)
    while True:
        c.update()
        val = c.pop()
        #now = datetime.datetime.now()
        if val == 1:
            print("S")
        elif val ==2:
            print("L")
        sys.stdout.flush()
        time.sleep(0.1)

def main():
    pin = int(sys.argv[1])
    b = Button(pin)
    main_loop(b)

if __name__ == "__main__":
    main()

