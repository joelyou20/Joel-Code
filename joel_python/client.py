#import socket module
from socket import *
import sys # In order to terminate the program

def main():

    addr, port, htmlFile = getServerInfo()

    clientSocket = socket(AF_INET, SOCK_STREAM)

    clientSocket.connect((addr, port))

    clientSocket.sendall("GET /{} HTTP/1.1".format(htmlFile))

    data = clientSocket.recv(10000)

    print(data)

    clientSocket.close()

def getServerInfo():
    addr = sys.argv[1]
    port = sys.argv[2]
    htmlFile = sys.argv[3]

    return (addr, int(port), htmlFile)


if __name__ == "__main__":
    main()