# taken from
# https://pythontic.com/modules/socket/udp-client-server-example
import socket

import sounddevice as sd
import numpy as np

import sowsettings as ss

#arr = np.array( dtype = np.uint8)
arr = np.fromfile("song.raw", dtype =np.uint8)
new_size = (len(arr) // 128 + 1) * 256
#print(new_size)
arr1 = np.zeros(new_size, dtype = np.uint8)
for i in range(0, len(arr)):
    arr1[2*i] = arr[i]

#sd.play(arr,samplerate=44100)
#sd.wait()

block = 0
nblocks = int(new_size / 256)
print("Number of blocks of size 256:", nblocks)

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
    #message = bytesAddressPair[0]
    address = bytesAddressPair[1]
    #clientMsg = "Message from Client:{}".format(message)
    #clientIP  = "Client IP Address:{}".format(address) 
    #print(clientMsg)
    #print(clientIP) 
    # Sending a reply to client
    #UDPServerSocket.sendto(bytesToSend, address)
    UDPServerSocket.sendto(msg, address)

#respond(str.encode(str(nblocks)))

while(True): 
    for block in range(nblocks):
        packet = arr1[block*256:(1+block)*256]
        respond(packet)
