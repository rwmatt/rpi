import sys
import select
import time

def heardEnter():
    i,o,e = select.select([sys.stdin],[],[],0.0001)
    for s in i:
        if s == sys.stdin:
            inp = sys.stdin.readline()
            return True, inp
    return False, None

def main():
    start = time.time()
    def loop():
        global start
        resp, inp = heardEnter()
        if resp:
            print('You said:', inp)
        end = time.time()
        if(end-start>5):
            start =end
            print(".",)
    while True: loop()
