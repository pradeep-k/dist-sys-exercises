# CS 6421 - Simple Message Board Client in Python
# T. Wood
# Run with:     python msgclient.py

import socket
import sys, getopt 

def main(argv):
    host = "127.0.0.1";
    #host = "upkk5920aab9.pradeep-k.koding.io";
    portnum = 5555;
    name = "Pradeep";
    message = "Hello World";
    
    try:
        opts, args = getopt.getopt(argv,"h:s:p:n:m:")
    except getopt.GetoptError:
        print 'msgclient.py -s <server> -p <port> -n <name> -m <message>'
        sys.exit()

    for opt, arg in opts:
        if opt == '-h':
            print 'msgclient.py -s <server> -p <port> -n <name> -m <message>'
            sys.exit()
        elif opt == '-s':
            host = arg
            #print host
        elif opt == '-p':
            portnum = arg
            #print portnum
        elif opt == '-n':
            name = arg
            #print name
        elif opt == '-m':
            message = arg
            #print msg

    # your code here!
    clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientsocket.connect((host, portnum))
    clientsocket.send(name)
    clientsocket.send("\n")
    clientsocket.send(message)

    print("Sent message to server!")

if __name__ == "__main__":
    main(sys.argv[1:])
