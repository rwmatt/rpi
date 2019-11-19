import socket
import time

import sowsettings as ss

#msgFromClient       = "Hello UDP Server"
bytesToSend         = str.encode("SND")
serverAddressPort   = (ss.host, 20001)
bufferSize          = 256
  

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
 
# Send to server using created UDP socket
UDPClientSocket.sendto(bytesToSend, serverAddressPort)
msgFromServer = UDPClientSocket.recvfrom(bufferSize)
#msg = "Message from Server {}".format(msgFromServer[0])
msg = msgFromServer[0]
#print(msg)
nblocks = int(msg)
print("Client thinks the number of blocks to receive is:", nblocks)

start = time.time()
for blk in range(nblocks):
    #print(blk)
    UDPClientSocket.sendto(bytesToSend, serverAddressPort)
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    #msg = "Message from Server {}".format(msgFromServer[0])
    msg = msgFromServer[0]
end = time.time()
print("Time taken to receive packets:", end - start, " seconds")

