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
#include <signal.h> 
#define IP "192.168.1.46"
#define PORT 8080

char id_destino[2];
char ppid[2];
char id[2];
char nickname[20];

int color;
int lock = 1;

int sockete;

char tablero[630] = "  | a | b | c | d | e | f | g | h |\n-----------------------------------\n1 |   |   |   |   |   |   |   |   |\n-----------------------------------\n2 |   |   |   |   |   |   |   |   |\n-----------------------------------\n3 |   |   |   |   |   |   |   |   |\n-----------------------------------\n4 |   |   |   |   |   |   |   |   |\n-----------------------------------\n5 |   |   |   |   |   |   |   |   |\n-----------------------------------\n6 |   |   |   |   |   |   |   |   |\n-----------------------------------\n7 |   |   |   |   |   |   |   |   |\n-----------------------------------\n8 |   |   |   |   |   |   |   |   |";

void printtablero(char *blancos, char *negros){
	/*
	vamos a guardar en un string cada 2 bits (fila- columna) es una ficha y van rey-reina-alfil-alfil-cabalo-caballo-torre-torre-peon-peon-peon-peon-peon-peon-peon-peon
	*/
	char fil0[73] = "  | a | b | c | d | e | f | g | h |\n-----------------------------------\n";
	char fil1[73] = "1 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil2[73] = "2 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil3[73] = "3 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil4[73] = "4 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil5[73] = "5 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil6[73] = "6 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil7[73] = "7 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";
	char fil8[73] = "8 |   |   |   |   |   |   |   |   |\n-----------------------------------\n";

	int col;
	for(int i = 0; i < 32; i = i + 2){
		if (blancos[i] == 1){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil1[col*4] = 97;
			}else if (i == 2){ //reina
				fil1[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil1[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil1[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil1[col*4] = 101;
			}else{ //peones
				fil1[col*4] = 102;
			}
		}else if (blancos[i] == 2){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil2[col*4] = 97;
			}else if (i == 2){ //reina
				fil2[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil2[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil2[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil2[col*4] = 101;
			}else{ //peones
				fil2[col*4] = 102;
			}
		}else if (blancos[i] == 3){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil3[col*4] = 97;
			}else if (i == 2){ //reina
				fil3[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil3[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil3[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil3[col*4] = 101;
			}else{ //peones
				fil3[col*4] = 102;
			}
		}else if (blancos[i] == 4){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil4[col*4] = 97;
			}else if (i == 2){ //reina
				fil4[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil4[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil4[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil4[col*4] = 101;
			}else{ //peones
				fil4[col*4] = 102;
			}
		}else if (blancos[i] == 5){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil5[col*4] = 97;
			}else if (i == 2){ //reina
				fil5[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil5[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil5[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil5[col*4] = 101;
			}else{ //peones
				fil5[col*4] = 102;
			}
		}else if (blancos[i] == 6){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil6[col*4] = 97;
			}else if (i == 2){ //reina
				fil6[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil6[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil6[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil6[col*4] = 101;
			}else{ //peones
				fil6[col*4] = 102;
			}
		}else if (blancos[i] == 7){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil7[col*4] = 97;
			}else if (i == 2){ //reina
				fil7[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil7[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil7[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil7[col*4] = 101;
			}else{ //peones
				fil7[col*4] = 102;
			}
		}else if (blancos[i] == 8){
			col = blancos[i+1];
			if (i == 0){ //rey
				fil8[col*4] = 97;
			}else if (i == 2){ //reina
				fil8[col*4] = 98;
			}else if (i == 4 || i == 6){ // alfil
				fil8[col*4] = 99;				
			}else if (i == 8 || i == 10){ // caballo
				fil8[col*4] = 100;
			}else if (i == 12 || i == 14){ //torre
				fil8[col*4] = 101;
			}else{ //peones
				fil8[col*4] = 102;
			}
		}
	}

	for(int i = 0; i < 32; i = i + 2){
		if (negros[i] == 1){
			col = negros[i+1];
			if (i == 0){ //rey
				fil1[col*4] = 65;
			}else if (i == 2){ //reina
				fil1[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil1[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil1[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil1[col*4] = 69;
			}else{ //peones
				fil1[col*4] = 70;
			}
		}else if (negros[i] == 2){
			col = negros[i+1];
			if (i == 0){ //rey
				fil2[col*4] = 65;
			}else if (i == 2){ //reina
				fil2[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil2[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil2[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil2[col*4] = 69;
			}else{ //peones
				fil2[col*4] = 70;
			}
		}else if (negros[i] == 3){
			col = negros[i+1];
			if (i == 0){ //rey
				fil3[col*4] = 65;
			}else if (i == 2){ //reina
				fil3[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil3[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil3[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil3[col*4] = 69;
			}else{ //peones
				fil3[col*4] = 70;
			}
		}else if (negros[i] == 4){
			col = negros[i+1];
			if (i == 0){ //rey
				fil4[col*4] = 65;
			}else if (i == 2){ //reina
				fil4[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil4[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil4[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil4[col*4] = 69;
			}else{ //peones
				fil4[col*4] = 70;
			}
		}else if (negros[i] == 5){
			col = negros[i+1];
			if (i == 0){ //rey
				fil5[col*4] = 65;
			}else if (i == 2){ //reina
				fil5[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil5[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil5[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil5[col*4] = 69;
			}else{ //peones
				fil5[col*4] = 70;
			}
		}else if (negros[i] == 6){
			col = negros[i+1];
			if (i == 0){ //rey
				fil6[col*4] = 65;
			}else if (i == 2){ //reina
				fil6[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil6[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil6[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil6[col*4] = 69;
			}else{ //peones
				fil6[col*4] = 70;
			}
		}else if (negros[i] == 7){
			col = negros[i+1];
			if (i == 0){ //rey
				fil7[col*4] = 65;
			}else if (i == 2){ //reina
				fil7[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil7[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil7[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil7[col*4] = 69;
			}else{ //peones
				fil7[col*4] = 70;
			}
		}else if (negros[i] == 8){
			col = negros[i+1];
			if (i == 0){ //rey
				fil8[col*4] = 65;
			}else if (i == 2){ //reina
				fil8[col*4] = 66;
			}else if (i == 4 || i == 6){ // alfil
				fil8[col*4] = 67;				
			}else if (i == 8 || i == 10){ // caballo
				fil8[col*4] = 68;
			}else if (i == 12 || i == 14){ //torre
				fil8[col*4] = 69;
			}else{ //peones
				fil8[col*4] = 70;
			}
		}

	}
	printf("%s", fil0);
	for (int i = 0; i < 73; i++){
		if (64 < fil1[i] && fil1[i] < 123){
			if (fil1[i] == 65){
				printf("♚");
			}else if (fil1[i] == 66){
				printf("♛");
			}else if (fil1[i] == 67){
				printf("♝");
			}else if (fil1[i] == 68){
				printf("♞");
			}else if (fil1[i] == 69){
				printf("♜");
			}else if (fil1[i] == 70){
				printf("♟");
			}else if (fil1[i] == 97){
				printf("♔");
			}else if (fil1[i] == 98){
				printf("♕");
			}else if (fil1[i] == 99){
				printf("♗");
			}else if (fil1[i] == 100){
				printf("♘");
			}else if (fil1[i] == 101){
				printf("♖");
			}else if (fil1[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil1[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil2[i]) && (fil2[i] < 123)){
			if (fil2[i] == 65){
				printf("♚");
			}else if (fil2[i] == 66){
				printf("♛");
			}else if (fil2[i] == 67){
				printf("♝");
			}else if (fil2[i] == 68){
				printf("♞");
			}else if (fil2[i] == 69){
				printf("♜");
			}else if (fil2[i] == 70){
				printf("♟");
			}else if (fil2[i] == 97){
				printf("♔");
			}else if (fil2[i] == 98){
				printf("♕");
			}else if (fil2[i] == 99){
				printf("♗");
			}else if (fil2[i] == 100){
				printf("♘");
			}else if (fil2[i] == 101){
				printf("♖");
			}else if (fil2[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil2[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil3[i]) && (fil3[i] < 123)){
			if (fil3[i] == 65){
				printf("♚");
			}else if (fil3[i] == 66){
				printf("♛");
			}else if (fil3[i] == 67){
				printf("♝");
			}else if (fil3[i] == 68){
				printf("♞");
			}else if (fil3[i] == 69){
				printf("♜");
			}else if (fil3[i] == 70){
				printf("♟");
			}else if (fil3[i] == 97){
				printf("♔");
			}else if (fil3[i] == 98){
				printf("♕");
			}else if (fil3[i] == 99){
				printf("♗");
			}else if (fil3[i] == 100){
				printf("♘");
			}else if (fil3[i] == 101){
				printf("♖");
			}else if (fil3[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil3[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil4[i]) && (fil4[i] < 123)){
			if (fil4[i] == 65){
				printf("♚");
			}else if (fil4[i] == 66){
				printf("♛");
			}else if (fil4[i] == 67){
				printf("♝");
			}else if (fil4[i] == 68){
				printf("♞");
			}else if (fil4[i] == 69){
				printf("♜");
			}else if (fil4[i] == 70){
				printf("♟");
			}else if (fil4[i] == 97){
				printf("♔");
			}else if (fil4[i] == 98){
				printf("♕");
			}else if (fil4[i] == 99){
				printf("♗");
			}else if (fil4[i] == 100){
				printf("♘");
			}else if (fil4[i] == 101){
				printf("♖");
			}else if (fil4[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil4[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil5[i]) && (fil5[i] < 123)){
			if (fil5[i] == 65){
				printf("♚");
			}else if (fil5[i] == 66){
				printf("♛");
			}else if (fil5[i] == 67){
				printf("♝");
			}else if (fil5[i] == 68){
				printf("♞");
			}else if (fil5[i] == 69){
				printf("♜");
			}else if (fil5[i] == 70){
				printf("♟");
			}else if (fil5[i] == 97){
				printf("♔");
			}else if (fil5[i] == 98){
				printf("♕");
			}else if (fil5[i] == 99){
				printf("♗");
			}else if (fil5[i] == 100){
				printf("♘");
			}else if (fil5[i] == 101){
				printf("♖");
			}else if (fil5[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil5[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil6[i]) && (fil6[i] < 123)){
			if (fil6[i] == 65){
				printf("♚");
			}else if (fil6[i] == 66){
				printf("♛");
			}else if (fil6[i] == 67){
				printf("♝");
			}else if (fil6[i] == 68){
				printf("♞");
			}else if (fil6[i] == 69){
				printf("♜");
			}else if (fil6[i] == 70){
				printf("♟");
			}else if (fil6[i] == 97){
				printf("♔");
			}else if (fil6[i] == 98){
				printf("♕");
			}else if (fil6[i] == 99){
				printf("♗");
			}else if (fil6[i] == 100){
				printf("♘");
			}else if (fil6[i] == 101){
				printf("♖");
			}else if (fil6[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil6[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil7[i]) && (fil7[i] < 123)){
			if (fil7[i] == 65){
				printf("♚");
			}else if (fil7[i] == 66){
				printf("♛");
			}else if (fil7[i] == 67){
				printf("♝");
			}else if (fil7[i] == 68){
				printf("♞");
			}else if (fil7[i] == 69){
				printf("♜");
			}else if (fil7[i] == 70){
				printf("♟");
			}else if (fil7[i] == 97){
				printf("♔");
			}else if (fil7[i] == 98){
				printf("♕");
			}else if (fil7[i] == 99){
				printf("♗");
			}else if (fil7[i] == 100){
				printf("♘");
			}else if (fil7[i] == 101){
				printf("♖");
			}else if (fil7[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil7[i]);
		}
	}
	for (int i = 0; i < 73; i++){
		if ((64 < fil8[i]) && (fil8[i] < 123)){
			if (fil8[i] == 65){
				printf("♚");
			}else if (fil8[i] == 66){
				printf("♛");
			}else if (fil8[i] == 67){
				printf("♝");
			}else if (fil8[i] == 68){
				printf("♞");
			}else if (fil8[i] == 69){
				printf("♜");
			}else if (fil8[i] == 70){
				printf("♟");
			}else if (fil8[i] == 97){
				printf("♔");
			}else if (fil8[i] == 98){
				printf("♕");
			}else if (fil8[i] == 99){
				printf("♗");
			}else if (fil8[i] == 100){
				printf("♘");
			}else if (fil8[i] == 101){
				printf("♖");
			}else if (fil8[i] == 102){
				printf("♙");}
		}else{
			printf("%c", fil8[i]);
		}
	}
}

char* recieveMessage(int socket, char* message){
  //printf("♔ ... \n");
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
	if (message_answer[0] == 4 && message_answer[1] == 0) {
		printf("This player is not waiting\n");
		return 2;
	}
	//printf("The answer is: |%c|\n", message_answer[2]);
	if (message_answer[2] == '1'){
		id_destino[0] = id[0];
		id_destino[1] = id[1];
		return 1;
	}
	return 0;
}

void cancel_thread(int clientSocket, char idd[2]){
	int fid = 20;
	char cancel_msg[1];
	cancel_msg[0] = fid;
	sendMessage(clientSocket, cancel_msg);
	while(lock);
	lock = 1;
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
	mensaje[2] = filao;
	mensaje[3] = colo;
	mensaje[4] = fild;
	mensaje[5] = cold;
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
	if (cant_clientes == 0) {
		printf("No waiting players\n");
	}
	else {
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
	printf("\nYour Nickname: ");
	scanf("%s", nickname);
	printf("\n");
	size_t size = strlen(nickname) + 1;
	char ch = size;
	char message_init[1024];
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
	recieveMessage(clientSocket, message_init);
	id[0] = message_init[2];
	id[1] = message_init[3];
	ppid[0] = message_init[2];
	ppid[1] = message_init[3];
	int n, m;
	n = message_init[2];
	m = message_init[3];
	printf("Your ID is: %i%i\n", n, m);
	// termina funcion 2
	return clientSocket;
}

void initBoard(char *blancos, char *negros){
	negros[0] = 1;
	negros[1] = 4;
	negros[2] = 1;
	negros[3] = 5;
	negros[4] = 1;
	negros[5] = 3;
	negros[6] = 1;
	negros[7] = 6;
	negros[8] = 1;
	negros[9] = 2;
	negros[10] = 1;
	negros[11] = 7;
	negros[12] = 1;
	negros[13] = 1;
	negros[14] = 1;
	negros[15] = 8;
	negros[16] = 2;
	negros[17] = 1;
	negros[18] = 2;
	negros[19] = 2;
	negros[20] = 2;
	negros[21] = 3;
	negros[22] = 2;
	negros[23] = 4;
	negros[24] = 2;
	negros[25] = 5;
	negros[26] = 2;
	negros[27] = 6;
	negros[28] = 2;
	negros[29] = 7;
	negros[30] = 2;
	negros[31] = 8;

	blancos[0] = 8;
	blancos[1] = 4;
	blancos[2] = 8;
	blancos[3] = 5;
	blancos[4] = 8;
	blancos[5] = 3;
	blancos[6] = 8;
	blancos[7] = 6;
	blancos[8] = 8;
	blancos[9] = 2;
	blancos[10] = 8;
	blancos[11] = 7;
	blancos[12] = 8;
	blancos[13] = 1;
	blancos[14] = 8;
	blancos[15] = 8;
	blancos[16] = 7;
	blancos[17] = 1;
	blancos[18] = 7;
	blancos[19] = 2;
	blancos[20] = 7;
	blancos[21] = 3;
	blancos[22] = 7;
	blancos[23] = 4;
	blancos[24] = 7;
	blancos[25] = 5;
	blancos[26] = 7;
	blancos[27] = 6;
	blancos[28] = 7;
	blancos[29] = 7;
	blancos[30] = 7;
	blancos[31] = 8;
}
int cambiarpieza(char *blancos, char *negros, int color, char filo, char colo, char fild, char cold, char p){
	//printf("%c %c %c %c %c\n", filo, colo, fild, cold, p);
	//printf("%i %i %i %i %c\n", filo - 48, colo - 96, fild - 48, cold - 96, p);
	if (color == 0){ // reviso que escogio una pieza existente
		for(int i = 0; i < 32; i = i + 2){ // reviso que no exista nadie en el destino, hay que cambiarlo para ver si se come a alguien
			if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96)
				return 1;
		}
		if (p == "R"[0]){ //rey
			if ( (abs(fild - filo) != 1 ||  cold != colo) && (abs(cold - colo) != 1 && fild != filo) && (abs(fild - filo) != abs(cold - colo) || abs(fild - filo) != 1) ){
				return 1;
			}
			if (blancos[0] == filo - 48 && blancos[1] == colo - 96){
				blancos[0] = fild - 48;
				blancos[1] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}

		}else if (p == "r"[0]){ //reina
			if ((abs(fild - filo) != abs(cold - colo)) && (cold != colo && fild != filo)) {
				return 1;
			}
			if ( abs(fild - filo) == abs(cold - colo) ) {
				if ( fild < filo ) {

					if ( cold < colo ) { // hacia arriba - izquierda
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k){
									return 1;
								}
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k){
									return 1;
								}
							}
						}
					} else { // hacia arriba - derecha
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k){
									return 1;
								}
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k){
									return 1;
								}
							}
						}
					}

				} else {
					if ( cold < colo ) { // hacia abajo - izquierda
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k){
									return 1;
								}
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k){
									return 1;
								}
							}
						}
					} else { // hacia abajo - derecha
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k){
									return 1;
								}
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k){
									return 1;
								}
							}
						}
					}
				}
			} else if (cold == colo || fild == filo) {
				if ( cold == colo ){
					if ( fild < filo ) { // hacia arriba
						for (int k = 1; k < abs(fild - filo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96)
									return 1;
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96)
									return 1;
							}
						}	
					} else { // hacia abajo
						for (int k = 1; k < abs(fild - filo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96)
									return 1;
								if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96)
									return 1;
							}
						}
					}
				} else if (fild == filo){
					if ( cold < colo ) { // hacia izquierda
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 + k)
									return 1;
								if (negros[i] == fild - 48 && negros[i+1] == cold - 96 + k)
									return 1;
							}
						}
					} else { // hacia derecha
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 - k)
									return 1;
								if (negros[i] == fild - 48 && negros[i+1] == cold - 96 - k)
									return 1;
							}
						}
					}
				}
			}
			if (blancos[2] == filo - 48 && blancos[3] == colo - 96){
				blancos[2] = fild - 48;
				blancos[3] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		} else if (p == "a"[0]){
			if ( abs(fild - filo) != abs(cold - colo) ) {
				return 1;
			}
			// revisar tope
			if ( fild < filo ) {

				if ( cold < colo ) { // hacia arriba - izquierda
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k)
								return 1;
							if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k)
								return 1;
						}
					}
				} else { // hacia arriba - derecha
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 - k)
								return 1;
							if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 - k)
								return 1;
						}
					}
				}

			} else {
				if ( cold < colo ) { // hacia abajo - izquierda
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96 + k)
								return 1;
							if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96 + k)
								return 1;
						}
					}
				} else { // hacia abajo - derecha
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96 - k)
								return 1;
							if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96 - k)
								return 1;
						}
					}
				}
			}
			if (blancos[4] == filo - 48 && blancos[5] == colo - 96){
				blancos[4] = fild - 48;
				blancos[5] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else if (blancos[6] == filo - 48 && blancos[7] == colo - 96){
				blancos[6] = fild - 48;
				blancos[7] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "c"[0]){
			if ( (abs(fild - filo) != 2 || abs(cold - colo) != 1 ) && (abs(fild - filo) != 1 || abs(cold - colo) != 2 ) ) {
				return 1;
			}
			if (blancos[8] == filo - 48 && blancos[9] == colo - 96){
				blancos[8] = fild - 48;
				blancos[9] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else if (blancos[10] == filo - 48 && blancos[11] == colo - 96){
				blancos[10] = fild - 48;
				blancos[11] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "t"[0]){
			if (cold != colo && fild != filo) {
				return 1;
			}
			// revisar tope
			if ( cold == colo ){
				if ( fild < filo ) { // hacia arriba
					for (int k = 1; k < abs(fild - filo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96)
								return 1;
							if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96)
								return 1;
						}
					}	
				} else { // hacia abajo
					for (int k = 1; k < abs(fild - filo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96)
								return 1;
							if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96)
								return 1;
						}
					}
				}
			} else if (fild == filo){
				if ( cold < colo ) { // hacia izquierda
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 + k)
								return 1;
							if (negros[i] == fild - 48 && negros[i+1] == cold - 96 + k)
								return 1;
						}
					}
				} else { // hacia derecha
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 - k)
								return 1;
							if (negros[i] == fild - 48 && negros[i+1] == cold - 96 - k)
								return 1;
						}
					}
				}
			}
			if (blancos[12] == filo - 48 && blancos[13] == colo - 96){
				blancos[12] = fild - 48;
				blancos[13] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else if (blancos[14] == filo - 48 && blancos[15] == colo - 96){
				blancos[14] = fild - 48;
				blancos[15] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "p"[0]){
			if ( abs(cold - colo) == 1 && fild == filo - 1 ) {
				int come = 0;
				for(int i = 0; i < 32; i = i + 2){
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96){
						come++;
						negros[i] = 0 - 48;
						negros[i+1] = 0 - 96;
					}
				}
				if ( come == 0 ) {
					return 1;
				}
			} else {
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96)
						return 1;
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96)
						return 1;
				}
				if( cold != colo ) {
					return 1;
				}
				if (filo == 55) {
					if (fild != filo - 1 && fild != filo - 2) {
						return 1;
					}
					if ( fild == filo - 2 ) {
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild + 1 - 48 && blancos[i+1] == cold - 96)
								return 1;
							if (negros[i] == fild + 1 - 48 && negros[i+1] == cold - 96)
								return 1;
						}
					}
				} else {
					if (fild != filo - 1) {
						return 1;
					}
				}
			}
			if (blancos[16] == filo - 48 && blancos[17] == colo - 96){
				blancos[16] = fild - 48;
				blancos[17] = cold - 96;
				return 0;
			}
			else if (blancos[18] == filo - 48 && blancos[19] == colo - 96){
				blancos[18] = fild - 48;
				blancos[19] = cold - 96;
				return 0;
			}
			else if (blancos[20] == filo - 48 && blancos[21] == colo - 96){
				blancos[20] = fild - 48;
				blancos[21] = cold - 96;
				return 0;
			}
			else if (blancos[22] == filo - 48 && blancos[23] == colo - 96){
				blancos[22] = fild - 48;
				blancos[23] = cold - 96;
				return 0;
			}
			else if (blancos[24] == filo - 48 && blancos[25] == colo - 96){
				blancos[24] = fild - 48;
				blancos[25] = cold - 96;
				return 0;
			}
			else if (blancos[26] == filo - 48 && blancos[27] == colo - 96){
				blancos[26] = fild - 48;
				blancos[27] = cold - 96;
				return 0;
			}
			else if (blancos[28] == filo - 48 && blancos[29] == colo - 96){
				blancos[28] = fild - 48;
				blancos[29] = cold - 96;
				return 0;
			}
			else if (blancos[30] == filo - 48 && blancos[31] == colo - 96){
				blancos[30] = fild - 48;
				blancos[31] = cold - 96;
				return 0;
			}
			else{
				return 1;
			}
			
		}else{
			return 1;
		}
	}
	else if (color == 1){//negros
		for(int i = 0; i < 32; i = i + 2){
			if (negros[i] == fild - 48 && negros[i+1] == cold - 96)
				return 1;
		}
		if (p == "R"[0]){ //rey
			if ( (abs(fild - filo) != 1 ||  cold != colo) && (abs(cold - colo) != 1 && fild != filo) && (abs(fild - filo) != abs(cold - colo) || abs(fild - filo) != 1) ){
				return 1;
			}
			if (negros[0] == filo - 48 && negros[1] == colo - 96){
				negros[0] = fild - 48;
				negros[1] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}

		}else if (p == "r"[0]){ //reina
			if (abs(fild - filo) != abs(cold - colo) && (cold != colo && fild != filo)) {
				return 1;
			}
			if ( abs(fild - filo) == abs(cold - colo) ) {
				if ( fild < filo ) {

					if ( cold < colo ) { // hacia arriba - izquierda
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k)
									return 1;
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k)
									return 1;
							}
						}
					} else { // hacia arriba - derecha
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 - k)
									return 1;
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 - k)
									return 1;
							}
						}
					}

				} else {
					if ( cold < colo ) { // hacia abajo - izquierda
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96 + k)
									return 1;
								if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96 + k)
									return 1;
							}
						}
					} else { // hacia abajo - derecha
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96 - k)
									return 1;
								if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96 - k)
									return 1;
							}
						}
					}
				}
			} else if (cold == colo || fild == filo) {
				if ( cold == colo ){
					if ( fild < filo ) { // hacia arriba
						for (int k = 1; k < abs(fild - filo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96)
									return 1;
								if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96)
									return 1;
							}
						}	
					} else { // hacia abajo
						for (int k = 1; k < abs(fild - filo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96)
									return 1;
								if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96)
									return 1;
							}
						}
					}
				} else if (fild == filo){
					if ( cold < colo ) { // hacia izquierda
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 + k)
									return 1;
								if (negros[i] == fild - 48 && negros[i+1] == cold - 96 + k)
									return 1;
							}
						}
					} else { // hacia derecha
						for (int k = 1; k < abs(cold - colo); ++k)
						{
							for(int i = 0; i < 32; i = i + 2){
								if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 - k)
									return 1;
								if (negros[i] == fild - 48 && negros[i+1] == cold - 96 - k)
									return 1;
							}
						}
					}
				}
			}
			if (negros[2] == filo - 48 && negros[3] == colo - 96){
				negros[2] = fild - 48;
				negros[3] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "a"[0]){
			if ( abs(fild - filo) != abs(cold - colo) ) {
				return 1;
			}
			// revisar tope
			if ( fild < filo ) {

				if ( cold < colo ) { // hacia arriba - izquierda
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 + k)
								return 1;
							if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 + k)
								return 1;
						}
					}
				} else { // hacia arriba - derecha
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96 - k)
								return 1;
							if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96 - k)
								return 1;
						}
					}
				}

			} else {
				if ( cold < colo ) { // hacia abajo - izquierda
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96 + k)
								return 1;
							if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96 + k)
								return 1;
						}
					}
				} else { // hacia abajo - derecha
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96 - k)
								return 1;
							if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96 - k)
								return 1;
						}
					}
				}
			}
			if (negros[4] == filo - 48 && negros[5] == colo - 96){
				negros[4] = fild - 48;
				negros[5] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else if (negros[6] == filo - 48 && negros[7] == colo - 96){
				negros[6] = fild - 48;
				negros[7] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "c"[0]){
			if ( (abs(fild - filo) != 2 || abs(cold - colo) != 1 ) && (abs(fild - filo) != 1 || abs(cold - colo) != 2 ) ) {
				return 1;
			}
			if (negros[8] == filo - 48 && negros[9] == colo - 96){
				negros[8] = fild - 48;
				negros[9] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else if (negros[10] == filo - 48 && negros[11] == colo - 96){
				negros[10] = fild - 48;
				negros[11] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "t"[0]){
			if (cold != colo && fild != filo) {
				return 1;
			}
			// revisar tope
			if ( cold == colo ){
				if ( fild < filo ) { // hacia arriba
					for (int k = 1; k < abs(fild - filo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 + k && blancos[i+1] == cold - 96)
								return 1;
							if (negros[i] == fild - 48 + k && negros[i+1] == cold - 96)
								return 1;
						}
					}	
				} else { // hacia abajo
					for (int k = 1; k < abs(fild - filo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 - k && blancos[i+1] == cold - 96)
								return 1;
							if (negros[i] == fild - 48 - k && negros[i+1] == cold - 96)
								return 1;
						}
					}
				}
			} else if (fild == filo){
				if ( cold < colo ) { // hacia izquierda
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 + k)
								return 1;
							if (negros[i] == fild - 48 && negros[i+1] == cold - 96 + k)
								return 1;
						}
					}
				} else { // hacia derecha
					for (int k = 1; k < abs(cold - colo); ++k)
					{
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96 - k)
								return 1;
							if (negros[i] == fild - 48 && negros[i+1] == cold - 96 - k)
								return 1;
						}
					}
				}
			}
			if (negros[12] == filo - 48 && negros[13] == colo - 96){
				negros[12] = fild - 48;
				negros[13] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else if (negros[14] == filo - 48 && negros[15] == colo - 96){
				negros[14] = fild - 48;
				negros[15] = cold - 96;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				return 0;
			}
			else{
				return 1;
			}
			
		}else if (p == "p"[0]){
			if ( abs(cold - colo) == 1 && fild == filo + 1 ) {
				int come = 0;
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96){
						come++;
						blancos[i] = 0 - 48;
						blancos[i+1] = 0 - 96;
					}
				}
				if ( come == 0 ) {
					return 1;
				}
			} else {
				for(int i = 0; i < 32; i = i + 2){
					if (blancos[i] == fild - 48 && blancos[i+1] == cold - 96)
						return 1;
					if (negros[i] == fild - 48 && negros[i+1] == cold - 96)
						return 1;
				}
				if( cold != colo ) {
					return 1;
				}
				if (filo == 50) {
					if (fild != filo + 1 && fild != filo + 2) {
						return 1;
					}
					if ( fild == filo + 2 ) {
						for(int i = 0; i < 32; i = i + 2){
							if (blancos[i] == fild - 1 - 48 && blancos[i+1] == cold - 96)
								return 1;
							if (negros[i] == fild - 1 - 48 && negros[i+1] == cold - 96)
								return 1;
						}
					}
				} else {
					if (fild != filo + 1) {
						return 1;
					}
				}
			}
			if (negros[16] == filo - 48 && negros[17] == colo - 96){
				negros[16] = fild - 48;
				negros[17] = cold - 96;
				return 0;
			}
			else if (negros[18] == filo - 48 && negros[19] == colo - 96){
				negros[18] = fild - 48;
				negros[19] = cold - 96;
				return 0;
			}
			else if (negros[20] == filo - 48 && negros[21] == colo - 96){
				negros[20] = fild - 48;
				negros[21] = cold - 96;
				return 0;
			}
			else if (negros[22] == filo - 48 && negros[23] == colo - 96){
				negros[22] = fild - 48;
				negros[23] = cold - 96;
				return 0;
			}
			else if (negros[24] == filo - 48 && negros[25] == colo - 96){
				negros[24] = fild - 48;
				negros[25] = cold - 96;
				return 0;
			}
			else if (negros[26] == filo - 48 && negros[27] == colo - 96){
				negros[26] = fild - 48;
				negros[27] = cold - 96;
				return 0;
			}
			else if (negros[28] == filo - 48 && negros[29] == colo - 96){
				negros[28] = fild - 48;
				negros[29] = cold - 96;
				return 0;
			}
			else if (negros[30] == filo - 48 && negros[31] == colo - 96){
				negros[30] = fild - 48;
				negros[31] = cold - 96;
				return 0;
			}
			else{
				return 1;
			}
			
		}else{
			return 1;
		}

	}else{
		return 1;
	}
}

