# CS 6421 - Simple Message Board Client in Python
# T. Wood
# Run with:     python msgclient.py

import socket

#host = "twood02.koding.io";
host = "127.0.0.1";
portnum = 5555;

# your code here!
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect((host, portnum))
clientsocket.send("Pradeep\nHello")
print("Sent message to server!")
