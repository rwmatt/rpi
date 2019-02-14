import serial

def main():
    ser = serial.Serial('/dev/ttyACM0', 9600)
    #ser.open()
    #ser.write([c for c in '?\n'])
    #ser.write('?\n'.encode("ascii"))
    ser.write(b'?\n')
    ser.flush()
    #for i in range(10):
    print(ser.readline())
    ser.close()
    pass

if __name__ == "__main__":
    main()
