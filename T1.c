#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fptr;

struct Process
{
	int PID;
	int priority;
	char nombre[257];
	char state[8];
	int start;
	int N;
	int *tiempos;

	int pasos_cpu; // cantidad de veces que entro a running
	int veces_bloqueo; // cantidad de veces de running a waiting
	int turnaround; // tiempo total desde ejecucion hasta que termina
	int response; // 
	int waiting; // todo el tiempo que no esta en running

	struct Process *sgte;
};

struct Queue
{
	struct Process *primerProceso;
	struct Process *ultimoProceso;
};

void agregarProceso(struct Queue *lista){
	struct Process *nuevoProceso = (struct Process *)malloc(sizeof(struct Process));
	nuevoProceso -> sgte = NULL;

	if (lista -> primerProceso == NULL) {

		lista -> primerProceso = nuevoProceso;
		lista -> ultimoProceso = nuevoProceso;

	} else {

		lista -> ultimoProceso -> sgte = nuevoProceso;
		lista -> ultimoProceso = nuevoProceso;

	}

	lista -> ultimoProceso -> sgte = NULL;

}

void eliminarProceso(struct Queue *lista, int PID) {

	struct Process *i = lista -> primerProceso;

	while(i != NULL) {
		struct Process *aux = NULL;

		if (i -> sgte != NULL && i -> sgte -> PID == PID) {
			if (i -> sgte == lista -> ultimoProceso) {
				aux = i -> sgte;
				i -> sgte = NULL;
				lista -> ultimoProceso = i;
			} else {
				aux = i -> sgte;
				i -> sgte = i -> sgte -> sgte;
			}

		}

		if (i -> PID == PID && i == lista -> primerProceso) {
			aux = i;
			lista -> primerProceso = i -> sgte;
			
		}

		i = i -> sgte;
		if (aux != NULL){
			printf("\n-> Se ha eliminado: %i\n", aux -> PID);
			free(aux);
		}
		
	}

}

void imprimirQueue(struct Queue *lista){
	struct Process *i = lista -> primerProceso;

	if(lista -> primerProceso == NULL) {
		printf("No Hay Procesos\n");
	}
	printf("\nImprimiendo Procesos:");
	printf("\n****************************************\n\n");
	while(i != NULL) {
		printf("PID: %i", i -> PID);
	    printf("\nNOMBRE: %s", i -> nombre);
	    printf("\npriority: %i", i -> priority);
	    printf("\nstart: %i", i -> start);
	    int *p_tiempos = i -> tiempos;
	    int N = i -> N;
	    for (int k = 0; k < N*2-1; ++k)
	    {
	    	printf("\nTiempo[%i]:%i", k+1, p_tiempos[k]);
	    }
		i = i -> sgte;
		printf("\n--------------------------\n");
	}
	printf("\n****************************************\n");
}


int main(int argc, char const *argv[])
{
	struct Queue *Running = (struct Queue *)malloc(sizeof(struct Queue));
	struct Queue *Waiting = (struct Queue *)malloc(sizeof(struct Queue)); 
	struct Queue *Ready = (struct Queue *)malloc(sizeof(struct Queue)); 
	struct Queue *Idle = (struct Queue *)malloc(sizeof(struct Queue)); 
	Running -> primerProceso = NULL;
	Waiting -> primerProceso = NULL;
	Ready -> primerProceso = NULL;
	Idle -> primerProceso = NULL;
	Running -> ultimoProceso = NULL;
	Waiting -> ultimoProceso = NULL;
	Ready -> ultimoProceso = NULL;
	Idle -> ultimoProceso = NULL;

	int PID = 0;

	agregarProceso(Idle); // Primer Proceso
	Idle -> ultimoProceso -> PID = PID;
	PID++;
	
	// Inicio Carga de Procesos
	char filename[20] = "text.txt";
	char ch;
	int i;
	int j = 0;
	int k = 0;
	int N;
	int *p_tiempos;


    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("No se puede abrir archivo \n");
        exit(0);
    }
    ch = fgetc(fptr);
    for (i = 0; ch != EOF; i++)
    {

    	char aux[257];

    	if (ch != 32 && ch != '\n'){

    		if (j == 0){
	    		aux[i] = ch;
	    	}

	    	else if (j == 1){
	    		aux[i] = ch;
	    	}

	    	else if (j == 2){
	    		aux[i] = ch;
	    	}

	    	else if (j == 3){
	    		aux[i] = ch;

	    	} 

	    	else {
	    		aux[i] = ch;
	    	}

    	}

    	else {

    		if (j == 0) {
    			strcpy(Idle -> ultimoProceso -> nombre, aux);
    		}
    		else if (j == 1) {
    			int priority = atoi(aux);
    			Idle -> ultimoProceso -> priority = priority;
    		}
    		else if (j == 2) {
    			int start = atoi(aux);
    			Idle -> ultimoProceso -> start = start;
    		}
    		else if (j == 3) {
    			N = atoi(aux);
    			Idle -> ultimoProceso -> N = N;
    			Idle -> ultimoProceso -> tiempos = (int *)malloc(sizeof(int)*2*N-1);
    			p_tiempos = Idle -> ultimoProceso -> tiempos;
    			
    		}
    		else {
    			int t = atoi(aux);
    			p_tiempos[k] = t;
    			k++;
    		}
    		//printf("%s\t", aux);
    		i = -1;
    		memset(aux, 0, sizeof aux);
    		j++;

    		if (ch == '\n'){
	    		agregarProceso(Idle); // Agrega Nuevo Proceso a Idle
	    		Idle -> ultimoProceso -> PID = PID;
	    		PID++;
	    		j = 0;
	    		p_tiempos = NULL;
	    		k = 0;
    		}
    	}

    	if(2*N-1 == k+1) {
    		int t = atoi(aux);
    		p_tiempos[k] = t;
    	}

    	

    	
        ch = fgetc(fptr);
    }
    fclose(fptr);
    // Fin Carga de Procesos



    imprimirQueue(Idle);

    eliminarProceso(Idle,0);

    imprimirQueue(Idle);















	
	return 0;
}

