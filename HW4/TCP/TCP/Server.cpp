/*
Author: Hussain Miyaziwala
Class: ECE4122
Last Date Modified: 10/24/19
Description:
TCP Server Code (HW4)

NOTE: DO NOT CONTROL+C TO EXIT THE PROGRAM.
THIS WILL MESS UP AND POLLLUTE PORTS.
CLOSE CLIENTS BEFORE CLOSING THE SERVER, there is no heartbeat used.

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
#include <thread>
#include <iostream>
#include <iterator>
#include <map>


using namespace std;

// struct def
struct tcpMessage
{
 unsigned char nVersion;
 unsigned char nType;
 unsigned short nMsgLen;
 char chMsg[1000];
};

// method stubs
void doStuff(int);
void console();
void printDetails();


// GLOBALS
int clientCount = 0;
char mostRecent[1000];
map<int, string> clientList;
bool run = true;

int main(int argc, char *argv[]){

  if (argc < 2) {
     fprintf(stderr,"ERROR, no port provided\n");
     exit(1);
  }

  int portno = atoi(argv[1]);

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

// Create Socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
//	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverAddr.sin_addr.s_addr = INADDR_ANY;

// Bind
	ret = ::bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	//printf("[+]Bind to port %d\n", portno);

	if(listen(sockfd, 10) == 0){
	//	printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}

  thread console_t(console);

	thread mythread[50];
// Connect to client
	while(run){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			printf("Bad connection!\n");
			continue;
		}
    char detail[100];
		sprintf(detail,"%s\t%d", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

    // create map of clients  based on the socket
    clientList.insert(pair<int,string>(newSocket, detail));

		mythread[clientCount] = thread(doStuff, newSocket);
		clientCount++;
    if (clientCount >=50 ){
      printf("Too many clients.");
      break;
    }

	}
	return 0;
}

// print details of clients by traversing client map
void printDetails(){
  printf("Number of clients: %d\n", int(clientList.size()));
  cout << "IP\t\tPort" << endl;
  for (auto mapPair=clientList.begin(); mapPair!=clientList.end(); ++mapPair ) {
        cout  << (*mapPair).second << "\n";
    }
}

// thread to listen to user input into the console
void console(){
  char command;;
  while(run){
    printf("Command: ");
    cin >> command;
    if (command == '0'){
      if (mostRecent[0] != 0){
        printf("Last message recieved: %s\n", mostRecent);
      }
    } else if (command == '1'){
      printDetails();
    } else if (command == 'q'){
      for (auto mapPair=clientList.begin(); mapPair!=clientList.end(); ++mapPair ) {
          close((*mapPair).first);
      }
      exit(1);
    }
}
}

// thread for each client.
// analyzes each packet and acts accordingly
void doStuff(int newSocket){
	struct tcpMessage tm;

	while(run){
		recv(newSocket, &tm, sizeof(tcpMessage), 0); // wait until packet recieved
    if (tm.nVersion != '1'){ //ignore msg if not 1
      continue;
    }

    if (tm.nType == 'X'){
      clientList.erase(newSocket);
      //printf("Disconnected from %d\n",newSocket);
			return;
    } else if (tm.nType == '0') {
      memcpy(mostRecent, tm.chMsg, sizeof(mostRecent));
      // send to everyone except newSocket
      for (auto mapPair=clientList.begin(); mapPair!=clientList.end(); ++mapPair ) {
        if ((*mapPair).first != newSocket){
          send((*mapPair).first, &tm, sizeof(tcpMessage), 0);
        }
      }

    } else if (tm.nType == '1'){ //echo back
      memcpy(mostRecent, tm.chMsg, sizeof(mostRecent));
  		send(newSocket, &tm, sizeof(tcpMessage), 0);
    }
		}
	}
