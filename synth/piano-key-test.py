# key test for down and up to resolve key repeating issues
import datetime, os, sys, pygame
from time import sleep

#from master import usleep
#os.system('sleep 1; xset r off')

pygame.init()
scr = pygame.display.set_mode((200,200))
#delay = 1000 * 1000 # num milliseconds
#pygame.key.set_repeat(delay, delay)

def notify(msg):
    print(datetime.datetime.now().time(), 'key', msg)

def main():
    while 1:
        #keys = pygame.key.get_pressed()
        #if keys[pygame.K_Q]:
        #    print('pressed key
        #print(keys)
        #pygame.event.pump()
        #sleep(2)
        #continue
        events = pygame.event.get()
        #pygame.event
        for ev in events:
            if ev.type == pygame.QUIT: return
            if ev.type == pygame.KEYDOWN:
                notify('down')
                c = pygame.key.name(ev.key)
                if c == 'q': return
            if ev.type == pygame.KEYUP:
                notify('up')


if __name__ == "__main__": main()        

