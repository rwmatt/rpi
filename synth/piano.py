import RPi.GPIO as GPIO
#from curses import wrapper

import sys, pygame

import enough
import master
from master import usleep

keys = """
0 0
g G3
h A3
j B3
k C4
"""

pygame.init()
scr = pygame.display.set_mode((200,200))

freqs = {}
def init_freqs():
    #global freqs
    for key in keys.splitlines():
        if len(key) == 0: continue        
        kbd, note = key.split()
        freqs[kbd] = enough.nmap[note]

def inner():
    while 1:
        events = pygame.event.get()
        for ev in events:            
            if ev.type == pygame.QUIT: return
            if ev.type == pygame.KEYDOWN:
                #print('key down')
                c = pygame.key.name(ev.key)
                #print(c)
                if c == 'q': return
                try:
                    f = freqs[c]
                    master.send_freq(f)
                except KeyError:
                    pass
            if ev.type == pygame.KEYUP:
                #print('key up')
                master.send_freq(0);

def main():
    try:
        #global freqs
        init_freqs()
        master.setup()
        #print(freqs)
        inner()
    finally:
        GPIO.cleanup()

if __name__ == "__main__": main()        

