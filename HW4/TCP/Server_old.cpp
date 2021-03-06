#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>

#define PORT 4444

using namespace std;

struct tcpMessage
{
 unsigned char nVersion;
 unsigned char nType;
 unsigned short nMsgLen;
 char chMsg[1000];
};


void doStuff(int newSocket);

int main(){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}

	thread mythread[10];
	int i = 0;

	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			//printf("bad conn");
			continue;
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		mythread[i] = thread(doStuff, newSocket);
		i++;

		}

	close(newSocket);
	return 0;
}

void doStuff(int newSocket){
	char buffer[1024];
	while(1){
		recv(newSocket, buffer, 1024, 0);
		if(strcmp(buffer, ":exit") == 0){
			//printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			printf("Disconnected from %d\n",1);
			return;
		}else{
			printf("Client: %s\n", buffer);
			send(newSocket, buffer, strlen(buffer), 0);
			bzero(buffer, sizeof(buffer));
		}
	}
}
