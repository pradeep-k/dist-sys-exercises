import socket
import sys

print_server_ip = raw_input("Please enter print server IP address: ");
print_server_port = raw_input("Please enter print server port: ");
kg_g_ip = raw_input("Please enter kg g server IP address: ");
kg_g_port = raw_input("Please enter kg g server port: ");
kg_pound_ip = raw_input("Please enter kg pound server IP address: ");
kg_pound_port = raw_input("Please enter kg pound server port: ");
result_string1 = " ";
result_string2 = " ";
result_string = " ";

## Function to process requests
def process(conn):
     conn.send("Welcome, you are connected to a Python-based server\n")
     #global variable for storing initial input format
     global result_string
     global result_string1
     global result_string2
     # read userInput from client
     userInput = conn.recv(BUFFER_SIZE)
     if not userInput:
         print "Error reading message"
         sys.exit(1)

     print "Received message: ", userInput
    
     conn.close()

     mylist = userInput.split(" ")
     #in sock, 1 or 2 means the times of certain message comes to proxy server
     if mylist[3] != None:
	#input from client
	if mylist[3] == "c":
		result_string1 = mylist[0]
		result_string2 = mylist[1]
		if mylist[0]=="g":
			if mylist[1] == "lbs":
     				mylist[1]="kg"
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);  
			sock.connect((kg_g_ip, int(kg_g_port))); 
			sock.send(mylist[0]+" "+mylist[1]+" "+mylist[2]+" "+"1");
			sock.close();
    		if mylist[0]=="lbs":
			if mylist[1] == "g":
				mylist[1]="kg"
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);  
			sock.connect((kg_pound_ip, int(kg_pound_port))); 
			sock.send(mylist[0]+" "+mylist[1]+" "+mylist[2]+" "+"1");
			sock.close();
		if mylist[0]=="kg" and mylist[1]=="g":
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);  
			sock.connect((kg_g_ip, int(kg_g_port))); 
			sock.send(mylist[0]+" "+mylist[1]+" "+mylist[2]+" "+"1");
			sock.close();
		if mylist[0]=="kg" and mylist[1]=="lbs":
			sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);  
			sock.connect((kg_pound_ip, int(kg_pound_port))); 
			sock.send(mylist[0]+" "+mylist[1]+" "+mylist[2]+" "+"1");
			sock.close();
	#input from other servers
     	if mylist[3] != "c": 
        	if mylist[4] == "n":
	   		if (result_string1=="g" or result_string1=="lbs") and (result_string2=="g" or result_string2=="lbs"):
				if result_string1=="g":
					sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);  
					sock.connect((kg_pound_ip, int(kg_pound_port))); 
					sock.send("kg"+" "+"lbs"+" "+mylist[2]+" "+"2");
					sock.close();
				else:
					sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);  
					sock.connect((kg_g_ip, int(kg_g_port))); 
					sock.send("kg"+" "+"g"+" "+mylist[2]+" "+"2");
					sock.close();
		#present outputs for lbs->g or g->lbs
                #other outputs, such as kg->g or lbs->kg will be directly shown on the screen
       		else:
			result_string = result_string1+" "+result_string2+" "+mylist[2];
			print result_string
    


### Main code run when program is started
BUFFER_SIZE = 1024
interface = ""

# if input arguments are wrong, print out usage
if len(sys.argv) != 2:
    print >> sys.stderr, "usage: python {0} portnum\n".format(sys.argv[0])
    sys.exit(1)

portnum = int(sys.argv[1])

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect((print_server_ip, int(print_server_port)))
clientsocket.send("Proxy server registered\n")
print("Sent message to print server!\n")

# create socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((interface, portnum))
s.listen(5)

while True:
    # accept connection and print out info of client
    conn, addr = s.accept()
    print 'Accepted connection from client', addr
    process(conn)
s.close()
