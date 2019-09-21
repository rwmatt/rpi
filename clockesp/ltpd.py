#!/usr/bin/env  python3
import socketserver
import time

class MyTCPHandler(socketserver.BaseRequestHandler):
    """
    The request handler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        resp = time.strftime("%Y %m %d %u %H %M %S 0\n", time.localtime())
        resp = str.encode(resp)
        self.request.sendall(resp)
        return

        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        print("{} wrote:".format(self.client_address[0]))
        print(self.data)
        # just send back the same data, but upper-cased
        self.request.sendall(self.data.upper())

if __name__ == "__main__":
    HOST, PORT = "", 1762

    # Create the server, binding to localhost on port PORT
    #with socketserver.TCPServer((HOST, PORT), MyTCPHandler) as server:
    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()


