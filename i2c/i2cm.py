import smbus, time

bus = smbus.SMBus(1)
addr = 0x4


while True:
    b = bus.read_byte(addr)
    print(b)
    time.sleep(0.08)

bus.close()
