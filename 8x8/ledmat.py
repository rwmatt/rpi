from machine import I2C, Pin
import urandom

sda = Pin(4) # D2
scl = Pin(5) # D1

i2c = I2C(scl = scl, sda = sda)
print(i2c.scan())


def send_cmd(cmd):
    if isinstance(cmd, int): cmd = [cmd]
    arr = bytearray(cmd)
    i2c.writeto(0x70, arr)
    
# initialise device
send_cmd(0x21)
send_cmd(0x81)
send_cmd(0xE0)

# fill the display with random chats
for y in range(0, 8):
    patt = urandom.getrandbits(8) # 0x0 - 0xF (15)
    send_cmd([y*2, patt])

def rl(): # convenience reload of this module
    import sys
    del sys.modules['ledmat']
    import ledmat
