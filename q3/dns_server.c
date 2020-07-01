#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h>   //hostent
#include<time.h>   //hostent

int PORT = 9002;
int BUFF_SIZE = 59;

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
        
    if ( (he = gethostbyname( hostname ) ) == NULL) 
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **)he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++) 
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
    
    return 1;
}


int main(){
    int socketfd;
    struct sockaddr_in server_addr;
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

    //BINDING SOCKET
    int err = bind(socketfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1){
        perror("BINDING FAILED\n");
        exit(EXIT_FAILURE);
    }
    while(1){
        struct sockaddr_in client_addr;
        int len = sizeof(client_addr);
        // printf("size of buffer : %ld\n",sizeof(buffer));
        int recv_len = recvfrom(socketfd,(char *) buffer, sizeof(buffer),0,(struct sockaddr*)&client_addr, &len);
        if(recv_len == -1){
            perror("MESSAGE NOT RECIEVED FROM CLIENT\n");
            exit(EXIT_FAILURE);
        }
        buffer[recv_len]='\0';
        printf("MESSAGE RECIEVED FROM CLIENT :\n%s\n",buffer);
        char *hostname = buffer;
        char ip[100];
        int message = hostname_to_ip(hostname , ip);
        // printf("sending response : %s \n",ip);
        err = sendto(socketfd, ip, sizeof(ip), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        if(err == -1){
            perror("MESSAGE NOT SEND TO CLIENT \n");
            exit(EXIT_FAILURE);
        }
        printf("MESSAGE SENT TO CLIENT  \n");
        // printf("CLOSING SERVER ...\n");
        // close(socketfd);
        sleep(1);
    }
    return 0;
}


