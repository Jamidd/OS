#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
FILE *fptr;
int finalizar = 0;

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
    char nombre[8];
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

void agregarProceso2(struct Process *procesoOriginal, struct Queue *lista){
    struct Process *nuevoProceso = (struct Process *)malloc(sizeof(struct Process));

    nuevoProceso -> PID = procesoOriginal -> PID;
    nuevoProceso -> priority = procesoOriginal -> priority;
    strcpy(nuevoProceso -> nombre, procesoOriginal -> nombre);
    strcpy(nuevoProceso -> state, procesoOriginal -> state);
    nuevoProceso -> start = procesoOriginal -> start;
    nuevoProceso -> N = procesoOriginal -> N;
    nuevoProceso -> tiempos = procesoOriginal -> tiempos;
    nuevoProceso -> pasos_cpu = procesoOriginal -> pasos_cpu;
    nuevoProceso -> veces_bloqueo = procesoOriginal -> veces_bloqueo;
    nuevoProceso -> turnaround = procesoOriginal -> turnaround;
    nuevoProceso -> response = procesoOriginal -> response;
    nuevoProceso -> waiting = procesoOriginal -> waiting;   

    nuevoProceso -> sgte = NULL;

    if (lista -> primerProceso == NULL) {

        lista -> primerProceso = nuevoProceso;
        lista -> ultimoProceso = nuevoProceso;

    } else {

        lista -> ultimoProceso -> sgte = nuevoProceso;
        lista -> ultimoProceso = nuevoProceso;

    }

    lista -> ultimoProceso -> sgte = NULL;

    printf("\n-> Se ha agregado %i a la lista %s\n", nuevoProceso -> PID, lista -> nombre);

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
			printf("\n-> Se ha eliminado %i de la lista %s\n", aux -> PID, lista -> nombre);
			free(aux);
		}
		
	}

}

void cambiarProceso(int PID, struct Queue *lista1, struct Queue *lista2){
    struct Process *i = lista1 -> primerProceso;

    while(i != NULL) {

        if (i -> PID == PID) {
            agregarProceso2(i, lista2);
            eliminarProceso(lista1, PID);
            return;
        }
        i = i -> sgte;
    }

}

