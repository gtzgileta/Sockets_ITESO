/*
*
*	En la terminal: ./server <PUERTO>
*
*	Autor: Oscar Daniel Gutiérrez Gileta
*
*/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <time.h>

int main(int args, char *argv[]) {

	u_int port;
	int server;
	int client;
	int localerror;
	struct sockaddr_in myAddr;
	struct sockaddr_in cliente_addr;
	socklen_t clienteLen;	
	int status;

	char buffer[80];
	char cadena[10];
    	time_t t;
	time(&t);

    //Validamos los Arguemntos
	if(args < 2) {
		fprintf(stderr,"Error: Missing Arguments\n");
		fprintf(stderr,"\tUSE: %s [PORT]\n",argv[0]);
		return 1;
	}

	port = atoi(argv[1]);
	if(port < 1 || port > 65535) {
		fprintf(stderr,"Port %i out of range (1-65535)\n",port);
		return 1;
	}

	//Iniciamos la apertura del Socket
	server = socket(PF_INET,SOCK_STREAM,0);
	if(server == -1) {
		localerror = errno;
		fprintf(stderr, "Error: %s\n",strerror(localerror));
		return 1;
	}

	//Nos adjudicamos el Puerto.
	bzero(&myAddr,sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(port);

	status = bind(server, (struct sockaddr *)&myAddr, sizeof(myAddr));
	if(status != 0) {
		localerror = errno;
		fprintf(stderr,"Can't Bind Port: %s\n",strerror(localerror));
		close(server);
		return 1;
	}

	//Nos ponemos el modo de Escucha
	status = listen(server,5);
	if(status == -1) {
		localerror = errno;
		fprintf(stderr,"Can't listen on socket(%s)\n",strerror(localerror));
		close(server);
		return 1;
	}

	//Nos mantenemos en modo de "escucha" y aceptamos conexiones
	while(1) {

		printf("\n\n\t[Esperando solicitud del cliente...]\n\n");

		bzero(&cliente_addr,sizeof(cliente_addr));
		client = accept(server,(struct sockaddr *)&cliente_addr,&clienteLen);		
		
		status=read(client,cadena,4);

		printf("\n> Client request: '%s'", cadena);
		sprintf(buffer, "%s",ctime(&t));

		if(strcmp(cadena,"Hora")==0) {
			printf("\n> El cliente solicitó la fecha y hora");
			status = write(client, buffer, strlen(buffer));	
			printf("\n> Se le contestó: '%s'", buffer);
		}	
		close(client);
		
	}

	

	return 0;
}
