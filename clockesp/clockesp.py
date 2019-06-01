#import machine
#import network
from ntptime import settime
import time
#import utime
from machine import Pin, SPI, RTC
#import uasyncio as asyncio
from utime import sleep_ms, ticks_ms, ticks_diff
#import urtc

import mel
from mel import Every, adjustBST
#import settings # personal settings which contain Wifi info

heart = Pin(16, Pin.OUT) # D0, but internal LED
heart.on() # counterintuively, for the internal LED, ON means OFF
sw1 = Pin(4, Pin.IN, Pin.PULL_UP) # D2
sw2 = Pin(5, Pin.IN, Pin.PULL_UP) # D1
buzzer = Pin(0, Pin.OUT) # D3
buzzer.off()
cs_pin = Pin(15, Pin.OUT) # aka SS slave select, D8
cs_pin.on()

# set up display
spi = SPI(1)
spi.init(phase = 0)
spi.init(baudrate=400000)

def maxTransfer(address, value):
    global cs_pin, spi
    cs_pin.off()
    spi.write(bytearray([address, value]))
    cs_pin.on()

def init_display():
    #global cs_pin, spi   
    #cs_pin.on()
    #spi = SPI(sck=Pin(14), mosi=Pin(13), miso=Pin(12))
    maxTransfer(0x0F, 0x00)
    maxTransfer(0x09, 0xFF) # enable mode B
    maxTransfer(0x0A, 0x0F) # set intensity
    maxTransfer(0x0B, 0x07) # use all pins
    maxTransfer(0x0C, 0x01) # turn on chip

init_display()

def set_display(list4):
    #pos = 8
    for i in range(4):
        n = list4[i]
        if n is None: # blankify
            hi = 0b1111
            lo = 0b1111
        else:
            hi = n % 10
            lo = int(n / 10)
        if i == 2:
            hi |= (1 << 7) # decimal point
        maxTransfer(7-2*i, hi)
        maxTransfer(8-2*i, lo)                        
 
#set_display([19, None, 85, 76])

#def write_digit(pos, digit, dp = False):
#    maxTransfer()
def show_status(num):
    for i in range(3):
        num, digit = divmod(num, 10)
        maxTransfer(6+i, digit)


class Pauser:
    def __init__(self):
        self.callback = None
        self.condition = None
        self.start = None
        self.delay_ms = None
    
    def pause(self, callback, condition = None, delay_ms = 0):
        self.callback = callback
        self.condition = condition
        self.start = ticks_ms()
        self.delay_ms = delay_ms
        
    def update(self):
        if self.callback == None: return
        if ticks_diff(ticks_ms(), self.start) < self.delay_ms:
            return
        try: 
            triggered = self.condition()
        except TypeError:
            triggered = True
        if triggered:
            fn = self.callback
            self.callback = None
            fn(self)

show_status(401)
mel.do_connect()

rtc = RTC()

def update_ntp():
    show_status(402)
    settime()
    show_status(403)
#print(rtc.datetime())


def display_time():
    yr , imonth, iday, _ , hr, mint, _, _ = rtc.datetime()
    #print(mint)
    imonth, iday, hr = mel.adjustBST(yr, imonth, iday, hr)
    set_display([iday, None, hr, mint])

timing = False
timer_start = None

def change_major_mode():
    global timing, timer_start
    timing = not timing;
    if timing:
        timer_start = ticks_ms()

def elapsed_time():
    global timer_start
    secs = int(utime.ticks_diff(ticks_ms(), timer_start)/1000)
    mins = int(secs/60)
    secs = secs % 60
    return mins, secs
    
def timer_expired():
    mins, _ = elapsed_time()
    return mins >= 30



def beep():
    buzzer.on()
    sleep_ms(100)
    buzzer.off()
    sleep_ms(100)

        
def update_buzzer():
    global timing
    if timing:
        beep()
        if timer_expired(): beep()


def update_display():
    global timing
    if timing:
        mins, secs = elapsed_time()
        set_display([None, None, mins, secs])
    else:
        display_time()



def button_pressed(pauser):
    #print('Button pressed')
    change_major_mode()
    pauser.pause(button_released, condition = lambda: sw1.value() == 1, 
                 delay_ms = 20)
    

def button_released(pauser):
    #print("Button released")
    pauser.pause(button_pressed, condition = lambda: sw1.value() == 0, 
                 delay_ms = 20)    

def update_heartbeat():
    heart.off()
    utime.sleep_ms(10)
    heart.on()

#from machine import Timer
import utime
def loop():
    update_ntp()
    display_time()
    p = Pauser()
    p.pause(button_pressed, condition = lambda: sw1.value() == 0)
    ev_buzzer = Every(5000, update_buzzer)
    ev_clock = Every(1000*60*30, update_ntp)
    ev_display = Every(500, update_display)
    ev_heartbeat = Every(3000, update_heartbeat)
    while True:
        ev_clock.update()
        p.update()
        ev_buzzer.update()
        ev_display.update()
        ev_heartbeat.update()
loop()    

