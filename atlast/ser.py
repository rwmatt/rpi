import time
import serial

import keys

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
    #parity=serial.PARITY_ODD,
    #
    #stopbits=serial.STOPBITS_TWO,
    #bytesize=serial.SEVENBITS
)

#ser.open()
ser.isOpen()

trans = open("trans.txt", "a")

print('Enter your commands below.\r\nInsert "exit" to leave the application.')

def do_input():
    resp, data = keys.heardEnter()
    if not resp: return
    serdata = str.encode(data)
    ser.write(serdata)
    trans.write(data)
    trans.flush()
    # maybe flush?
    # get keyboard input
    #input = raw_input(">> ")
    #    # Python 3 users
    #    # input = input(">> ")
    #if input == 'exit':
    #    ser.close()
    #    exit()

def do_output(): 
    # send the character to the device
    # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
    #ser.write(input + '\n')
    #ser.flush()
    #out = ''
    # let's wait one second before reading output (let's give device time to answer)
    #time.sleep(0.5)
    #response = yypser.readline()
    n = ser.in_waiting
    if n == 0: return
    data = ser.read(n)
    data = data.decode() # turn bytes int a string
    print(data, end = '')
    #while True:
    #    response = ser.read_until()
    #    print(response,)
    #    if response == "  ok\n": break

#input=1
def inner_loop():
    do_input()
    do_output()

def loop(): 
    try: 
        while True: 
            inner_loop()
    finally:
        trans.close()

loop()
