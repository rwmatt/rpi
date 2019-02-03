import spidev
import random

big = 5000000
spi = spidev.SpiDev()
spi.open(0, 0)

def set_byte(pos, value):
    spi.xfer([pos, value], big)

# 5 is 0x5b
# H is 0x37
# I is 0x30

def ch(): return random.choice([0x5b, 0x37, 0x30])
c1 = ch()
c2 = ch()

pos = random.randint(1,8)
set_byte(pos, c1)
#set_byte(1, c1) # output letter H
#set_byte(2, c2) # output letter I
