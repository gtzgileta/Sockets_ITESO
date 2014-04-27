/*
*
*	En la terminal: ./client
*
*	Autor: Oscar Daniel Gutiérrez Gileta
*
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>  

/*
*	VARIABLES GLOBALES
*/

	int udpSocket;
	struct sockaddr_in udpServer, udpClient;
	socklen_t addrlen = sizeof(udpClient);
	int Status;
	char buffer[255];
	char ip[17];
	u_short ClientPort;

	char *SendString = "Daniel PC";
	unsigned int SendStringLen;

/*
*	CREAR SOCKET
*/

int createSocket() {
	udpSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(udpSocket == -1) {
			fprintf(stderr,"Can´t created UDP Socket");
			return 1;
		}            

	udpServer.sin_family = AF_INET;
	inet_pton(AF_INET,"0.0.0.0",&udpServer.sin_addr.s_addr);
	udpServer.sin_port = htons(5000);

	Status = bind(udpSocket,(struct sockaddr*)&udpServer,sizeof(udpServer));
		if(Status != 0) {
				fprintf(stderr,"Can´t bind"); 
			}

	bzero(buffer, 255);
	SendStringLen = strlen(SendString); 
	Status = recvfrom(udpSocket,buffer,255,0,(struct sockaddr*)&udpClient,&addrlen);
	udpServer.sin_family = AF_INET;
	inet_ntop(AF_INET,&(udpClient.sin_addr),ip,INET_ADDRSTRLEN);
	ClientPort = ntohs(udpClient.sin_port);

	return 0;
}

/*
*	FUNCIÓN MAIN
*/

int main (int argc, char const *argv[]){

	createSocket();

	printf("Received from [%s:%i]:%s\n",ip,ClientPort,buffer);

	Status = sendto(udpSocket,SendString,SendStringLen,0,(struct sockaddr*)&udpClient,sizeof(udpClient));
	fflush(stdout);
	close (udpSocket);
	return 0;
}
