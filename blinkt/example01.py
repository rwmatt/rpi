from blinkt import set_pixel, show
from random import randint
from time import sleep

def randv(): return randint(0, 255)

while True:
    for pixel in range(8):
        r = randv()
        g = randv()
        b = randv()
        set_pixel(pixel, r, g, b)
        show()
        sleep(0.1)