void imprimirQueue(struct Queue *lista){
	struct Process *i = lista -> primerProceso;

	if(lista -> primerProceso == NULL) {
		printf("No Hay Procesos\n");
	}
	printf("\nImprimiendo Procesos de %s:", lista -> nombre);
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

int length(struct Process *lista){
  struct Process *i = lista;
  int lenght = 0;

  while (i != NULL)
  {
    ++lenght;
    i = i -> sgte;
  }

  return lenght;
}

int empezarProceso(struct Queue *lista, int clock)
{
    
    struct Process *i = lista -> primerProceso;
    int id = -1;
    while(i != NULL && id == -1){
        if (i -> start == clock)
        {
            id = i -> PID;
        }
        i = i -> sgte;
    }
    return id;
}

int sacarPrimero(struct Queue *lista)
{
    struct Process *i = lista -> primerProceso;
    int id = -1;
    if (i != NULL && id == -1)
    {
        id = i -> PID;
    }
    return id;
    return 0;
}

int priority(struct Queue *lista)
{
    struct Process *i = lista -> primerProceso;
    int id = -1;
    int prio = -1;

    while(i != NULL && id == -1){
        if (i -> priority > prio){
            id = i -> PID;
            prio = i -> priority;
        }
        i = i -> sgte;
    }
    return id;
}

int roundrobin(struct Queue *lista, int quantum){

    return 0;
}

int revisarWaiting(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    int id = -1;
    while(i != NULL && id == -1){
        int *p_tiempos = i -> tiempos;
        int pasos = i -> pasos_cpu;
        if (p_tiempos[2*pasos - 1] == 0){
            id = i -> PID;
            return id;
        }
        i = i -> sgte;
    }
    return id;
}

int revisarRunning(struct Queue *lista){
    struct Process *i = lista ->primerProceso;
    int id = -1;
    int *p_tiempos = i -> tiempos;
    int pasos = i -> pasos_cpu;
    if (i != NULL && id == -1){
        id = i -> PID;
    }
    if (id != -1){
        if (p_tiempos[2*pasos - 2] != 0){
            return -1;
        }
    }
    return id;
}

int revisarReady(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    int id = -1;
    if (i != NULL && id == -1){
        id = i -> PID;
    }
    return id;
}

void handler(){
    finalizar = 1;
}

void termiante(){

    ///ya veremos//
    exit(0);
}
int main(int argc, char *argv[])
{
    signal(SIGINT, handler);
	struct Queue *Running = (struct Queue *)malloc(sizeof(struct Queue));
	struct Queue *Waiting = (struct Queue *)malloc(sizeof(struct Queue)); 
	struct Queue *Ready = (struct Queue *)malloc(sizeof(struct Queue)); 
	struct Queue *Idle = (struct Queue *)malloc(sizeof(struct Queue));
    strcpy(Running -> nombre, "Running"); 
    strcpy(Waiting -> nombre, "Waiting"); 
    strcpy(Ready -> nombre, "Ready"); 
    strcpy(Idle -> nombre, "Idle"); 
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
    char filename[20];
    char scheduler[20];
    int quantum;
    if (argc == 1)
    {
        printf("2 - 3 parametros requeridos, 0 recividos \n");
        exit(0);
    }
    else if (argc == 2)
    {
        printf("2 - 3 parametros requeridos, 1 recividos \n");
        exit(0);
    }
    else if (argc == 3)
    {
        strcpy(scheduler, argv[1]);
        strcpy(filename, argv[2]);
        if (strcmp(filename, "roundrobin") == 0)
        {
            quantum = 3;
        }
    }
    else if (argc == 4)
    {
        strcpy(scheduler, argv[1]);
        strcpy(filename, argv[2]);
        quantum = argv[3] - "0";
        if (strcmp(filename, "roundrobin") != 0)
        {
            printf("un quantum solo es recivido en caso de un scheduler roundrobin \n");
            exit(0);
        }
    }
    else
    {
        printf("%i parametros entregados, 2 - 3 requeridos\n", argc);
        exit(0);
    }
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
 
    int clock = 0;
    int CPU_libre = 0; // 0 -> libre  1 -> ocupada
    while(Running -> primerProceso != NULL || Ready -> primerProceso != NULL || Waiting -> primerProceso != NULL || Idle -> primerProceso != NULL){
        ///////////// en caso de que apretern crt C///////////////
        if (finalizar == 1){
            termiante();
        }
        ///////////////////////////////////
        int proceso_a_iniciar = empezarProceso(Idle, clock);
        if (proceso_a_iniciar != -1)
        {   
            printf("el proceso a iniciar es:%i\n", proceso_a_iniciar);
            cambiarProceso(proceso_a_iniciar, Idle, Ready);
        }

        int sacar_de_waiting = -1;
        int sacar_de_ready = -1;
        int sacar_de_running = -1;
        if (strcmp(scheduler, "fcfs") == 0){
            sacar_de_running = revisarRunning(Running); //el proceso de rrunning ya termino??
            if (sacar_de_running != -1){ // el proceso sigue ocupando la cpu
                cambiarProceso(sacar_de_running,Running,Waiting);
                CPU_libre = 0;
            }

            SACOWAIT:
            sacar_de_waiting = revisarWaiting(Waiting);
            if (sacar_de_waiting != -1){
                cambiarProceso(sacar_de_waiting, Waiting, Ready);
                goto SACOWAIT;
            }

            if(CPU_libre == 0){
                sacar_de_ready = revisarReady(Ready);
                if (sacar_de_ready != -1){
                    cambiarProceso(sacar_de_ready,Ready,Running);
                    Running -> primerProceso -> pasos_cpu ++;
                }
            }
        } 










































        else if (strcmp(scheduler, "priority") == 0){
            int proceso_a_cpu = priority(Ready);
            if (proceso_a_cpu != -1)
            {
                printf("dsa\n");
            }
        }
        else if (strcmp(scheduler, "roundrobin" ) == 0){
            int proceso_a_cpu = sacarPrimero(Ready);
            if (proceso_a_cpu != -1)
            {
                printf("dsa\n");
            }
        }

        FINAL:
        agregarInfoRunning(Running);
        agregarInfoReady(Ready);
        agregarInfoWaiting(Waiting);
        agregarInfoIdle(Idle);

        ++clock;
    }

	return 0;
}