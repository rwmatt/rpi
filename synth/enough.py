import RPi.GPIO as GPIO
import master
from master import send_freq
from time import sleep

notes = """
0 0
G3 196
A3 220
B3 247
C4 262
"""

s1 = """
G3 1
G3 11
G3 12
G3 12
G3 12
G3 11
A3 11
B3 11
"""


s2 = """
G3 1
G3 11
G3 12
C4 12
C4 12
B3 11
A3 11
B3 11
"""

score = s1*3 + """
C4 1
B3 1
A3 1
B3 1
""" + s1 * 3 + s2 + s1 * 3 + s2

nmap = {} # notes map
for n in notes.splitlines():
    if len(n) == 0: continue
    #n = n.strip()
    note, freq = n.split()
    nmap[note] = int(freq)
    #print(n)

durs = { "1" : 1, "11" : 0.5, "12" : 0.25, "13" : 0.125 }

delay = 1

def play_score(score):
    for note in score.splitlines():
        if len(note) == 0: continue
        n, dur = note.split()
        freq = nmap[n]
        send_freq(freq, delay)
        duration = 0.4 * durs[dur]
        sleep(duration)
        send_freq(0, delay)
        sleep(0.07)


def main():
    try:
        master.setup()
        play_score(score)
    finally:
        GPIO.cleanup()

if __name__ == "__main__":
    main()
