
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



static char* port;
static char* server_ip;

//char proxy_server_ip[100];
//char proxy_server_port[100];
char* proxy_server_ip;
char* proxy_server_port;

char* print_server_ip;
char* print_server_port;


/*
 * Print a usage message
 */
static void
usage() {
    printf("Usage:  ./server1 -p <my ip> -s <proxy-sever-ip> -r <proxy-server-port>\n");
}

/*
 *If connection is established then send out welcome message
 */

//--TODO: add your converting functions here
// converting function
double converting_function(char* first_token, double input_num){
    double output_kg_g = 0;
    if (!strcmp(first_token,"kg")) {   // kg->g
    	output_kg_g = input_num*1000;
    	printf("kg is converted to g, results is %f g\n", output_kg_g);
    } else {						// g->kg
    	output_kg_g = input_num/1000;
    	printf("g is converted to kg, results is %f kg\n", output_kg_g);
    }
    return output_kg_g;

}
// function for sending msg to proxy server
void send_msg_proxy(char* x) {
        struct addrinfo hints, *server;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        int rc = 0;

        if ((rc = getaddrinfo(proxy_server_ip, proxy_server_port, &hints, &server)) != 0) {
                perror(gai_strerror(rc));
                exit(-1);
        }

        int sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
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
	
        send(sockfd, x, strlen(x), 0);
	close(sockfd);
}

void send_msg_print(char* x) {
        struct addrinfo hints, *server;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        //hints.ai_socktype = SOCK_DGRAM;

        int rc = 0;

        if ((rc = getaddrinfo(print_server_ip, print_server_port, &hints, &server)) != 0) {
                perror(gai_strerror(rc));
                //exit(-1);
        }
        printf("debug: I am here\n");
        int sockfd = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
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
        printf ("registered server1\n");	
        send(sockfd, x, strlen(x), 0);
	    close(sockfd);
}

// processing function
void
process(int sock)
{
    int n;
    int BUFSIZE = 1024;
    char buf[BUFSIZE];
    char* msg = "Welcome, you are connected to a C-based server\n";
    char* userInput;

    /* Write a welcome message to the client */
    n = write(sock, msg, strlen(msg));

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
    close(sock);
    printf("Received message: %s\n", userInput);

    //--TODO: add your converting functions here, msg = func(userInput);

    const char s[2] = " ";				//space as delimiter
    char *flag= " "; 
    char *(token[4]);
    /* get the first token */
    token[0] = strtok(userInput, s);
    /* walk through other tokens */
    int i = 0;
    while( token[i] != NULL ) {
      i++;
      token[i] = strtok(NULL, s);
    }
    // n means, this is the first time 
    // f means, this is the second time
    if (!strcmp(token[3],"1")) {
	    flag = "n";
    } else {
	    flag = "f";
    }
    
    double input_num = atof(token[2]);
    // send msg back to proxy server
    double output_kg_g_ = 0;
    char output_kg_g_c[BUFSIZE];
    bzero(output_kg_g_c,BUFSIZE);
    output_kg_g_ = converting_function(token[0], input_num);
    sprintf(output_kg_g_c,"%lf",output_kg_g_);
    strcat(token[2]," ");
    strcat(token[2],token[0]);
    strcat(token[2]," ");
    strcat(token[2],output_kg_g_c);
    strcat(token[2]," ");
    strcat(token[2],token[1]);
    strcat(token[2]," ");
    strcat(token[2],flag);
    
    msg = token[2];
    printf("%s\n",msg);
    send_msg_proxy(msg);
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
    serv_addr.sin_port = htons(atoi(port));
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

char* receiveInput( char *s ){
    scanf("%99s",s);
    return s;
}

int main(int argc, char ** argv){

    const char* progname = argv[0];
    //char* server_port   = "5555";
    //char* server_ip     = "upkk5920aab9.pradeep-k.koding.io";
        
    int o;
        
    while ((o = getopt (argc, argv, "hp:s:r:a:b:")) != -1) {
            switch(o){
            case 'b':
                    print_server_port = optarg;
                    break;
            case 'a':
                    print_server_ip = optarg;
                    break;
            case 'r':
                    proxy_server_port = optarg;
                    break;
            case 's':
                    proxy_server_ip = optarg;
                    break;
            case 'h':
                    usage();
                    return 0;
            case 'p':
                    port = optarg;
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
     
    printf("server_ip: %s port: %s\n", proxy_server_ip, proxy_server_port);
    printf("My port: %s\n", port);
	

    //send msg to print server
    char print_msg[1024];
    sprintf(print_msg, "Conversion server is up\n");
	send_msg_print(print_msg);

    //Do the usual job of conversion
    if (server() != 0){
        printf("server in trouble\n");
    }
}
