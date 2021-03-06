/*
*
*	En la terminal: ./client <PUERTO>
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

/*
*	VARIABLES GLOBALES
*/
	u_int port;
	int status;
	int server;
	int client;
	int localerror;
	struct sockaddr_in server_addr;
	socklen_t clienteLen;

/*
*	FUNCIÓN MAIN
*/

int main(int args, char *argv[]) {

	//Variables locales
	char cadena[50];
	char hora[] = "Hora";

	//Variables para tiempo usando librería "time.h"
    	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);

	//Creamos el Socket para Conexión
	createSocket(args, argv);

	printf("\n[Conexion exitosa]");
 	
	//Mandamos solicitud al servidor
	status = write(server,hora,strlen(hora));
	printf("\n> Solicitamos la fecha y hora del servidor");

	//Recibimos respuesta del servidor
    	status = read(server,cadena,50);
	printf("\n> Fecha y Hora del servidor: %s\n\n",cadena);

	//Cerramos conexión
	close(server);
}

/*
*	CREAR SOCKET
*/

int createSocket(int args, char *argv[]) {

	//Validamos los Arguemntos
	if(args < 2) {
		fprintf(stderr,"Error: Missing Arguments\n");
		fprintf(stderr,"\tUSE: %s [PORT]\n",argv[0]);
		return 1;
	}

	server = socket(PF_INET,SOCK_STREAM,0);
	if(server == -1) {
		localerror = errno;
		fprintf(stderr, "Error: %s\n",strerror(localerror));
		return 1;
	}

	port = atoi(argv[1]);

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;		
	status = inet_pton(AF_INET,argv[1],&server_addr.sin_addr.s_addr);
	server_addr.sin_port = htons(port);

	status = connect(server,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(status != 0) {
		localerror = errno;
		printf("\nError al conectarnos (%s)\n",strerror(localerror));
		return 1;
	}
}
