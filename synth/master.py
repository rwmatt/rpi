import RPi.GPIO as GPIO
from time import sleep

def usleep(x): sleep(x/1000000.0)

SND_SS = 16
SND_CLK = 20
SND_BYTE = 21

def setup():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(SND_CLK, GPIO.OUT, initial = 0)
    GPIO.setup(SND_BYTE, GPIO.OUT, initial = 0)
    GPIO.setup(SND_SS, GPIO.OUT, initial = 1)
    #for p in [SND_SS, SND_CLK, SND_BYTE]: GPIO.setup(p, GPIO.OUT, initial=0)

def send_freq(f, delay = 1):
        barr = "0" * 16 + bin(f)[2:]
        barr = barr[-16:]
        barr = list(barr)
        barr = [1 if b == '1' else 0 for b in barr]
        #print(barr)
        #barr = list(barr)
        #barr = barr[-16:0]
        #print(barr)
        GPIO.output(SND_CLK, 0);
        GPIO.output(SND_SS, 0)
        for b in barr:
            #b = 1 if b == '1' else 0
            GPIO.output(SND_BYTE, b)
            GPIO.output(SND_CLK, 1)
            usleep(delay)
            GPIO.output(SND_CLK, 0)
            usleep(delay)
        GPIO.output(SND_SS, 1)

def scale():
    for n in range(4):
        n = 400*(n+1)
        send_freq(n)
        sleep(2)
    send_freq(0)



def main():
    try:
        setup()
        scale()
    finally:
        GPIO.cleanup()

if __name__ == "__main__":
    main()
