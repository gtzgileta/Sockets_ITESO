/*
*
*	En la terminal: ./client <ADDR> <PUERTO>
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

#define BUFFERSIZE 1048576

/*
*	VARIABLES GLOBALES
*/

	u_int port;
	int server;
	int client;
	int localerror;
	struct sockaddr_in server_addr;
	socklen_t clienteLen;	
	int status;
	
	int readBytes = 0;
	int totalReadBytes = 0;
	int totalWriteBytes = 0;
	int writeBytes = 0;
	char *readBuffer = NULL;
	int file;

/*
*	FUNCIÓN MAIN
*/

int main(int args, char *argv[]) {

	//Validamos los Arguemntos
	if(args < 3) {
		fprintf(stderr,"Error: Missing Arguments\n");
		fprintf(stderr,"\tUSE: %s [ADDR] [PORT]\n",argv[0]);
		return 1;
	}
	
	port = atoi(argv[2]);
	if(port < 1 || port > 65535) {
		fprintf(stderr, "Port %i out of range (1-65535)\n",port);
		return	1;
	}
	
	//Abrimos el socket
	server = socket(AF_INET, SOCK_STREAM, 0);
	if(server == -1)
	{
		printf("Error: Socket can't be opened\n");
		return 1;
	}
	
	//Abrimos el archivo
	if((file = open("Recibido.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) == -1) {
		localerror = errno;
		fprintf(stderr,"Can't open file for write (%s)\n",strerror(localerror));
		return 1;
	}
	
	//Nos preparamos para la conexion
	server_addr.sin_family = AF_INET;
	status = inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr);
	server_addr.sin_port = htons(port);
	
	status = connect(server, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(status != 0)
	{
		printf("Error: Can't connect\n");
		return 1;
	}
	
	//Empezamos a recibir	
		readBuffer = (char *) calloc(1,BUFFERSIZE);

		while((readBytes = read(server,readBuffer,BUFFERSIZE)) > 0) {
			totalWriteBytes = 0;
			printf("> Leyendo Archivo...\n");
			while(totalWriteBytes < readBytes) {
				writeBytes = write(file,readBuffer+totalWriteBytes,readBytes-totalWriteBytes);
				totalWriteBytes += writeBytes;
			}
			totalReadBytes += readBytes;
			printf("> Archivo Recibido\n\n");
		}

		printf("> Ahora puedes confirmar haber recibido el archivo de 'Recibido.txt'\n\n");
		
		//Cerramos
		free(readBuffer);
		close(file);
		close(client);		
}
