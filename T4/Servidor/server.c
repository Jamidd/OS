#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "math.h"
#include <pthread.h>
#define IP "0.0.0.0"
#define PORT 8080

int X = 1;
int Y = 0;
int Z = 0;
int ID_IMPLEMENTACION = 200; // ??? 

struct Cliente
{
	char id[2];
	int socket;
	char id_contrincante[2];
	char nickname[20];
	char nickname_contrincante[20];
	int status; // 0 -> idle, 1 -> waiting, 2 -> playing

	struct Cliente *sgte;

};

struct Cliente *primero = NULL;
struct Cliente *ultimo = NULL;


void agregarContrincante(char id1[2], char id2[2]) {
	struct Cliente *i = primero;
	struct Cliente *i2 = primero;
	while (i != NULL){
		if (i -> id[0] == id1[0] && i -> id[1] == id1[1]){
			while (i2 != NULL){
				if (i2 -> id[0] == id2[0] && i2 -> id[1] == id2[1]){
					strcpy(i -> nickname_contrincante, i2 -> nickname);
					strcpy(i2 -> nickname_contrincante, i -> nickname);
					i -> id_contrincante[0] = i2 -> id[0];
					i -> id_contrincante[1] = i2 -> id[1];
					i2 -> id_contrincante[0] = i -> id[0];
					i2 -> id_contrincante[1] = i -> id[1];
					return;
				}
				i2 = i2 -> sgte;
			}
		}
		i = i -> sgte;
	}
}


void agregarCliente(char id[2], int socket, char nickname[20]){
	struct Cliente *nuevoCliente = (struct Cliente *)malloc(sizeof(struct Cliente));
	nuevoCliente -> sgte = NULL;
	nuevoCliente -> id[0]  = id[0];
	nuevoCliente -> id[1]  = id[1];
	nuevoCliente -> socket = socket;
	strcpy(nuevoCliente -> nickname, nickname);
	nuevoCliente -> id_contrincante[0] = 0;
	nuevoCliente -> id_contrincante[1] = 0;
	nuevoCliente -> status = 0;

	if (primero == NULL) {

		primero = nuevoCliente;
		ultimo = nuevoCliente;

	} else {

		ultimo -> sgte = nuevoCliente;
		ultimo = nuevoCliente;

	}

	ultimo -> sgte = NULL;

}

void eliminarCliente( char id[2] ) {

	struct Cliente *i = primero;
	while(i != NULL) {
		struct Cliente *aux = NULL;

		if (i -> sgte != NULL && strcmp(i -> sgte -> id, id) == 0) {
			if (i -> sgte == ultimo) {
				aux = i -> sgte;
				i -> sgte = NULL;
				ultimo = i;
			} else {
				aux = i -> sgte;
				i -> sgte = i -> sgte -> sgte;
			}

		}

		if (strcmp(i -> id, id) == 0 && i == primero) {
			aux = i;
			primero = i -> sgte;
		}

		i = i -> sgte;
		if (aux != NULL){
			free(aux);

		}
		
	}

}

int oponenteValido( char id[2] ) {
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( i -> id[0] == id[0] && i -> id[1] == id[1] ){
			if ( i -> status == 1 ) {
				return 0;
			} else {
				return 1;
			}
		}

		i = i -> sgte;
	}

	return 1;
}


int buscarSocketPorID( char id[2] ) {
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( i -> id[0] == id[0] && i -> id[1] == id[1] ){
			return i -> socket;
		}

		i = i -> sgte;
	}

	return 0;
}

void cambiarEstadoPorID( char id[2], int stat ) {
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( strcmp(i -> id, id) == 0 ){
			i -> status = stat;
		}
		i = i -> sgte;
	}
}

void cambiarEstadoPorSocket( int socket, int status ) {
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( i -> socket == socket ){
			i -> status = status;
		}

		i = i -> sgte;
	}

}

int buscarSocketContrincantePorID( char id[2] ) {
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( i -> id[0] == id[0] && i -> id[1] == id[1] ){
			return buscarSocketPorID(i -> id_contrincante);
		}

		i = i -> sgte;
	}

	return 0;
}

char* buscarNicknamePorSocket( int socket ) {
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( i -> socket == socket ){
			return i -> nickname;
		}

		i = i -> sgte;
	}

	return NULL;
}

int cantidadClientesConectados() {
	int cantidad = 0;
	struct Cliente *i = primero;

	while(i != NULL) {
		cantidad++;

		i = i -> sgte;
	}

	return cantidad;
}

int cantidadClientesPorStatus( int status ) {
	int cantidad = 0;
	struct Cliente *i = primero;

	while(i != NULL) {
		if ( i -> status == status ) {
			cantidad++;
		}

		i = i -> sgte;
	}

	return cantidad;
}

void eliminarContrincantePorSocket( int socket ) {
	struct Cliente *i = primero;
	while(i != NULL) {
		if ( i -> socket == socket ){
			i -> id_contrincante[0] = 0;
			i -> id_contrincante[1] = 0;
			strcpy(i -> nickname_contrincante, "");
		}
		i = i -> sgte; 
	}
}


