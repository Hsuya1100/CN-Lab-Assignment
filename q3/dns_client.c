#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>

int PORT = 9002;
int BUFF_SIZE = 50;

int main(){
    int socketfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFF_SIZE] ;

    // SOCK CREATION
    socketfd = socket(AF_INET, SOCK_DGRAM,0);
    if(socketfd < 0 ){
        printf("SOCKET CREATION FAILED\n");
        exit(EXIT_FAILURE);
    }
    // SERVER INFO
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    int BUFF_SIZE = 256;
    char message[256];
    fgets(message, sizeof(message), stdin);
    BUFF_SIZE = strlen(message);
    //PING THE SERVER
    int err = sendto(socketfd, message, BUFF_SIZE-1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1){
        perror("CLIENT MESSAGE SEND FAILED \n");
        exit(EXIT_FAILURE);
    }

    int len = sizeof(client_addr) ;
    err = recvfrom(socketfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &len);
    if(err == -1){
        perror("MESSAGE NOT RECIEVED\n");
        exit(EXIT_FAILURE);
    }
    printf("HOST: %s\n",message);
    printf("IP  : %s\n", buffer);
    close(socketfd);
    return 0;
}
    















