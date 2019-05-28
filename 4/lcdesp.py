
%serialconnect to --port=/dev/ttyUSB0

from machine import Pin, SPI
from utime import sleep_ms

rs_pin = Pin(2, Pin.OUT) # Pin D4. Do NOT use the regular MISO pin (D6)
cs_pin = Pin(15, Pin.OUT) # aka SS slave select
cs_pin.on()

#spi = SPI(sck=Pin(14), mosi=Pin(13), miso=Pin(12))
spi = SPI(1)
spi.init(phase = 0)
spi.init(baudrate=400000)


def write_val(csv, c):
    global cs_pin, rs_pin, spi
    rs_pin.value(csv)
    cs_pin.off()
    spi.write(bytearray([c]))
    #print(nbytes)
    cs_pin.on()
    sleep_ms(60)


contrast = 0x70  + 0b1000 # from 0x7C
display = 0b1111 # ori 0x0F
init = [0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06]
for c in init: write_val(0, c)


for c in ['H', 'E', 'L', 'L', 'O']: write_val(1, ord(c))

spi.deinit()


