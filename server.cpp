#include <iostream>
#include <cstring>
#include <cstdlib>
#include<stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include<string>

void error(const char *msg){
    perror(msg);
    exit(1);
}


int main(int argc,char *argv[]){


    if(argc < 2){
        fprintf(stderr,"Usage: %s <server port numebr required>",argv[0]);
        exit(1);
    }

    int sockfd, newsockfd , portNumber, n;
    // sockfd,newsockfd : this tells the file discriptor value they store the value returned by the system calls 
    // portnumber : the port nummber through which the server accepts connectins
    // clilen : length of the client address
    // n : the value of read and write calls


    
    char buffer[255];


    struct sockaddr_in server_add;
    struct sockaddr_in client_add;
    socklen_t clientLength;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
   
    if(sockfd < 0 ){
        error("Failed to connect to a socket");
    }

    memset(&server_add, 0, sizeof(server_add));

    portNumber = atoi(argv[1]);

    server_add.sin_family = AF_INET;
    server_add.sin_addr.s_addr = INADDR_ANY;
    server_add.sin_port = htons(portNumber);

    if( bind(sockfd, (struct sockaddr *)&server_add, sizeof(server_add)) < 0 ){
        error("failed to bind");
    }

    listen(sockfd,5);


    clientLength = sizeof(client_add);
    newsockfd = accept(sockfd, (struct sockaddr *) &client_add, &clientLength);
    if (newsockfd < 0){
        error("ERROR on accept");
    }

    while(1){
        bzero(buffer,255);
        n = read(newsockfd,buffer,255);
        printf("the messge sent by client is %s" , buffer);
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(newsockfd,buffer,strlen(buffer));
        if(n<0){
            error("unable to send the packet");
        }

        int i = strncmp("bye",buffer,3);

        if(i==0){
            error("unable to semd the package");
        }
    }


    close(newsockfd);
    close(sockfd);



    return  0;
}
