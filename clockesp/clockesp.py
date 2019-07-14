


import machine
import time
from machine import Pin, SPI, RTC
import ntptime
from utime import sleep_ms, ticks_ms, ticks_diff
import machine
import socket

import mel
#from mel import adjustBST

heart = Pin(16, Pin.OUT) # D0, but internal LED
heart.on() # counterintuively, for the internal LED, ON means OFF
sw1 = Pin(4, Pin.IN, Pin.PULL_UP) # D2
sw2 = Pin(5, Pin.IN, Pin.PULL_UP) # D1
buzzer = Pin(0, Pin.OUT) # D3
buzzer.off()
cs_pin = Pin(15, Pin.OUT) # aka SS slave select, D8
cs_pin.on()

# maybe put in boot mode
if(sw2.value() == 0):
    while True:
        sleep_ms(1000)

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
set_display([None, None, None, None])

#def write_digit(pos, digit, dp = False):
#    maxTransfer()
def show_status(num):
    for i in range(3):
        num, digit = divmod(num, 10)
        maxTransfer(6+i, digit)


class Every:
    def __init__(self, interval_ms, func = None ):
        self.start = utime.ticks_ms()
        self.interval_ms = interval_ms
        self.func = func
        self.first = True
        
    def rising(self):
        now =utime.ticks_ms()
        if now<self.start: self.start = now
        if now - self.start < self.interval_ms: return False
        self.start = now
        return True
    
    def update(self):
        if self.first:
            self.first = False
        elif not self.rising(): 
            return
        self.func() 


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

rtc = RTC()

def display_time():
    yr , imonth, iday, _ , hr, mint, _, _ = rtc.datetime()
    #imonth, iday, hr = mel.adjustBST(yr, imonth, iday, hr)
    #set_display([iday, None, hr, mint])
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
    return # turn it off for now
    heart.off()
    utime.sleep_ms(10)
    heart.on()

# watchdog functions
import machine
dog_started = False
dog_start_time = None
killed=False
def _killer():
    #print('*',)
    global dog_started, dog_start_time, killed
    #killed = True
    if not dog_started: return
    if ticks_diff(ticks_ms(), dog_start_time) > 10000:
        machine.reset() # too long. Just kill the whole MCU
        killed = True
        #print("You're terminated!")
    
def killer(dog): _killer()
def start_killer():
    global dog_started, dog_start_time, killed
    state = machine.disable_irq()
    dog_started = True
    dog_start_time = ticks_ms()
    killed = False
    machine.enable_irq(state)
    
def stop_killer():
    global dog_started, dog_start_time, killed
    state = machine.disable_irq()
    dog_started = False
    dog_start_time = None
    killed = False
    machine.enable_irq(state)

  
# ensure the dog is set up only once, otherwise we'll get an OSError 261
activate_dog = not ('dog' in locals())
#activate_dog = False
if activate_dog:
    dog = machine.Timer(0)
    dog.init(period = 1000, mode=machine.Timer.PERIODIC, callback = killer)

sta = None

def do_connect(delay_ms = 100):
    import network
    import settings
    show_status(401)
    start_killer()
    sta = network.WLAN(network.STA_IF)
    if not sta.isconnected():
        print('connecting to network...')
        sta.active(True)
        sta.connect(settings.wifi_essid, settings.wifi_password)
        while not sta.isconnected():
            sleep_ms(delay_ms)
    stop_killer()
    return sta

#if sta is not None:
#    sta = do_connect()
do_connect()


def use_local():
    try:
        s = socket.socket()
        addr = socket.getaddrinfo("192.168.0.17", 1762)[0][-1]
        s.connect(addr)
        data = s.recv(100)
    finally:
        s.close()
    
    fields = data.decode().split(" ")
    fields = tuple(map(int, fields))
    rtc = machine.RTC()
    rtc.datetime(fields)

def update_ntp():
    show_status(402)
    start_killer()
    #ntptime.host = '192.168.0.17'
    #ntptime.settime()
    use_local()    
    stop_killer()
    show_status(403)
    update_display()


#from machine import Timer
import utime
def loop():
    global timing
    #update_ntp()
    #display_time()
    p = Pauser()
    p.pause(button_pressed, condition = lambda: sw1.value() == 0)
    ev_buzzer = Every(5000, update_buzzer)
    ev_ntp = Every(1000*60*30, update_ntp)
    ev_display = Every(500, update_display)
    ev_heartbeat = Every(3000, update_heartbeat)
    while True:
        if not timing:
            ev_ntp.update()
        p.update()
        ev_buzzer.update()
        ev_display.update()
        ev_heartbeat.update()
loop()    


