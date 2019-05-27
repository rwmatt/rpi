
#%serialconnect to --port /dev/ttyUSB0

import machine
import network
import ntptime
import time
import utime
from machine import Pin, SPI, I2C
from utime import sleep_ms, ticks_ms

import urtc

import settings # personal settings which contain Wifi info

buzzer = Pin(16, Pin.OUT) # D0
buzzer.off()

class Every:
    def __init__(self, interval_ms, func = None ):
        self.start = utime.ticks_ms()
        self.interval_ms = interval_ms
        self.func = func
        
    def rising(self):
        now =utime.ticks_ms()
        if now<self.start: self.start = now
        if now - self.start < self.interval_ms: return False
        self.start = now
        return True
    
    def update(self):
        if not self.rising(): return
        #print("Updating")
        self.func() 

def do_connect():
    import network
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(settings.wifi_essid, settings.wifi_password)
        while not sta_if.isconnected():
            pass
    print('network config:', sta_if.ifconfig())

do_connect()

#rtc = machine.RTC()
i2c = I2C(scl=Pin(5), sda=Pin(4))
rtc = urtc.DS1307(i2c)
print("Inital rtc datetime", rtc.datetime())

# http://my-small-projects.blogspot.com/2015/05/arduino-checking-for-british-summer-time.html
def IsBST(yr, imonth, iday, hr):
    #January, february, and november are out.
    if (imonth < 3 or imonth > 10): return False
    #April to September are in
    if (imonth > 3 and imonth < 10): return True

    #find last sun in mar and oct - quickest way I've found to do it
    #last sunday of march
    lastMarSunday =  (31 - (5* yr /4 + 4) % 7)
    #last sunday of october
    lastOctSunday = (31 - (5 * yr /4 + 1) % 7)
        
    #In march, we are BST if is the last sunday in the month
    if (imonth == 3):      
      if ( iday > lastMarSunday):
        return True
      if ( iday < lastMarSunday):
        return False
      
      if (hr < 1):
        return False
              
      return True; 
  
    
    #In October we must be before the last sunday to be bst.
    #That means the previous sunday must be before the 1st.
    if (imonth == 10):

      if ( iday < lastOctSunday):
        return True
      if ( iday > lastOctSunday):
        return False
      
      if (hr >= 1):
        return false;
        
      return True 
    

def adjustBST(yr, imonth, iday, hr):
    if not IsBST(yr, imonth, iday, hr):
        return imonth, iday, hr
    daysInMonth = { 3:31, 4:30, 5:31, 6:30,  7:31, 8:31, 9:30, 10:31 }
    hr += 1
    if hr == 24:
        hr = 1
        iday += 1
        if iday> daysInMonth[imonth]:
            iday = 1
            imonth +=1
    return imonth, iday, hr
              
#print(adjustBST(2019, 5, 31, 23))
#print(adjustBST(2019, 2, 24, 20))




def set_rtc():
    ts = ntptime.time()
    tm = time.localtime(ts)
    #print(ts1)
    tm = tm[0:3] + (0,) + tm[3:6] + (0,) # odd rearrangement of fields. Sight
    rtc.datetime(tm)
    print("set_rtc:", time.localtime(ts))
    #print(rtc.datetime())

set_rtc()

spi = None
cs_pin = None

def maxTransfer(address, value):
    global cs_pin, spi
    cs_pin.off()
    spi.write(bytearray([address, value]))
    cs_pin.on()

def init_display():
    global cs_pin, spi
    cs_pin = Pin(15, Pin.OUT) # aka SS slave select
    cs_pin.on()
    spi = SPI(sck=Pin(14), mosi=Pin(13), miso=Pin(12))
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
        if i == 2: hi |= (1 << 7) # decimal point
        maxTransfer(7-2*i, hi)
        maxTransfer(8-2*i, lo)                        
 
# set_display([19, None, 85, 76])

def display_rtc():
    yr , imonth, iday, _ , hr, mint, _, _ = rtc.datetime()
    #print(mint)
    imonth, iday, hr = adjustBST(yr, imonth, iday, hr)
    set_display([iday, None, hr, mint])
#    return y, mnth, day, hr, mint
#print(display_rtc())
display_rtc()



timing = False
timer_start = None

def elapsed_time():
    global timer_start
    secs = int((ticks_ms() - timer_start)/1000)
    mins = int(secs/60)
    secs = secs % 60
    return mins, secs
    
def timer_expired():
    mins, _ = elapsed_time()
    return mins >= 30


def print_elapsed_time():
    mins, secs = elapsed_time()
    set_display([None, None, mins, secs])
    
def display():
    global timing
    if timing:
        print_elapsed_time()
    else:
        display_rtc()
        #write_to_0seg()
        
def change_major_mode():
    global timing, timer_start
    #print("change_major_mode")
    timing = not timing;
    if timing:
        timer_start = ticks_ms()
    #    print_elapsed_time()
    #else:
    #    write_to_0seg()

btn = Pin(2, Pin.IN, Pin.PULL_UP) # D4
btnState = 0
released = True
def tmrButton():
    #print("tmrButton")
    global btnState, released
    btnState = (btnState << 1) + (1 if btn.value() == 0 else 0)
    btnState = btnState % 0x100
    #print("btmState", btnState)
    if btnState == 0xFF and released:
        released = False
        change_major_mode()
    if btnState == 0x00: released = True
    
#btn_state = None
#btn_ms = ticks_ms()
#def btn_handler(p):
#    global btn_state, btn_ms
#    if ticks_ms() - btn_ms < 20: return
#    btn_ms = ticks_ms()
#    btn_state = btn.value()
    
#btn.irq(trigger=Pin.IRQ_RISING | Pin.IRQ_FALLING, handler = btn_handler)

class Duration:
    def __init__(self, ms):
        self.start = ticks_ms()
        self.ms = ms
        
    def more(self):
        return ticks_ms() - self.start < self.ms

def test_button():
    global btn_state
    print("test_button ...")
    dure = Duration(30* 1000)
    while dure.more():
        if btn_state == 0:
            btn_state = None
            print("Falling detected")
    print("Finished")

#test_button()

def beep():
    buzzer.on()
    sleep_ms(100)
    buzzer.off()
    sleep_ms(100)
    
def buzz():
    global timing
    if not timing: return
    beep()
    if timer_expired(): beep()

# main loop
display_rtc()
#ev = Every(5000, display_rtc)
ev_set_rtc = Every(1000 * 60 * 60 *24, set_rtc) # every 24 hours
#ev_set_rtc = Every(1000 * 60 * 5, set_rtc) # every 5 mins - for testing
ev_buzz = Every(5000, buzz)
ev_tmr = Every(3, tmrButton)
ev_display = Every(500, display)
def loop():
    while True:
        #global timing
        ev_set_rtc.update()
        ev_buzz.update()  
        ev_tmr.update()
        ev_display.update()
loop()
