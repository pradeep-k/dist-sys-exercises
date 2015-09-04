// CS 6421 - Simple Message Board Client in C
// T. Wood
// Compile with: gcc msg_client -o msg_client
// Run with:     ./msg_client

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

static void usage()
{
        printf("./client -h for help\n");
        printf("./client -s <server ip/name> -p <port> -n <name> -m <msg>\n");
}

int main(int argc, char ** argv)
{
        /*
         * Default values. Arguments will override this
         */
        char* server_port   = "5555";
        char* server_ip     = "upkk5920aab9.pradeep-k.koding.io";
        char* message       = "Hello";
        char* name          = "Pradeep";

        // your code here!
        int sockfd, rc;
        struct addrinfo hints, *server;
        int o;
        
        while ((o = getopt (argc, argv, "p:h:n:m:s:")) != -1) {
                switch(o){
                case 'p':
                        server_port = optarg;
                        break;
                case 's':
                        server_ip = optarg;
                        break;
                case 'm':
                        message = optarg;
                        break;
                case 'n':
                        name = optarg;
                        break;
                case 'h':
                        usage();
                        return 0;
                case '?':
                        if (optopt == 'h') { 
                                usage();
                                return 0;
                        } else if(optopt == 'p' || optopt == 's' || optopt == 'm' || optopt == 'n' ) {
                                fprintf (stderr, "Option %c requires an argument.\n", optopt);
                         
                        } else {
                                fprintf (stderr, "Unknown argument: %c.\n", optopt);
                        }
                        return -1;
                }
        }
        
      
        /* 
         * We will be concatenating the name and message
         */
        char* msg =  calloc(sizeof(char), strlen(name) + strlen(message) + 1);
        char* ori_msg =  msg;
        snprintf(msg, strlen(name) + 2,"%s\n", name); 
        msg += strlen(name) + 1;
        snprintf(msg, strlen(message) + 1, "%s", message);
        
        printf("server_ip: %s port: %s\n", server_ip, server_port);
        printf("name: %s\n", name);
        printf("message: %s\n", message);

        /* The hints struct is used to specify what kind of server info we are looking for */
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM; /* or SOCK_DGRAM */
        
        
        /* getaddrinfo() gives us back a server address we can connect to.
           It actually gives us a linked list of addresses, but we'll just use the first.
         */
        if ((rc = getaddrinfo(server_ip, server_port, &hints, &server)) != 0) {
                perror(gai_strerror(rc));
                exit(-1);
        }
        
        /* Now we can create the socket and connect */
        sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
        if (sockfd == -1) {
                perror("ERROR opening socket");
                exit(-1);
        }
        rc = connect(sockfd, server->ai_addr, server->ai_addrlen);
        if (rc == -1) {
                perror("ERROR on connect");
                close(sockfd);
                exit(-1);
        }

        /* Send the message, plus the \0 string ending. Use 0 flags. */
        rc = send(sockfd, ori_msg, strlen(ori_msg)+1, 0);
        if(rc < 0) {
                perror("ERROR on send");
                exit(-1);
        }

        out:
        freeaddrinfo(server);
        close(sockfd);

        printf("Done.\n");
        return 0;
}
