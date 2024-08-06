#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include<netdb.h>



void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc,char* argv[]){

    if(argc < 3){
        error("Either port no or server address not provided");
    }

    int sockfd,portNumber,n;

    char buffer[255];

    struct sockaddr_in server_add;
    struct hostent *server;

    portNumber = atoi(argv[2]);

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd<0){
        error("Failed to connect to socket on client side");
    }

    server = gethostbyname(argv[1]);

    if(server==NULL){
        fprintf(stderr,"No server found");
    }

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(portNumber);
    bcopy((char *)server->h_addr_list[0],(char *) &server_add.sin_addr.s_addr, server->h_length);

    if(connect(sockfd,(struct sockaddr*) &server_add,sizeof(server_add)) < 0){
        error("failed to connect ");
    }

    while(1){
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if(n < 0){
            error("failed to send message");
        }
        n = read(sockfd,buffer,strlen(buffer));
        if( n < 0){
            error("failed to recieve a message");
        }
        printf("Server said : %s",buffer);

        int i = strncmp("Bye",buffer,3);

        if(i==0){
            break;
        }

    }

    close(sockfd);
    return 0;


}
