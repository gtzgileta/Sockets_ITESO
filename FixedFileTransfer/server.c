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
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFERSIZE 1048576  //Tamaño del buffer

/*
*	VARIABLES
*/

u_int port;
int server;
int client;
int status;
struct sockaddr_in myAddr;
struct sockaddr clientAddr;
socklen_t clientLen;

char Archivo [] = "Prueba.txt"; //Nombre del archivo a transferir
int file;
char *buffer;

int readBytes, totalReadBytes;
int writeBytes, totalWriteBytes;

/*
*	FUNCIÓN MAIN
*/

int main(int args, char *argv[]) {
	
	//Abrimos el archivo
	file = open(Archivo, O_RDONLY);
	if(file == -1)
	{
		printf("Error: Can't open the file\n");
		return 1;
	}
	
	//Validamos los argumentos
	if(args < 2) {
		fprintf(stderr, "Error: Missing Arguments\n");
		fprintf(stderr, "\tUse: %s [PORT]\n", argv[0]);
		return	1;
	}
	
	port = atoi(argv[1]);
	if(port < 1 || port > 65535) {
		fprintf(stderr, "Port %i out of range (1-65535)\n",port);
		return	1;
	}
	
	//Abrir el socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if(server == -1)
	{
		printf("Error: The socket can't be opened\n");
		return 1;
	}
	
	//Avisamos al sistema que utilizaremos el socket
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(port);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	status = bind(server, (struct sockaddr*)&myAddr, sizeof(myAddr));
	if(status != 0)
	{
		printf("Error: Can't bind port\n");
		return 1;
	}
	
	//Nos ponemos en modo de escucha
	status = listen(server, 5);
	if(status == -1)
	{
		printf("Error: Can't listen on socket\n");
		close(server);
		return 1;
	}
	
	//Nos mantenemos en modo de "escucha" y aceptamos conexiones
	while(1) {

		printf("\n\n\t[Esperando solicitud del cliente...]\n\n");

		client = accept(server, &clientAddr, &clientLen);

		if(client == -1) {
			printf("Error: Can't acept connection\n");
			close(server);
			return 1;
		}
		
		//Comenzamos el envio
		buffer = (char *) calloc(1, BUFFERSIZE);
		totalReadBytes = 0;
		while((readBytes = read(file, buffer, BUFFERSIZE))>0) {
			totalWriteBytes = 0;
			while(totalWriteBytes < readBytes) {
				writeBytes = write(client, buffer+totalWriteBytes, readBytes-totalWriteBytes);
				totalWriteBytes += writeBytes;
			}
			
			totalReadBytes += readBytes;
		}
		
		printf("> Archivo Enviado");	
		
		//Liberamos el buffer
		free (buffer);

		//Cerrando
		close(file);
		close(client);		
				
	}
		
}
