import socket
import time

import sowsettings as ss

import sounddevice as sd
import numpy as np

#msgFromClient       = "Hello UDP Server"
bytesToSend         = str.encode("SND")
serverAddressPort   = (ss.host, ss.port)
bufferSize          = 512
  

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
 

buffer = b""
start = time.time()
for blk in range(4096):
    #print(blk)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    msg = msgFromServer[0]
    buffer += msg
end = time.time()
print("Time taken to receive packets:", end - start, " seconds")

arr = np.frombuffer(buffer, dtype = np.uint8)
sd.play(arr, samplerate=8000)
sd.wait()
