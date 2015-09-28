.PHONY: all

DEPS=MsgClient

all: $(DEPS) test

MsgClient: msg_client.c 
	gcc msg_client.c -o msgclient

test:
	./msgclient -s 127.0.0.1 -p 5555 -n Pradeep -m "Hello World"

clean: 
	rm msgclient
