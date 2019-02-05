#import socket module
from socket import *
import codecs
import httplib2
import fcntl
import struct
import sys # In order to terminate the program

def main():
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
        try:
            with open(filename[1:], 'r') as f:  
                outputdata = f.read()
                #Send one HTTP header line into socket
                c.send(b'HTTP/1.0 200 OK\r\n\r\n'.encode())
                
                c.send(outputdata.encode())
                c.send("\r\n".encode())
        except (OSError, IOError) as e:
                c.send(b'HTTP/1.0 404 Not Found\r\n\r\n'.encode())
    # Close the connection with the client
    print('Closing connection with the client')
    c.close()
    serverSocket.close()

def get_ip_address(s, ifname):
    return inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

if __name__ == "__main__":
    main()
