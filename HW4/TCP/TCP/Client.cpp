/*
Author: Hussain Miyaziwala
Class: ECE4122
Last Date Modified: 10/24/19
Description:
TCP Server Code (HW4)

NOTE: DO NOT CONTROL+C TO EXIT THE PROGRAM.
THIS WILL MESS UP AND POLLLUTE PORTS.

Compile with g++ Client.cpp -pthread -std=c++0x
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <thread>

using namespace std;

// packet struct
struct tcpMessage
{
 unsigned char nVersion;
 unsigned char nType;
 unsigned short nMsgLen;
 char chMsg[1000];
};

// method stubs
void listener(int clientSocket);
void writer(int clientSocket);

int main(int argc, char *argv[]){

  if (argc < 3) {
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }

  struct hostent *server;

  int portno = atoi(argv[2]);
  //server = gethostbyname(argv[1]);

	int clientSocket, ret;
	struct sockaddr_in serverAddr;

// create socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	//printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
  //cout << (strcmp(argv[1], "localhost") == 0 ? "127.0.0.1" : argv[1]);
	serverAddr.sin_addr.s_addr = inet_addr((strcmp(argv[1], "localhost") == 0 ? "127.0.0.1" : argv[1]));
  //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //bcopy((char *)server->h_addr,
  //       (char *)&serverAddr.sin_addr.s_addr,
  //       server->h_length);

// connect to server
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	//printf("[+]Connected to Server.\n");

  thread listen_t(listener,clientSocket);
  thread writer_t(writer, clientSocket);
  writer_t.join();
  exit(1);

}

// thread to listen for messages from the server
 void listener(int clientSocket){

   tcpMessage tm;

   while(1){
     if(recv(clientSocket, &tm, sizeof(tcpMessage), 0) < 0){
       printf("[-]Error in receiving data.\n");
     }else{
       if (tm.nMsgLen > 0){
       printf("\nReceived Msg Type: %c; %s\n", tm.nType, tm.chMsg);
     }
   }
   }
 }

// thread to listen for user input, and sends them to the server
void writer(int clientSocket){

  tcpMessage tm;

  tm.nType = '1';
  tm.nVersion = '1';
  tm.nMsgLen = 1000;

  char buffer[1000];

	while(1){
		printf("Command:  ");
    bzero(buffer, sizeof(buffer));
    bzero(tm.chMsg, sizeof(tm.chMsg));
    cin.getline(buffer, 1000);

    //buffer now contains the command. parse and complete:

    char commandType = buffer[0];

    if (commandType == 'v' && buffer[1] == ' ' && buffer[3] == 0){
      tm.nVersion = buffer[2];
    } else if (commandType == 't' && buffer[1] == ' '){
      tm.nType = buffer[2];
      memcpy(tm.chMsg, buffer + 3, sizeof(tm.chMsg));
  		send(clientSocket, &tm, sizeof(tcpMessage), 0);
    } else if (commandType == 'q' &&  buffer[1] == 0 ){
      tm.nType = 'X';
  		send(clientSocket, &tm, sizeof(tcpMessage), 0);
      exit(1);
			printf("[-]Disconnected from server.\n");
			return;
    }
	}
}
