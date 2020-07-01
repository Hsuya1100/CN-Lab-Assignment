#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<errno.h> 
#include<netdb.h>  
#include<time.h>   
#define PORT 9002
#define MAXLINE 1024 
#define BUFF_SIZE 100 

int hostname_to_ip(char*, char*);
int max(int x, int y) ;

int main() 
{ 
	int  clientfd, sel, maxfdp1;
	pid_t childpid; 
	fd_set fdset; 
	ssize_t n; 
	socklen_t len; 
	const int on = 1; 
	void sig_chld(int); 
	// ==============================TCP=======================================
	int tcpfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFF_SIZE],message[BUFF_SIZE] ;
    time_t ticks; 

    // SOCK CREATION
    tcpfd = socket(AF_INET, SOCK_STREAM,0);
    if(tcpfd < 0 ){
        printf("SOCKET CREATION FAILED\n");
        exit(EXIT_FAILURE);
    }

    // SERVER INFO
    // memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //BINDING SOCKET
    int err = bind(tcpfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1){
        perror("BINDING FAILED\n");
        exit(EXIT_FAILURE);
    }
    // LISTEN
    err = listen(tcpfd,5);
    if(err == -1){
        perror("LISTENING FAILED\n");
        exit(EXIT_FAILURE);
    }
	// ==============================UDP=======================================
	int udpfd = socket(AF_INET, SOCK_DGRAM,0);
    err = bind(udpfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(err == -1){
        perror("BINDING 2 FAILED\n");
        exit(EXIT_FAILURE);
    }
    
	// ==============================END=======================================
	FD_ZERO(&fdset); 
	maxfdp1 = max(tcpfd, udpfd) + 1; 
	while(1) { 
		FD_SET(tcpfd, &fdset); 
		FD_SET(udpfd, &fdset); 
		sel = select(maxfdp1, &fdset, NULL, NULL, NULL); 
		if (FD_ISSET(tcpfd, &fdset)) { 
			len = sizeof(client_addr); 
			clientfd = accept(tcpfd, (struct sockaddr*)&client_addr, &len); 
			if ((childpid = fork()) == 0) { 
				close(tcpfd); 
				bzero(buffer, sizeof(buffer)); 
				printf("TCP request for time\n"); 
				char snd[40];
				ticks = time(NULL);
		        snprintf(snd, sizeof(snd), "%.24s\r\n", ctime(&ticks));
				write(clientfd, snd, sizeof(snd)); 
				printf("TCP request completed\n"); 
				close(clientfd); 
				exit(0); 
			} 
			close(clientfd); 
		} 
		if (FD_ISSET(udpfd, &fdset)) { 
			len = sizeof(client_addr); 
			bzero(buffer, sizeof(buffer)); 
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0, 
						(struct sockaddr*)&client_addr, &len); 
			printf("\nUDP request for: %s\n",buffer); 
			// puts(buffer); 
			char *hostname = buffer;
	        char ip[100];
	        int signa = hostname_to_ip(hostname , ip);
			sendto(udpfd, ip, sizeof(ip), 0,(struct sockaddr*)&client_addr, sizeof(client_addr)); 
				printf("UDP request completed\n"); 
		} 
	} 
} 
int max(int x, int y){ 
	if (x > y) 
		return x; 
	else
		return y; 
} 
int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ( (he = gethostbyname( hostname ) ) == NULL) {
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **)he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
    return 1;
}