#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

struct Nodo {
	int fila;
	int columna;

	struct Nodo *sgte;
};

struct Lista {
	struct Nodo *primerNodo;
	struct Nodo *ultimoNodo;
};

void agregarNodo(struct Lista *lista, int i, int j) {
	struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
	nuevoNodo -> sgte = NULL;

	if (lista -> primerNodo == NULL) {

		lista -> primerNodo = nuevoNodo;
		lista -> ultimoNodo = nuevoNodo;

	} else {

		lista -> ultimoNodo -> sgte = nuevoNodo;
		lista -> ultimoNodo = nuevoNodo;

	}

	lista -> ultimoNodo -> fila = i;
	lista -> ultimoNodo -> columna = j;
	lista -> ultimoNodo -> sgte = NULL;
}

void imprimirMatrix(int **matrix, int filas, int columnas) {
	for (int i = 0; i < filas; ++i)
	{
		for (int j = 0; j < columnas; ++j)
		{
			printf("%i ", matrix[i][j]);
		}
		printf("\n");
	}
}

void asignarValores(int **matrix, struct Lista *lista, int n) {
	struct Nodo *i = lista -> primerNodo;
	while (i != NULL) {

		int fila_nodo = i -> fila;
		int columna_nodo = i -> columna;

		matrix[fila_nodo][columna_nodo] = n;

		i = i -> sgte;
	}
}

int** procesarMatrix(int **matrix, int filas, int columnas, int n) {
	if (n == 0){
		return matrix;
	}
	struct Lista *PorVivir = (struct Lista *)malloc(sizeof(struct Lista));
	PorVivir -> primerNodo = NULL;
	PorVivir -> ultimoNodo = NULL;

	struct Lista *PorMorir = (struct Lista *)malloc(sizeof(struct Lista));
	PorMorir -> primerNodo = NULL;
	PorMorir -> ultimoNodo = NULL;
	
	for (int i = 0; i < filas; ++i) // Revisar vecinos
	{
		for (int j = 0; j < columnas; ++j)
		{
			int vecinos_vivos = 0;
			if (i-1 >= 0 && j-1 >= 0){
				if (matrix[i-1][j-1] == 1)
					vecinos_vivos++;
			}
			if (i-1 >= 0){
				if (matrix[i-1][j] == 1)
					vecinos_vivos++;
			}
			if (j-1 >= 0){
				if (matrix[i][j-1] == 1)
					vecinos_vivos++;
			}
			if (i+1 < filas && j+1 < columnas){
				if (matrix[i+1][j+1] == 1)
					vecinos_vivos++;
			}
			if (i+1 < filas){
				if (matrix[i+1][j] == 1)
					vecinos_vivos++;
			}
			if (j+1 < columnas){
				if (matrix[i][j+1] == 1)
					vecinos_vivos++;
			}
			if (i-1 >= 0 && j+1 < columnas){
				if (matrix[i-1][j+1] == 1)
					vecinos_vivos++;
			}
			if (i+1 < filas && j-1 >= 0){
				if (matrix[i+1][j-1] == 1)
					vecinos_vivos++;
			}

			if (vecinos_vivos < 2) { // regla 1 - Toda celda viva con menos de dos vecinos vivos, muere.
				agregarNodo(PorMorir, i, j);
			} 
			else if (vecinos_vivos > 3) { // regla 2 - Toda celda viva con mas de tres vecinos vivos, muere.
				agregarNodo(PorMorir, i, j);
			}
			else if (vecinos_vivos == 3) { // Regla 3 - Toda celda muerta con exactamente tres vecinos vivos, se convierte en una celda viva.
				agregarNodo(PorVivir, i, j);
			}
		}
	}
	asignarValores(matrix, PorVivir, 1);
	asignarValores(matrix, PorMorir, 0);
	return procesarMatrix(matrix, filas, columnas, n - 1);
}

int main(int argc, char const *argv[]) {
	int iteraciones = atoi(argv[1]);
    int filas = atoi(argv[2]);
    int columnas = atoi(argv[3]);
    int vivas = atoi(argv[4]);
    //int threads = atoi(argv[5]);
    int cant_workers = sysconf(_SC_NPROCESSORS_ONLN);
    pid_t pids[cant_workers];
    struct Lista *Init = (struct Lista *)malloc(sizeof(struct Lista));
	Init -> primerNodo = NULL;
	Init -> ultimoNodo = NULL;

	char line[20];
	char *pch;

	for (int i = 0; i < vivas; i++) {
		int n1 = -1, n2 = -1;
		fgets(line, sizeof line, stdin);
		pch = strtok (line," \n");
		while (pch != NULL) {
			if (n1 < 0)
				n1 = atoi(pch) - 1;
			else {
				n2 = atoi(pch) - 1;
				agregarNodo(Init, n1, n2);
			}
			
			pch = strtok (NULL, " \n");
		}
	}
    for (int i = 0; i < cant_workers; ++i) {
    	pids[i] = fork();
    	if (pids[i] == 0) {
    		// init matrix
		    int **matrix = malloc(sizeof *matrix * filas);
			if (matrix) {
			  for (int i = 0; i < filas; i++) {
			    matrix[i] = malloc(sizeof *matrix[i] * columnas);
			  }
			}
			
			for (int i = 0; i < filas; ++i) {
				memset(matrix[i], 0, sizeof matrix[i]*columnas);
			}

			asignarValores(matrix, Init, 1);
			

			procesarMatrix(matrix, filas, columnas, iteraciones);
			imprimirMatrix(matrix, filas, columnas);

			exit(0);

    	} else {
    		wait(NULL);

		    for (int i = 0; i < cant_workers; ++i) {
		    	kill( pids[i], SIGINT ); // kill a todos los procesos creados
		    }
    	}	
    }

	return 0;
}