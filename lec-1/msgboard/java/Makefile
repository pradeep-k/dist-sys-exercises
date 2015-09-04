.PHONY: all

DEPS=MsgClient

all: $(DEPS) test

MsgClient: MsgClient.java
	javac -cp '.:gnu-getopt-1.0.13.jar' MsgClient.java

test:
	java -cp '.:gnu-getopt-1.0.13.jar' MsgClient -s 127.0.0.1 -p 5555 -n Pradeep -m "Hello World"

clean: 
	rm MsgClient.class
