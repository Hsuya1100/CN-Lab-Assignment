#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/time.h>
#define MAXLEN 4096
#define PORT 9002
int main()
{
	fd_set read_fds, write_fds;				
	int sockfd;								
	struct sockaddr_in srv, cli_addr,client_addr;
	socklen_t clilen = sizeof(cli_addr);
	int nbytes;	
	fd_set nset;
	 
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
		perror("ERROR opening socket");
		exit(1);
	}
	else
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = htonl(INADDR_ANY);
     
	if(bind(sockfd, (struct sockaddr*) &srv, sizeof(srv)) < 0){
		perror("ERROR on binding");
		close(sockfd);
		exit(1);
	}
	else{
		 int le = sizeof(client_addr);
		 char pin[222];
	    int ping = recvfrom(sockfd, pin, sizeof(pin),0,(struct sockaddr*)&client_addr, &le);
	    if(ping == -1){
	        perror("CLIENT  PING FAILED\n");
	        exit(EXIT_FAILURE);
	        close(sockfd);
	        exit(0);
	    }
		while(1){ 
			char* buf     = (char*)malloc(MAXLEN);
			memset(buf, 0, strlen(buf));
			bzero(buf, MAXLEN);
			FD_ZERO(&nset);
			FD_SET(0, &nset);
			FD_SET(sockfd, &nset);
			int n = select(10, &nset, 0, 0, 0);
			if(n < 0){
				perror("ERROR Server : select()\n");
				close(sockfd);
				exit(1);
			}
			if(FD_ISSET(sockfd, &nset)){
				nbytes = recvfrom(sockfd, buf, MAXLEN, 0, (struct sockaddr*)&client_addr, &le);
				if (nbytes < 0){
					perror("ERROR in recvfrom()");
					close(sockfd);
					exit(1);
				}
				printf("Client : %s\n",buf);
			}
			if(FD_ISSET(0, &nset)){
				char mess[256];
				fgets(mess,sizeof(mess),stdin);
				nbytes = sendto(sockfd, mess, sizeof(mess), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
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
			memset(&buf, 0, sizeof(buf));	
		}
	}
	close(sockfd);
	return 0;
}
