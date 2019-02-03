#import socket module
from socket import *
import codecs
import fcntl
import urllib3
import struct
import sys # In order to terminate the program

def main():
    #try:

        # next create a socket object
        serverSocket = socket(AF_INET, SOCK_STREAM)
        c = None
        print "Socket successfully created"

        # reserve a port on your computer in our
        # case it is 12345 but it can be anything
        port = 12345

        # Next bind to the port
        # we have not typed any ip in the ip field
        # instead we have inputted an empty string
        # this makes the server listen to requests
        # coming from other computers on the network
        serverSocket.bind(('', port))
        print "socket binded to %s" %(port)

        # put the socket into listening mode
        serverSocket.listen(1)
        print "socket is listening"

        # a forever loop until we interrupt it or
        # an error occurs
        while True:

            # Establish connection with client.
            c, addr = serverSocket.accept()
            ip = get_ip_address(serverSocket, 'enp0s31f6')
            print 'Got connection from', addr

            message = c.recv(10000)
            filename = message.split()[1]            
            f = open(filename[1:], 'r')
            outputdata = f.read()
            http = urllib3.PoolManager()
            r = http.request('GET', 'hello.html')
            print(r.status)
            #Send one HTTP header line into socket
            c.send(outputdata)

            #Send the content of the requested file to the client
            #for i in range(0, len(outputdata)):
            #    print(outputdata[i])
            #    c.send(outputdata[i].encode())
            #c.send("\r\n".encode())
            
            # Close the connection with the client
            print('Closing connection with the client')
            c.close()
    #except Exception as e:
     #   print(e)
     #   serverSocket.close()
    #    if c is not None:
     #       c.close()

def get_ip_address(s, ifname):
    return inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

if __name__ == "__main__":
    main()