void *listenChatMessage(void *socket_void) {
	
	int *socket0 = socket_void;
	int socket = *socket0;
	while (1) {
		char message[1024];
		recv(socket, message, 1024, 0);
		if (message[0] == 20){
			lock = 0;
			return NULL;
		}

		if (message[0] == 6) {
			char msg[message[1]];
	
			for (int i = 0; i < message[1]; ++i)
			{
				msg[i] = message[i+2];
			}
	
			printf("%s\n", msg);
		}
	}
	return NULL;
}

void SigInt_Handler(int n_signal)
{	
	//printf("termine\n");
    int fid = 9;
	char message[1];
	message[0] = fid;
	//char response[1025];
	sendMessage(sockete, message);
	//recv(sockete, response, 1024, 0);
	//printf("res puesta %s\n", response);
	exit(0);
}

void SigBreak_Handler(int n_signal)
{	
	//printf("termine2\n");
    int fid = 9;
	char message[1];
	message[0] = fid;
	//char response[1025];
	sendMessage(sockete, message);
	//recv(sockete, response, 1024, 0);
	//printf("res puesta %s\n", response);
	exit(0);
}

int main(int argc, char const *argv[])
{	

	signal(SIGINT, &SigInt_Handler);
    signal(SIGTSTP, &SigBreak_Handler);
    signal(SIGHUP, &SigBreak_Handler);

	int socket;
    socket = initializeClient(IP, PORT);
	sockete = socket;

	char negros[32];
	char blancos[32];
	
	printf("Client\n");
    pthread_t thread;
	pthread_create(&thread, NULL, listenChatMessage, &socket);
    printf("/i:id -> Invite Waiting Player ID, /a -> Waiting Players, /w -> Wait Invitation /s -> Server Info /q -> Quit\n");
    char message[1024];
    while (1) {
		

    	INICIO:
		fgets (message, 1024, stdin);
		if ((strlen(message)>0) && (message[strlen (message) - 1] == '\n'))
        	message[strlen (message) - 1] = '\0';
		if (message[0] == '/') {
			if (message[1] == 'i'){
				cancel_thread(socket, ppid);
				char id_invite[4];
				for (int i = 0; i < 4; ++i)
				{
					id_invite[i] = message[i+3];
				}
				printf("Inviting %s\n", id_invite);
				int gameon;
				gameon = iniviteClient(socket, id_invite);
				if (gameon != 1){
					pthread_t thread;
					pthread_create(&thread, NULL, listenChatMessage, &socket);
					goto INICIO;
				}
			} 
			else if (message[1] == 'a') {
				cancel_thread(socket, ppid);
				matchMakingList(socket);
				pthread_t thread;
				pthread_create(&thread, NULL, listenChatMessage, &socket);
				goto INICIO;
			}
			else if (message[1] == 'w') {
				cancel_thread(socket, ppid);
				char message[1];
				message[0] = 17;
				sendMessage(socket, message);
				printf("waiting\n");
			}
			else if (message[1] == 's') {
				cancel_thread(socket, ppid);
				char message[1];
				message[0] = 14;
				sendMessage(socket, message);
				char response[12];
				recv(socket, response, 1024, 0);
				int cantiidad_conectados = response[2];
				int cantiidad_waiting = response[3];
				int cantiidad_playing = response[4];
				int X = response[5];
				int Y = response[6];
				int Z = response[7];
				char id_implementacion_str[4];
				for (int i = 0; i < 4; ++i)
				{
					id_implementacion_str[i] = response[8 + i];
				}
				int id_implementacion = atoi(id_implementacion_str);
				printf("Server Info\n");
				printf("	Players Online: %i\n", cantiidad_conectados);
				printf("	Players Waiting: %i\n", cantiidad_waiting);
				printf("	Players Playing: %i\n", cantiidad_playing);
				printf("	Version: %i.%i.%i\n", X, Y, Z);
				printf("	ID: %i\n", id_implementacion);
				pthread_t thread;
				pthread_create(&thread, NULL, listenChatMessage, &socket);
				goto INICIO;
			}
			else if (message[1] == 'q') {
				cancel_thread(socket, ppid);
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
						//printf("!\n");
						sendAsnwerToInvitation(socket, "1", id_emisor);
						id_destino[0] = id_emisor/100;
						id_destino[1] = id_emisor-(id_emisor/100)*100;
					}
					else if (decicion == 0){
						sendAsnwerToInvitation(socket, "0", id_emisor);
					}
				}
				else if (fid == 7){
					char message[1];
					message[0] = 18;
					sendMessage(socket, message);
					if (msg[6] == 1){
						color = 1; //negro
						printf("You are black ♚\n");
						initBoard(blancos, negros);
						printtablero(blancos, negros);
					}
					else if (msg[6] == 0){
						color = 0; //blanco
						printf("You are white ♔\n");
						initBoard(blancos, negros);
						char mov[10];
						printtablero(blancos, negros);
						MOVIN:
						printf("Enter your first move (/q Quit Game)\n");
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
							int repito = 0;
							repito = cambiarpieza(blancos, negros, color, mov[0], mov[1], mov[2], mov[3], mov[4]);
							if (repito == 1){
								printtablero(blancos, negros);
								printf("Invalid Move!\n");
								goto MOVIN;
							}
							printtablero(blancos, negros);
							sendMove(socket, mov[0], mov[1], mov[2], mov[3], mov[4]);
						}
						
					}
				}
				else if (fid == 8){
					if (color == 0){
						cambiarpieza(blancos, negros, 1, msg[2], msg[3], msg[4], msg[5], msg[6]);
					}else{
						cambiarpieza(blancos, negros, 0, msg[2], msg[3], msg[4], msg[5], msg[6]);
					}
					//usleep(200);
					printtablero(blancos, negros);
					printf("The other player move: piece-> %c from-> %c%c  to-> %c%c\n", msg[6], msg[2], msg[3], msg[4], msg[5]);
					if ((blancos[0] == 0 - 48 && blancos[1] == 0 - 96) || (negros[0] == 0 - 48 && negros[1] == 0 - 96)) {
						if ((color == 0 && negros[0] == 0 - 48 && negros[1] == 0 - 96) || (color == 1 && blancos[0] == 0 - 48 && blancos[1] == 0 - 96)){
							printf("You won!\n");
						} else {
							printf("You Loose...\n");
						}
						int fid = 19;
						char message[3];
						message[0] = fid;
						message[1] = 1;
						message[2] = 0;
						sendMessage(socket, message);
						pthread_t thread;
						pthread_create(&thread, NULL, listenChatMessage, &socket);
						printf("/i:id -> Invite Waiting Player ID, /a -> Waiting Players, /w -> Wait Invitation /s -> Server Info /q -> Quit\n");
						goto INICIO;
					}
					MOV:
					printf("Enter your move (/q Quit Game)\n");
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
						int repito = 0;
						repito = cambiarpieza(blancos, negros, color, mov[0], mov[1], mov[2], mov[3], mov[4]);
						if (repito == 1){
							printtablero(blancos, negros);
								printf("Invalid Move!\n");
							goto MOV;
						}
						printtablero(blancos, negros);
						sendMove(socket, mov[0], mov[1], mov[2], mov[3], mov[4]);
						if ((blancos[0] == 0 - 48 && blancos[1] == 0 - 96) || (negros[0] == 0 - 48 && negros[1] == 0 - 96)) {
							if ((color == 0 && negros[0] == 0 - 48 && negros[1] == 0 - 96) || (color == 1 && blancos[0] == 0 - 48 && blancos[1] == 0 - 96)){
								printf("You won!\n");
							} else {
								printf("You Loose...\n");
							}
							int fid = 19;
							char message[3];
							message[0] = fid;
							message[1] = 1;
							message[2] = 0;
							sendMessage(socket, message);
							pthread_t thread;
							pthread_create(&thread, NULL, listenChatMessage, &socket);
							printf("/i:id -> Invite Waiting Player ID, /a -> Waiting Players, /w -> Wait Invitation /s -> Server Info /q -> Quit\n");
							goto INICIO;
						}
					}
				}
				else if (fid == 10){
					printf("The other player has left\n");
					int fid = 10;
					char message[3];
					message[0] = fid;
					message[1] = 1;
					message[2] = 0;
					sendMessage(socket, message);
					char response[3];
					recv(socket, response, 1024, 0);
					pthread_t thread;
					pthread_create(&thread, NULL, listenChatMessage, &socket);
					printf("/i:id -> Invite Waiting Player ID, /a -> Waiting Players, /w -> Wait Invitation /s -> Server Info /q -> Quit\n");
					goto INICIO;
				}

			}
		} else {
			if ( strlen(message) != 0 ) {
				int size_nick = 0;
				for (int i = 0; i < 20; ++i)
				{
					if ( nickname[i] == '\0') {
						break;
					}
					size_nick++;
				}
				int size_msg = 0;
				for (int i = 0; i < 1024; ++i)
				{
					size_msg++;
					if ( message[i] == '\0') {
						break;
					}
					
				}
				char chat_message[size_nick+2+size_msg+4];
				chat_message[0] = 6;
				chat_message[1] = size_nick+2+size_msg+2;
				chat_message[2] = 0;
				chat_message[3] = 0;
				for (int i = 0; i < size_nick; ++i)
				{
					chat_message[i+4] = nickname[i];
				}
				chat_message[size_nick+4] = ':';
				chat_message[size_nick+5] = ' ';
				for (int i = 0; i < size_msg; ++i)
				{
					chat_message[i+4+size_nick+2] = message[i];
				}
				chat_message[size_msg+size_nick+6] = '\0';
				sendMessage(socket, chat_message);
			} 

		}

    }
	return 0;
}
