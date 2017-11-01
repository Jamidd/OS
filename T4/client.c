#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "math.h"
#define IP "0.0.0.0"
#define PORT 8080

char id_destino[2];

char id[2];

int color;

char* recieveMessage(int socket, char* message){
  printf("♔ ... \n");
  recv(socket, message, 1024, 0);
  return message;
}

void sendMessage(int socket, char* message){
    send(socket, message, 1024,0);
}

int iniviteClient(int clientSocket, char id_str_4[4] ){
	int id_int = atoi(id_str_4);
	char id[2];
	int c1, c2;
	c1 = id_int/100;
	c2 = id_int-(id_int/100)*100;
	id[0] = c1;
	id[1] = c2;
	int fid = 4;
	char ask_f4[4];
	ask_f4[0] = fid;
	ask_f4[1] = 2;
	ask_f4[2] = id[0];
	ask_f4[3] = id[1];
	sendMessage(clientSocket, ask_f4);
	char* message_answer = malloc( 1024 );
	message_answer = recieveMessage(clientSocket, message_answer);
	printf("The answer is: |%c|\n", message_answer[2]);
	if (message_answer[2] == '1'){
		id_destino[0] = id[0];
		id_destino[1] = id[1];
		printf("!!\n");
		return 1;
	}
	return 0;
}

void sendAsnwerToInvitation(int clientSocket, char answerr[1], int id_answ ){
	char id[2];
	int c1, c2;
	c1 = id_answ/100;
	c2 = id_answ-(id_answ/100)*100;
	id[0] = c1;
	id[1] = c2;
	int fid = 5;
	char answer[5];
	answer[0] = fid;
	answer[1] = 3;
	answer[2] = answerr[0];
	answer[3] = id[0];
	answer[4] = id[1];
	sendMessage(clientSocket, answer);
	return;
}

void sendMove(int client_socket, char filao, char colo, char fild, char cold, char pieza){
	char mensaje[7];
	int fid = 8;
	mensaje[0] = fid;
	mensaje[1] = 5;
	mensaje[2] = colo;
	mensaje[3] = filao;
	mensaje[4] = cold;
	mensaje[5] = fild;
	mensaje[6] = pieza;
	sendMessage(client_socket, mensaje);
}

void matchMakingList(int clientSocket) {
	int fid = 3;
	char ask_f3[1];
	ask_f3[0] = fid;
	sendMessage(clientSocket, ask_f3);
	char message_answer[1024];
	recv(clientSocket, message_answer, 1024, 0);
	char cant_clientes_str[4];
	for (int i = 0; i < 4; ++i)
	{
		cant_clientes_str[i] = message_answer[i];
	}
	int cant_clientes = atoi(cant_clientes_str);

	printf("Players:\n");
	int avance = 4;
	for (int i = 0; i < cant_clientes; ++i)
	{	
		int id_1, id_2, id;
		id_1 = message_answer[avance];
		id_2 = message_answer[avance+1];
		id = id_1*100+id_2;
		int byte_nickname = message_answer[avance+2];
		char nickname_cliente[byte_nickname];
		for (int j = avance + 3; j < avance + 3 + byte_nickname; ++j)
		{
			nickname_cliente[j - (avance + 3)] = message_answer[j];
		}
		printf("ID: %i - Nickname: %s\n", id, nickname_cliente);
		avance += 3 + byte_nickname;
	}

}

int initializeClient(char* ip, int port){
	int clientSocket;
	//char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	/*---- Creación del Socket. Se pasan 3 argumentos ----*/
	/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP en este caso) */
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*---- Configuración de la estructura del servidor ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set port number */
	serverAddr.sin_port = htons(port);
	/* Setear IP address como localhost */
	serverAddr.sin_addr.s_addr = inet_addr(ip);
	/* Setear todos los bits del padding en 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/*---- Conectar el socket al server ----*/
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	printf("Connected to server!\n");
	char nickname[20];
	printf("\nYour Nickname: ");
	scanf("%s", nickname);
	printf("\n");
	size_t size = strlen(nickname) + 1;
	char ch = size;
	char* message_init = malloc( size + 2 );
	int fid = 2;
	char fid_char = fid;
	message_init[0] = fid_char;
	message_init[1] = ch;
	for (int i = 2; i < size + 1; ++i)
	{
		message_init[i] = nickname[i - 2];
	}
	message_init[size + 1] = '\0';
	size = message_init[1];
	sendMessage(clientSocket, message_init);
	message_init = recieveMessage(clientSocket, message_init);
	id[0] = message_init[2];
	id[1] = message_init[3];
	int n, m;
	n = message_init[2];
	m = message_init[3];
	printf("tu id es: %i%i\n", n, m);
	// termina funcion 2
	
	return clientSocket;
}


