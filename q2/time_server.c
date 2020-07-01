#include<stdio.h>
#include<strings.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

int PORT = 9132;
int BUFF_SIZE = 200;
int main(){
    int socketfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFF_SIZE],message[BUFF_SIZE] ;
    time_t ticks; 

    // SOCK CREATION
    socketfd = socket(AF_INET, SOCK_STREAM,0);
    if(socketfd < 0 ){
        printf("SOCKET CREATION FAILED\n");
        exit(EXIT_FAILURE);
    }

    // SERVER INFO
    // memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //BINDING SOCKET
    int err = bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1){
        perror("BINDING FAILED\n");
        exit(EXIT_FAILURE);
    }
    // LISTEN
    err = listen(socketfd,5);
    if(err == -1){
        perror("LISTENING FAILED\n");
        exit(EXIT_FAILURE);
    }
    while(1){
    // ACCEPT
    int client_len = sizeof(client_addr);
    int client_socket = accept(socketfd,(struct sockaddr *)&client_addr,&client_len);
    if(err == -1){
        perror("ACCEPT FAILED\n");
        exit(EXIT_FAILURE);
    }
    // CHAT
    char msg[BUFF_SIZE];
    char snd[BUFF_SIZE];
    int len = sizeof(client_addr);
    
        ticks = time(NULL);
        snprintf(snd, sizeof(snd), "%.24s\r\n", ctime(&ticks));
        int snd_len = send(client_socket,&snd, sizeof(snd),0);
        if(snd_len == -1){
            perror("MESSAGE NOT SEND TO CLIENT 1\n");
            exit(EXIT_FAILURE);
        }   
    close(client_socket);
    sleep(1);    
    }
	exit(0);
    return 0;
}
