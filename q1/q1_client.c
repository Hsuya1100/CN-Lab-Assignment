
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 9002 
#define MAXLINE 1024 
#define MAXLEN 4096

int main() 
{ 
	int sockfd; 
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr, cli_addr; 
	socklen_t clilen = sizeof(cli_addr);
	int n, len;  
	fd_set nset;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 
	memset(&servaddr, 0, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    // PING
    char pin[222];
    int x = sendto(sockfd, pin, sizeof(pin), 0, (struct sockaddr*) &servaddr, sizeof(servaddr));
	if(x < 0) {
		perror("ERROR in PING");
		close(sockfd);
		exit(1);
	}	
	while (1){
		FD_ZERO(&nset);
		FD_SET(0,&nset);
		FD_SET(sockfd,&nset);
		int ret=select(10,&nset,0,0,0);
		if (ret==-1)
		{
			printf("SELECT Error\n");
			close(sockfd);
			return 0;
		}
		else if (FD_ISSET(0,&nset))
		{			
			char mess[256];
			fgets(mess,sizeof(mess),stdin);
			int nbytes = sendto(sockfd, mess, sizeof(mess), 0, (struct sockaddr*) &servaddr, sizeof(servaddr));
			if(nbytes < 0) {
				perror("ERROR in sendto");
				close(sockfd);
				exit(1);
			}	
			if (strncmp(mess,"bye",3)==0)
			{
				close(sockfd);
				return 0;
			}
		}
		else if (FD_ISSET(sockfd,&nset))
		{
			char buf[MAXLEN];
			int nbytes = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&cli_addr, &clilen);
			if (nbytes < 0){
				perror("ERROR in recvfrom()");
				close(sockfd);
				exit(1);
			}
			printf("Server : %s\n",buf);
			if (strncmp(buf,"bye",3)==0)
			{
				close(sockfd);
				return 0;
			}
		}
	}
	close(sockfd); 
	return 0; 
} 
