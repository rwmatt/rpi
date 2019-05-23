import spidev
import RPi.GPIO as GPIO
from time import sleep
GPIO.setmode(GPIO.BOARD) # use physical numberings

def foo():
    #cs_pin = 24
    #GPIO.setup(cs_pin, GPIO.OUT)
    rs_pin = 21
    GPIO.setup(rs_pin, GPIO.OUT)
    LOW = 0
    HIGH = 1

    bus = 0
    device = 0
    spi = spidev.SpiDev()
    spi.open(bus, device)
    spi.max_speed_hz = 400000
    spi.mode = 0

    def cmd_write(v):
        #GPIO.output(cs_pin, LOW)
        GPIO.output(rs_pin, LOW)
        spi.xfer2(bytearray(v))
        #GPIO.output(cs_pin, HIGH)
        sleep(0.06)



    contrast = 0x70  + 0b1000 # from 0x7C
    display = 0b1111 # ori 0x0F
    init = bytearray([0x39, 0x1D, 0x50, 0x6C, contrast, 0x38, display, 
        0x01, 0x06])
    init = [0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06]
    #for v in init: cmd_write(v)
    cmd_write(init)

    def write_datum(v):
        #GPIO.output(cs_pin, LOW)
        GPIO.output(rs_pin, HIGH)
        spi.xfer(bytearray(v))
        #GPIO.output(cs_pin, HIGH)

    write_datum([ord('H'), ord('E'), ord('L'), ord('L'), ord('O')])
    #write_datum(ord('E'))

    #spi.xfer(init)
    #T = ord('T')

    #print(T)
    #msg = [T, T, T , T]
    #print(spi.xfer2(msg))
    #msg = [ord('T')]
    #print(spi.xfer2(msg))
    #spi.xfer([0b00010001])
    spi.close()

try: 
    foo()
finally:
    GPIO.cleanup()
