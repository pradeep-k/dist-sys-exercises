
/**************************************************************************
 *
 *  CS 6421 - Simple Conversation
 *      It sends out a welcome message when receive connection from client.
 *  Compilation: gcc -o conv_server conv_server.c to compile
 *  Execution:
 *      server: ./conv_server portnum
 *
 **************************************************************************/
//Maotong Xu
//Pradeep K.
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <sys/socket.h>
#include <math.h>



static char* server_port;
static char* server_ip;


/*
 * Print a usage message
 */
static void
usage() {
    printf("Usage:  ./server -p <my ip> -s <sever-ip> \n");
}

// processing function
void
process(int sock)
{
    int n;
    int BUFSIZE = 1024;
    char buf[BUFSIZE];
    char* userInput;
    
    /* Write a welcome message to the client */
    /*
    char* msg = "Welcome, you are connected to a C-based server\n";
    n = write(sock, msg, strlen(msg));
    */

    if (n < 0){
        perror("ERROR writing to socket");
        exit(1);
    }

    /* read and print the client's request */
    bzero(buf, BUFSIZE);
    n = read(sock, buf, BUFSIZE);
    if (n < 0){
        perror("ERROR reading from socket");
        exit(1);
    }
    userInput = buf;

    //Displaying ip and port may be useful
    printf("Received message: %s\n", userInput);
    
    close(sock);
}
   

/*
 * Server
 */
int
server( void )
{
    int optval = 1;
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    if (sockfd < 0){
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(server_port));
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("ERROR on binding");
        exit(1);
    }
    /* Listening for the client */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    /* Loop forever receiving client connections */
    while(1) {
        /* Accept connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0){
            perror("ERROR on accept");
            exit(1);
        }
        printf("Accepted connection from client\n");
        /* Process a client request on new socket */
        process(newsockfd);
    }

    /*clean up*/
    close(sockfd);

    return 0;
}

int main(int argc, char ** argv)
{

    const char* progname = argv[0];
    //char* server_port   = "5555";
    //char* server_ip     = "upkk5920aab9.pradeep-k.koding.io";
        
    int o;
        
    while ((o = getopt (argc, argv, "hp:s:")) != -1) {
            switch(o){
            case 's':
                    server_ip = optarg;
                    break;
            case 'h':
                    usage();
                    return 0;
            case 'p':
                    server_port = optarg;
                    break;
            case '?':
                    if (optopt == 'h') { 
                            usage();
                            return 0;
                    } else if(optopt == 'p' || optopt == 's') {
                            fprintf (stderr, "Option %c requires an argument.\n", optopt);
                     
                    } else {
                            fprintf (stderr, "Unknown argument: %c.\n", optopt);
                    }
                    return -1;
            }
    }
    if (argc == 1) {
        usage();
        return 0;
    } 
     
    printf("server_ip: %s port: %s\n", server_ip, server_port);
	

    //TODO: add arguments exception handling here
	
    if (server() != 0){
        printf("server in trouble\n");
        exit(1);
    }
}