char* recieveMessage(int socket, char* message){
  //printf("status message... ♔ \n");
  recv(socket, message, 1024, 0);
  return message;
}

void sendMessage(int socket, char* message){
  send(socket, message, 1024,0);
}

void *listenClient(void *socket_void){
	int *socket0 = socket_void;
	int socket = *socket0;
	char* message = malloc(sizeof(char)*1024);
	char id[2];
	while(1) {
		recv(socket, message, 1024, 0);
		printf("llego mensaje\n");
		int fid = message[0];
		if (fid == 1) {

		} 
		else if ( fid == 2 ) {
			char nickname[message[1]];
			for (int i = 2; i < message[1] + 2; ++i)
			{
				nickname[i-2] = message[i];
			}

			printf("NICKNAME: %s\n", nickname);

			//servidor responde con ID unico de 2 bytes como payload

			id[0] = (socket % 90) + 10;
			id[1] = (rand() % 90) + 10;
	
			char returnMessage[4];
			char fid_char = fid;
			char size = 2;
			returnMessage[0] = fid_char;
			returnMessage[1] = size;
			returnMessage[2] = id[0];
			returnMessage[3] = id[1];
			send(socket, returnMessage, 1024,0);
			
			//int k = atoi("8933"); // para leer el id
			//char c1 = k/100;
			//char c2 = k-(k/100)*100;
			//printf("%c - %c\n", c1, c2);

			agregarCliente(id, socket, nickname);

			

		}
		else if ( fid == 3 ) {	
			int count = 0;
			int size = 4;

			struct Cliente *i = primero;
			while (i != NULL) {
				if (i -> status == 1) {
					count++;
					size += 3 + sizeof i -> nickname;
				}
				i = i -> sgte;
			}

			//printf("SIZE: %i\n", size);

			char returnMessage[size];
			char count_str[4];
			sprintf(count_str, "%d", count);
			for (int i = 0; i < 4; ++i)
			{
				returnMessage[i] = count_str[i];
			}

			int avance = 4;
			struct Cliente *j = primero;
			while (j != NULL) {
				if (j -> status == 1) {					
					returnMessage[avance] = j -> id[0];
					returnMessage[avance+1] = j -> id[1];
					char byte_nickname_ch = strlen(j -> nickname) + 1;
					returnMessage[avance+2] = byte_nickname_ch;
					for (int n = avance + 3; n < avance + 3 + strlen(j -> nickname); ++n)
					{
						returnMessage[n] = j -> nickname[n - (avance + 3)];
					}
					returnMessage[avance + 3 + strlen(j -> nickname)] = '\0';
					avance += 3 + strlen(j -> nickname) + 1;
				}
				j = j -> sgte;
			}
			//printf("RET %s\n", returnMessage);
			send(socket, returnMessage, 1024, 0);

		}
		else if ( fid == 4 ) {
			char id_receptor[2];
			id_receptor[0] = message[2];
			id_receptor[1] = message[3];
			int existe = oponenteValido( id_receptor );
			if (existe == 0) {
				int socket_receptor = buscarSocketPorID( id_receptor );
				char *nickname = buscarNicknamePorSocket( socket );
				char message[40];
				int r_fid = 5;
				message[0] = r_fid;
				message[1] = 40;
				message[2] = id[0];
				message[3] = id[1];
				for (int i = 0; i < strlen(nickname)+1; ++i)
				{
					message[i+4] = nickname[i];
				}
				send(socket_receptor, message, 1024, 0);
			} else {
				char message[2];
				int r_fid = 4;
				message[0] = r_fid;
				message[1] = 0;
				send(socket, message, 1024, 0);
			}
			
		}
		else if ( fid == 5 ) {
			char id_receptor[2];
			id_receptor[0] = message[3];
			id_receptor[1] = message[4];
			int socket_receptor = buscarSocketPorID( id_receptor );

			char respuesta[3];
			int r_fid = 4;
			respuesta[0] = r_fid;
			respuesta[1] = 1;
			respuesta[2] = message[2];

			send(socket_receptor, respuesta, 1024, 0);

			if (respuesta[2] == '1'){
				agregarContrincante(id, id_receptor);
				cambiarEstadoPorID(id, 2);
				cambiarEstadoPorID(id_receptor, 2);

				char mensaje[7];
				int r_fid = 7;
				mensaje[0] = r_fid;
				mensaje[1] = 5;
				mensaje[2] = id[0];
				mensaje[3] = id[1];
				mensaje[4] = id_receptor[0];
				mensaje[5] = id_receptor[1];
				mensaje[6] = 1;
				send(socket, mensaje, 1024, 0);

				mensaje[2] = id[0];
				mensaje[3] = id[1];
				mensaje[4] = id_receptor[0];
				mensaje[5] = id_receptor[1];
				mensaje[6] = 0;
				send(socket_receptor, mensaje, 1024, 0);
			}
			
		}
		else if ( fid == 6 ) {
			int size  = message[1];
			int length_message = size - 2;
			char id_destino[2];
			id_destino[0] = message[2];
			id_destino[1] = message[3];
			char msg[length_message+2];
			msg[0] = 6;
			msg[1] = length_message;
			for (int i = 0; i < length_message; ++i)
			{
				msg[i+2] = message[i+4];
			}
			if ( id_destino[0] == 0 || id_destino[1] == 0 ) {
				// enviar a todos
				struct Cliente *i = primero;

				while (i != NULL) {
					if ( (i -> status == 0 ) && i -> socket != socket) {
						send(i -> socket, msg, 1024, 0);
					}

					i = i -> sgte;
				}

			} else {
				int socket_destino = buscarSocketPorID( id_destino );
				send(socket_destino, msg, 1024, 0);
			}
			
		}
		else if ( fid == 7 ) {
			
		}
		else if ( fid == 8 ) {
			int socket_receptor;
			socket_receptor = buscarSocketContrincantePorID(id);
			//printf("enviando movida a socket |%i|\n",socket_receptor);
			send(socket_receptor, message, 1024, 0);
		}
		else if ( fid == 9 ) {
			//printf("llego mensaje desconexion\n");
			char returnMessage[3];
			returnMessage[0] = 9;
			returnMessage[1] = 1;
			returnMessage[2] = 0;
			//printf("enviando mensje de termino socket: %i\n", socket);
			send(socket, returnMessage, 1024, 0);
			char returnMessage2[4];
			returnMessage2[0] = 10;
			returnMessage2[1] = 1;
			returnMessage2[2] = id[0];
			returnMessage2[3] = id[1];
			int socket_contrincante;
			socket_contrincante = buscarSocketContrincantePorID(id);
			if (socket_contrincante > -1){
				send(socket_contrincante, returnMessage2, 1024, 0);
				char resp_message_contrincante[3];
				recv(socket, resp_message_contrincante, 1024, 0);
				eliminarContrincantePorSocket(socket_contrincante);
				cambiarEstadoPorSocket(socket_contrincante, 0);
			}
			eliminarCliente( id );
			pthread_exit(NULL);
			
		}
		else if ( fid == 10 ) {
			char message_resp[3];
			message_resp[0] = fid;
			message_resp[1] = 1;
			message_resp[2] = 0;
			sendMessage(socket, message);
			
		}
		else if ( fid == 11 ) {
			
		}
		else if ( fid == 12 ) {
			
		}
		else if ( fid == 13 ) {
			
		}
		else if ( fid == 14 ) {
			char message[12];
			message[0] = fid;
			message[1] = 10;
			message[2] = cantidadClientesConectados();
			message[3] = cantidadClientesPorStatus(1); // status waiting
			message[4] = cantidadClientesPorStatus(2); // status playing
			message[5] = X;
			message[6] = Y;
			message[7] = Z;
			char id_str[4];
			sprintf(id_str, "%d", ID_IMPLEMENTACION);
			for (int i = 0; i < 4; ++i)
			{
				message[i + 8] = id_str[i];

			}
			sendMessage(socket, message);

		}
		else if ( fid == 15 ) {
			
		}
		else if ( fid == 16 ) {
			
		}
		else if ( fid == 17 ) {
			printf("WAITING STATUS -> %s\n", id);
			cambiarEstadoPorID(id, 1);
		}
		else if ( fid == 18 ) {
			printf("PLAYING STATUS -> %s\n", id);
			cambiarEstadoPorID(id, 2);
		}
		else if ( fid == 19 ) {
			printf("CONNECTED STATUS -> %s\n", id);
			cambiarEstadoPorID(id, 0);
		}
		else if ( fid == 20 ) {
			char message[1];
			message[0] = 20;
			//printf("socket receptor : %i  mensaje : %s\n", socket_receptor, message);
			send(socket, message, 1024, 0);			
		}
	}
	return NULL;
}

int initializeServer(char* ip, int port){
	int welcomeSocket, newSocket;
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	/*---- Creación del Socket. Se pasan 3 argumentos ----*/
	/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP en este caso) */
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*---- Configuración de la estructura del servidor ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set port number */
	serverAddr.sin_port = htons(port);
	/* Setear IP address como localhost */
	serverAddr.sin_addr.s_addr = inet_addr(ip);
	/* Setear todos los bits del padding en 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/*---- Bindear la struct al socket ----*/
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	/*---- Listen del socket, con un máximo de 5 conexiones (solo como ejemplo) ----*/
	if(listen(welcomeSocket,100) == 0)
		printf("INIT...\n");
	else
		printf("Error\n");

	addr_size = sizeof serverStorage;
	while(1) {
		int newSocket;
		newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
		printf("Conected %i \n", newSocket);
		pthread_t thread;
		pthread_create(&thread, NULL, listenClient, &newSocket);

	}
	
	return newSocket;
}

int main(int argc, char const *argv[])
{
	int socket;
    printf("Server\n");
    socket = initializeServer(IP, PORT);
	  
	return 0;
}