int main(int argc, char const *argv[])
{
	int socket;
	printf("Client\n");
    socket = initializeClient(IP, PORT);
    printf("/i:id -> Invite Player ID, /a -> Available Players, /w Wait Invitation /q Quit\n");
    char message[1024];
    while (1) {

    	INICIO:
		printf("\nYour Message: ");
		scanf("%s", message);
		if (message[0] == '/') {
			if (message[1] == 'i'){
				char id_invite[4];
				for (int i = 0; i < 4; ++i)
				{
					id_invite[i] = message[i+3];
				}
				printf("Inviting %s\n", id_invite);
				int gameon;
				gameon = iniviteClient(socket, id_invite);
				if (gameon == 0){
					goto INICIO;
				}
			} 
			else if (message[1] == 'a') {
				matchMakingList(socket);
				goto INICIO;
			}
			else if (message[1] == 'w') {
				char message[1];
				message[0] = 17;
				sendMessage(socket, message);
				printf("waiting\n");
			}
			else if (message[1] == 'q') {
				int fid = 9;
				char message[1];
				message[0] = fid;
				sendMessage(socket, message);
				char response[3];
				recv(socket, response, 1024, 0);
				exit(0);
			}
			else {
				goto INICIO;
			}

			while(1) {
				char* msg = recieveMessage(socket, message);
				int fid = msg[0];
				if (fid == 5) {
					int c1, c2;
					c1 = msg[2];
					c2 = msg[3];
					int id_emisor = c1*100+c2;
					char emisor[40];
					for (int i = 0; i < strlen(msg)+1; ++i)
					{
						emisor[i] = msg[i+4];
					}
					printf("%i - %s Invited you, 1 - Accept, 0 - Decline\n", id_emisor, emisor);
					int decicion = 0;
					scanf("%i", &decicion);
					if (decicion == 1){
						sendAsnwerToInvitation(socket, "1", id_emisor);
						id_destino[0] = id_emisor/100;
						id_destino[1] = id_emisor-(id_emisor/100)*100;
					}
					else if (decicion == 0){
						sendAsnwerToInvitation(socket, "0", id_emisor);
					}
				}
				else if (fid == 7){
					if (msg[6] == 1){
						color = 1; //negro
						printf("soy negro\n");
					}
					else if (msg[6] == 0){
						color = 0; //blanco
						printf("soy blanco\n");
						char mov[10];
						printf("Cual es tu primer movimiento (/q Quit Game)\n");
						scanf("%s",mov);
						if (strcmp( mov, "/q" ) == 0) {
							int fid = 9;
							char message[1];
							message[0] = fid;
							sendMessage(socket, message);
							char response[3];
							recv(socket, response, 1024, 0);
							exit(0);
						} 
						else {
							sendMove(socket, mov[0], mov[1], mov[2], mov[3], mov[4]);
						}
						
					}
				}
				else if (fid == 8){
					printf(" el mov del otro jugador es: %c %c %c %c %c\n", msg[2], msg[3], msg[4], msg[5], msg[6]);
					printf("cual va a ser tu movimiento (/q Quit Game)\n");
					char mov[10];
					scanf("%s",mov);
					if (strcmp( mov, "/q" ) == 0) {
						int fid = 9;
						char message[1];
						message[0] = fid;
						sendMessage(socket, message);
						char response[3];
						recv(socket, response, 1024, 0);
						exit(0);
					}
					else {
						sendMove(socket, mov[0], mov[1], mov[2], mov[3], mov[4]);
					}
				}
				else if (fid == 10){
					printf("Se ha salido el otro jugador\n");
					int fid = 10;
					char message[3];
					message[0] = fid;
					message[1] = 1;
					message[2] = 0;
					sendMessage(socket, message);
					goto INICIO;
				}

			}
		} else {

			//sendMessage(socket, message);

		}

		
    }
	
	return 0;
}
