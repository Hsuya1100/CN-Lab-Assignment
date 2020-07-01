#include<stdio.h>
#include<strings.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

int PORT = 9002;
int BUFF_SIZE = 200;

int main(){
    int socketfd;
    struct sockaddr_in server_addr, server_addrclient_addr;
    char buffer[BUFF_SIZE];
    char msg[BUFF_SIZE] ;


    // SOCK CREATION
    socketfd = socket(AF_INET, SOCK_STREAM,0);
    if(socketfd < 0 ){
        printf("SOCKET CREATION FAILED\n");
        exit(EXIT_FAILURE);
    }

    // SERVER INFO
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // CONNECT
    int connect_stts = connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(connect_stts == -1){
        perror("CONNECTION FAILED\n");
        exit(EXIT_FAILURE);
    }
    while(1){
     
   int  rec = recv(socketfd,&msg, sizeof(msg), 0);
        if(rec == -1){
            perror("MESSAGE NOT RECIEVED\n");
            exit(EXIT_FAILURE);
        }
        printf("\nServer returned : %s ",msg);
        close(socketfd);
        exit(0);
    }
    
    
    return 0;
}
