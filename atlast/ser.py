import time
import serial

import keys

class Port:
    def __init__(self):
        self.port = serial.Serial( port='/dev/ttyUSB0', baudrate=115200)
    def write(self, data): self.port.write(data)
    def __del__(self):
        print("Closing port")
        self.port.close()


#ser.open()
#ser.isOpen()

#trans = open("trans.txt", "a")

#print('Enter your commands below.\r\nInsert "exit" to leave the application.')

def send(p, data):
    serdata = str.encode(data)
    p.write(serdata)

def do_input(p):
    resp, data = keys.heardEnter()
    if not resp: return
    send(p, data)
    #trans.write(data)
    #trans.flush()
    # maybe flush?
    # get keyboard input
    #input = raw_input(">> ")
    #    # Python 3 users
    #    # input = input(">> ")
    #if input == 'exit':
    #    ser.close()
    #    exit()

def do_output(p): 
    # send the character to the device
    # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
    #ser.write(input + '\n')
    #ser.flush()
    #out = ''
    # let's wait one second before reading output (let's give device time to answer)
    #time.sleep(0.5)
    #response = yypser.readline()
    n = p.port.in_waiting
    if n == 0: return
    data = p.port.read(n)
    data = data.decode() # turn bytes int a string
    print(data, end = '')
    #while True:
    #    response = ser.read_until()
    #    print(response,)
    #    if response == "  ok\n": break

#input=1
def inner_loop(p):
    do_input(p)
    do_output(p)

def main(): 
    try:
        p = Port()
        while True: 
            inner_loop(p)
    finally:
        #trans.close()
        pass

if __name__ == "__main__":
    main()
