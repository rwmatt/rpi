# taken from
# https://pythontic.com/modules/socket/udp-client-server-example
import socket

import sounddevice as sd
import numpy as np

import sowsettings as ss

#arr = np.array( dtype = np.uint8)
arr = np.fromfile("song8k.raw", dtype =np.uint8)
new_size = (len(arr) // 512 + 1) * 512
#print(new_size)
arr1 = np.zeros(new_size, dtype = np.uint8)
for i in range(len(arr)):
    arr1[i] = arr[i]


block = 0
nblocks = int(new_size / 512)
print("Number of blocks of size 512:", nblocks)

localIP     = ss.host
localPort   = ss.port
bufferSize  = 32

msgFromServer       = "Hello UDP Client"
bytesToSend         = str.encode(msgFromServer)
print("Ready to send")

# Create a datagram socket
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# Bind to address and ip
UDPServerSocket.bind((localIP, localPort))
#print("UDP server up and listening")
# Listen for incoming datagrams

def respond(msg):
    global bufferSize
    bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
    address = bytesAddressPair[1]
    UDPServerSocket.sendto(msg, address)

#respond(str.encode(str(nblocks)))

while(True): 
    for block in range(nblocks):
        packet = arr1[block*512:(1+block)*512]
        respond(packet)
