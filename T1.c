#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

FILE *fptr;
int finalizar = 0;

struct Process
{
	int PID;
	float priority;
	char nombre[257];
	char state[8];
	int start;
	int N;
	int *tiempos;
    int quantum;
    int pasos_robin;

	int pasos_cpu; // cantidad de veces que entro a running
	int veces_bloqueo; // cantidad de veces de running a waiting
	int turnaround; // tiempo total desde ejecucion hasta que termina
	int response; // 
	int waiting; // todo el tiempo que no esta en running

	struct Process *sgte;
};

struct Queue
{
    char nombre[9];
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
    nuevoProceso -> quantum = procesoOriginal -> quantum; 
    nuevoProceso -> pasos_robin = procesoOriginal -> pasos_robin;  

    nuevoProceso -> sgte = NULL;

    if (lista -> primerProceso == NULL) {

        lista -> primerProceso = nuevoProceso;
        lista -> ultimoProceso = nuevoProceso;

    } else {

        lista -> ultimoProceso -> sgte = nuevoProceso;
        lista -> ultimoProceso = nuevoProceso;

    }

    lista -> ultimoProceso -> sgte = NULL;

    //printf("\n-> Se ha agregado %i a la lista %s\n", nuevoProceso -> PID, lista -> nombre);

}

void eliminarProceso(struct Queue *lista, int PID) {

	struct Process *i = lista -> primerProceso;
	while(i != NULL) {
        //printf("%i vs %i\n", i -> PID, PID);
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
            //el free me tira error
			//printf("\n-> Se ha eliminado %i de la lista %s\n", aux -> PID, lista -> nombre);
			//free(aux);
            //printf("fdsfsdagaaaaaaaaaaaaa\n");

		}
		
	}

}

void cambiarProceso(int PID, struct Queue *lista1, struct Queue *lista2){
    struct Process *i = lista1 -> primerProceso;

    while(i != NULL) {

        if (i -> PID == PID) {
            agregarProceso2(i, lista2);
            eliminarProceso(lista1, PID);//////problemas con eliminar proceso
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
	    printf("\npriority: %.2f", i -> priority);
	    printf("\nstart: %i", i -> start);
	    int *p_tiempos = i -> tiempos;
	    int N = i -> N;
	    for (int k = 0; k < N*2-1; ++k)
	    {
	    	printf("\nTiempo[%i]:%i", k, p_tiempos[k]);
	    }
		i = i -> sgte;
		printf("\n--------------------------\n");
	}
	printf("\n****************************************\n");
}

int length(struct Queue *lista){

  struct Process *i = lista -> primerProceso;
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
    struct Process *i = lista -> primerProceso;
    int id = -1;
    int *p_tiempos = i -> tiempos;
    int pasos = i -> pasos_cpu;
    if (i != NULL){
        id = i -> PID;
        if (p_tiempos[2*pasos - 2] != 0){
            return -1;
        }
    }
    return id;
}

int revisarReadyFCFS(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    int id = -1;
    if (i != NULL){
        id = i -> PID;
    }
    return id;
}

int revisarReadyPriority(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    int id = -1;
    int mayorPriority = -1;
    while (i != NULL) {
        if (i -> priority > mayorPriority) {
            mayorPriority = i -> priority;
        }
        i = i -> sgte; 
    }

    i = lista -> primerProceso;
    while (i != NULL) {
        if (i -> priority == mayorPriority) {
            return i -> PID;
        }
        i = i -> sgte;
    } 
    return id;
}

int revisarRunningRobin(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    int id = -1;
    int *p_tiempos = i -> tiempos;
    int pasos = i -> pasos_robin;
    if (i != NULL){
        id = i -> PID;
        if (p_tiempos[2*pasos - 2] == 0){
            i -> pasos_robin ++;
            i -> quantum = -1;
            return id;
        }
        if (i -> quantum == 0){
            return id;
        }
    }
    return -1;
}

int revisarWaitingRobin(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    int id = -1;
    while(i != NULL && id == -1){
        int *p_tiempos = i -> tiempos;
        int pasos = i -> pasos_robin;
        if (p_tiempos[2*pasos - 1] == 0){
            id = i -> PID;
            return id;
        }
        i = i -> sgte;
    }
    return id;
}

void agregarInfoRunning(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        i -> turnaround ++;
        int *p_tiempos = i -> tiempos;
        int pasos = i -> pasos_cpu;
        //if (i -> PID == 0){
        //    printf("tiempo restante en la cpu %i\n", p_tiempos[2*pasos -2]);
        //}
        p_tiempos[2*pasos - 2] --;
        i = i -> sgte;
    }
    
}

void agregarInfoReady(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        
        i -> turnaround ++;
        i -> waiting ++;
        i = i -> sgte;
    }
}

void agregarInfoWaiting(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        i -> turnaround ++;
        i -> waiting ++;
        i -> response ++;
        int *p_tiempos = i -> tiempos;
        int pasos = i -> pasos_cpu;
        //if (i -> PID == 0){
        //    printf("tiempo restante en la waiting%i\n", p_tiempos[2*pasos -1]);        
        //}
        p_tiempos[2*pasos - 1] --;
        i = i -> sgte;
    }
}

void agregarInfoRunningRobin(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        i -> turnaround ++;
        int *p_tiempos = i -> tiempos;
        int pasos = i -> pasos_robin;
        p_tiempos[2*pasos - 2] --;
        i -> quantum --;
        i = i -> sgte;
    }
}

void agregarInfoReadyRobin(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        i -> turnaround ++;
        i -> waiting ++;
        i = i -> sgte;
    }
}

void agregarInfoWaitingRobin(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        i -> turnaround ++;
        i -> waiting ++;
        i -> response ++;
        int *p_tiempos = i -> tiempos;
        int pasos = i -> pasos_robin;
        p_tiempos[2*pasos - 1] --;
        i = i -> sgte;
    }
}

void agregarInfoIdle(struct Queue *lista){
    struct Process *i = lista -> primerProceso;

    while (i != NULL){
        i -> waiting ++;
        i = i -> sgte;
    }
}

void handler(){
    printf("Simulacion cancelada por usuario\n");
    finalizar = 1;
}

void setearIndicadores(struct Queue *lista){
    struct Process *i = lista -> primerProceso;
    while(i != NULL){
        i -> pasos_cpu = 0; 
        i -> veces_bloqueo = 0; 
        i -> turnaround = 0; 
        i -> response = 0; 
        i -> waiting = 0; 
        i -> quantum = 0; 
        i -> pasos_robin = 1;
        i = i -> sgte;
    }
}

void imprimir(struct Queue *Running, struct Queue *Waiting, struct Queue *Ready, struct Queue *Idle, struct Queue *Finished){
    struct Process *i = Running -> primerProceso;
    struct Process *j = Ready -> primerProceso;
    struct Process *k = Waiting -> primerProceso;
    struct Process *l = Idle -> primerProceso;
    struct Process *m = Finished -> primerProceso;
    char a[500] = "Running:  ";
    char b[500] = "Ready:    ";
    char c[500] = "Waiting:  ";
    char d[500] = "Idle:     ";
    char e[500] = "Finished: ";
    while(i != NULL){
        char buffer[5];
        sprintf(buffer, "%d", i -> PID);
        strcat(a, " -> ");
        strcat(a, buffer);
        i = i -> sgte;
    }
    while(j != NULL){
        char buffer[5];
        sprintf(buffer, "%d", j -> PID);
        strcat(b, " -> ");
        strcat(b, buffer);
        j = j -> sgte;
    }
    while(k != NULL){
        char buffer[5];
        sprintf(buffer, "%d", k -> PID);
        strcat(c, " -> ");
        strcat(c, buffer);
        k = k -> sgte;
    }
    while(l != NULL){
        char buffer[5];
        sprintf(buffer, "%d", l -> PID);
        strcat(d, " -> ");
        strcat(d, buffer);
        l = l -> sgte;
    }
    while(m != NULL){
        char buffer[5];
        sprintf(buffer, "%d", m -> PID);
        strcat(e, " -> ");
        strcat(e, buffer);
        m = m -> sgte;
    }
    //printf("\n\n\n\n");
    printf("%s\n", a);
    printf("%s\n", b);
    printf("%s\n", c);
    printf("%s\n", d);
    printf("%s\n", e);
}

void agregarQuantum(struct Queue *lista, int quantum){
    struct Process *i = lista -> primerProceso;
    while(i != NULL){
        float p = i -> priority; // prioridad texto
        float P = 0.0;
        float q = quantum;
        float Q = 0.0;

        P = p*q + pow(-1, roundf(p/q))*p;
        Q = floorf(p/64) + 1;
        i -> quantum = Q;
        i = i -> sgte;
    }
}
void terminate(struct Queue *Running, struct Queue *Waiting, struct Queue *Ready, struct Queue *Idle, struct Queue *Finished, int tipo){
    struct Process *i = Running -> primerProceso;
    struct Process *j = Ready -> primerProceso;
    struct Process *k = Waiting -> primerProceso;
    struct Process *l = Idle -> primerProceso;
    struct Process *m = Finished -> primerProceso;
    printf("\n \nHa terminado la simulacion \n \n");
    while(i != NULL){
        printf("Proceso: %s\n", i -> nombre);
        if (tipo == 1){
            if (i -> N == i -> pasos_robin){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        else{
            if (i -> N == i -> pasos_cpu){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        printf("Elegido para CPU: %i veces\n", i -> pasos_cpu);
        printf("Proceso bloqueado: %i veces\n", i -> veces_bloqueo);
        printf("Turnarround Time: %i\n", i -> turnaround);
        printf("Responce Time: %i\n", i -> response);
        printf("Waiting Time: %i\n", i -> waiting);
        printf("\n\n");
        i = i -> sgte;
    }

    while(j != NULL){
        printf("Proceso: %s\n", j -> nombre);
        if (tipo == 1){
            if (j -> N == j -> pasos_robin){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        else{
            if (j -> N == j -> pasos_cpu){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        printf("Elegido para CPU: %i veces\n", j -> pasos_cpu);
        printf("Proceso bloqueado: %i veces\n", j -> veces_bloqueo);
        printf("Turnarround Time: %i\n", j -> turnaround);
        printf("Responce Time: %i\n", j -> response);
        printf("Waiting Time: %i\n", j -> waiting);
        printf("\n\n");
        j = j -> sgte;
    }

    while(k != NULL){
        printf("Proceso: %s\n", k -> nombre);
        if (tipo == 1){
            if (k -> N == k -> pasos_robin){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        else{
            if (k -> N == k -> pasos_cpu){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        printf("Elegido para CPU: %i veces\n", k -> pasos_cpu);
        printf("Proceso bloqueado: %i veces\n", k -> veces_bloqueo);
        printf("Turnarround Time: %i\n", k -> turnaround);
        printf("Responce Time: %i\n", k -> response);
        printf("Waiting Time: %i\n", k -> waiting);
        printf("\n\n");
        k = k -> sgte;
    }

    while(l != NULL){
        printf("Proceso: %s\n", l -> nombre);
        if (tipo == 1){
            if (l -> N == l -> pasos_robin){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        else{
            if (l -> N == l -> pasos_cpu){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        printf("Elegido para CPU: %i veces\n", l -> pasos_cpu);
        printf("Proceso bloqueado: %i veces\n", l -> veces_bloqueo);
        printf("Turnarround Time: %i\n", l -> turnaround);
        printf("Responce Time: %i\n", l -> response);
        printf("Waiting Time: %i\n", l -> waiting);
        printf("\n\n");
        l = l -> sgte;
    }

    while(m  != NULL){
        printf("Proceso: %s\n", m -> nombre);
        if (tipo == 1){
            if (m -> N == m -> pasos_robin){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        else{
            if (m -> N == m -> pasos_cpu){
                printf("Estado: Finalizado\n");
            }
            else{
                printf("Estado: En ejecucion\n");
            }
        }
        printf("Elegido para CPU: %i  veces\n", m  -> pasos_cpu);
        printf("Proceso bloqueado: %i  veces\n", m  -> veces_bloqueo);
        printf("Turnarround Time: %i\n", m  -> turnaround);
        printf("Responce Time: %i\n", m  -> response);
        printf("Waiting Time: %i\n", m  -> waiting);
        printf("\n\n");
        m = m -> sgte;
    }
    
    exit(0);
}
int main(int argc, char *argv[])
{
    signal(SIGINT, handler);
	struct Queue *Running = (struct Queue *)malloc(sizeof(struct Queue));
	struct Queue *Waiting = (struct Queue *)malloc(sizeof(struct Queue)); 
	struct Queue *Ready = (struct Queue *)malloc(sizeof(struct Queue)); 
	struct Queue *Idle = (struct Queue *)malloc(sizeof(struct Queue));
    struct Queue *Finished = (struct Queue *)malloc(sizeof(struct Queue));
    strcpy(Running -> nombre, "Running");
    strcpy(Waiting -> nombre, "Waiting");
    strcpy(Ready -> nombre, "Ready"); 
    strcpy(Idle -> nombre, "Idle");
    strcpy(Finished -> nombre, "Finished");
	Running -> primerProceso = NULL;
	Waiting -> primerProceso = NULL;
	Ready -> primerProceso = NULL;
	Idle -> primerProceso = NULL;
    Finished -> primerProceso = NULL;
	Running -> ultimoProceso = NULL;
	Waiting -> ultimoProceso = NULL;
	Ready -> ultimoProceso = NULL;
	Idle -> ultimoProceso = NULL;
    Finished -> ultimoProceso = NULL;

	int PID = 0;

	agregarProceso(Idle); // Primer Proceso
	Idle -> ultimoProceso -> PID = PID;
	PID++;

	
	// Inicio Carga de Procesos
    char filename[20];
    char scheduler[20];
    float quantum;
    strcpy(filename, "text.txt");
    strcpy(scheduler, "roundrobin");
    quantum = 15.0;

    // if (argc == 1)
    // {
    //     printf("2 - 3 parametros requeridos, 0 recividos \n");
    //     exit(0);
    // }
    // else if (argc == 2)
    // {
    //     printf("2 - 3 parametros requeridos, 1 recividos \n");
    //     exit(0);
    // }
    // else if (argc == 3)
    // {
    //     strcpy(scheduler, argv[1]);
    //     strcpy(filename, argv[2]);
    //     if (strcmp(filename, "roundrobin") == 0)
    //     {
    //         quantum = 3;
    //     }
    // }
    // else if (argc == 4)
    // {
    //     strcpy(scheduler, argv[1]);
    //     strcpy(filename, argv[2]);
    //     quantum = argv[3] - "0";
    //     if (strcmp(filename, "roundrobin") != 0)
    //     {
    //         printf("un quantum solo es recivido en caso de un scheduler roundrobin \n");
    //         exit(0);
    //     }
    // }
    // else
    // {
    //     printf("%i parametros entregados, 2 - 3 requeridos\n", argc);
    //     exit(0);
    // }
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

	    	aux[i] = ch;

    	}

    	else {

    		if (j == 0) {
    			strcpy(Idle -> ultimoProceso -> nombre, aux);
    		}
    		else if (j == 1) {
    			int priority = atof(aux);
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
    setearIndicadores(Idle);
    // Fin Carga de Procesos
    int clock = 0;
    int CPU_libre = 0; // 0 -> libre  1 -> ocupada
    //imprimirQueue(Idle);

    if (strcmp(scheduler, "roundrobin") == 0){
        agregarQuantum(Idle, quantum);    
    }
    while(Running -> primerProceso != NULL || Ready -> primerProceso != NULL || Waiting -> primerProceso != NULL || Idle -> primerProceso != NULL){
        ///////////// en caso de que apretern crt C///////////////
        if (finalizar == 1){
            if (strcmp(scheduler, "roundrobin") == 0){
                terminate(Running, Waiting, Ready, Idle, Finished, 1);
            }
            else{
                terminate(Running, Waiting, Ready, Idle, Finished, 0);
            }
        }
        usleep(500000);
        ///////////////////////////////////
        int proceso_a_iniciar = empezarProceso(Idle, clock);
        printf("\n\n\nclock: %i\n", clock);
        imprimir(Running, Waiting, Ready, Idle, Finished);
        if (proceso_a_iniciar != -1)
        {   
            //printf("el proceso a iniciar es:%i\n", proceso_a_iniciar);
            cambiarProceso(proceso_a_iniciar, Idle, Ready);
        }
        int sacar_de_waiting = -1;
        int sacar_de_ready = -1;
        int sacar_de_running = -1;
        if (strcmp(scheduler, "fcfs") == 0){
            if(CPU_libre == 1){
                sacar_de_running = revisarRunning(Running); //el proceso de rrunning ya termino??
            }
            if (sacar_de_running != -1){ // el proceso sigue ocupando la cpu
                int a = Running -> primerProceso -> N;
                int b = Running -> primerProceso -> pasos_cpu;
                if (a == b){
                    //printf("el proceso PID: %i, ha finalizado\n", sacar_de_running);
                    cambiarProceso(sacar_de_running, Running, Finished);
                    CPU_libre = 0;
                }
                else{
                    Running -> primerProceso -> veces_bloqueo ++;
                    cambiarProceso(sacar_de_running,Running,Waiting);
                    //printf("el proceso %i ha salido de Running y paso a Waiting\n", sacar_de_running);
                    //imprimir(Running, Waiting, Ready, Idle, Finished);
                    CPU_libre = 0;
                }
            }

            // posiblemnet hacer do while
            SACOWAIT:
            sacar_de_waiting = revisarWaiting(Waiting);
            if (sacar_de_waiting != -1){
                cambiarProceso(sacar_de_waiting, Waiting, Ready);
                //printf("el proceso %i ha salido de Waiting y ha pasado a Ready\n", sacar_de_waiting);
                //imprimir(Running, Waiting, Ready, Idle, Finished);
                goto SACOWAIT;
            }

            if(CPU_libre == 0){
                sacar_de_ready = revisarReadyFCFS(Ready);
                if (sacar_de_ready != -1){
                    cambiarProceso(sacar_de_ready,Ready,Running);
                    //printf("el proceso %i ha salido de Ready y Paso a Running\n", sacar_de_ready);
                    //imprimir(Running, Waiting, Ready, Idle, Finished);
                    CPU_libre = 1;
                    Running -> primerProceso -> pasos_cpu ++;
                }
            }

            agregarInfoRunning(Running);
            agregarInfoReady(Ready);
            agregarInfoWaiting(Waiting);
            agregarInfoIdle(Idle);
        } 
        else if (strcmp(scheduler, "priority") == 0){
            if(CPU_libre == 1){
                sacar_de_running = revisarRunning(Running); //el proceso de rrunning ya termino??
            }
            if (sacar_de_running != -1){ // el proceso sigue ocupando la cpu
                int a = Running -> primerProceso -> N;
                int b = Running -> primerProceso -> pasos_cpu;
                if (a == b){
                    //printf("el proceso PID: %i, ha finalizado\n", sacar_de_running);
                    cambiarProceso(sacar_de_running, Running, Finished);
                    CPU_libre = 0;
                }
                else{
                    Running -> primerProceso -> veces_bloqueo ++;
                    cambiarProceso(sacar_de_running,Running,Waiting);
                    //printf("el proceso %i ha salido de Running y paso a Waiting\n", sacar_de_running);
                    //imprimir(Running, Waiting, Ready, Idle, Finished);
                    CPU_libre = 0;
                }
            }

            // posiblemnet hacer do while
            SACOWAIT2:
            sacar_de_waiting = revisarWaiting(Waiting);
            if (sacar_de_waiting != -1){
                cambiarProceso(sacar_de_waiting, Waiting, Ready);
                //printf("el proceso %i ha salido de Waiting y ha pasado a Ready\n", sacar_de_waiting);
                //imprimir(Running, Waiting, Ready, Idle, Finished);
                goto SACOWAIT2;
            }

            if(CPU_libre == 0){
                sacar_de_ready = revisarReadyPriority(Ready);
                if (sacar_de_ready != -1){
                    cambiarProceso(sacar_de_ready,Ready,Running);
                    //printf("el proceso %i ha salido de Ready y Paso a Running\n", sacar_de_ready);
                    //imprimir(Running, Waiting, Ready, Idle, Finished);
                    CPU_libre = 1;
                    Running -> primerProceso -> pasos_cpu ++;
                }
            }
            agregarInfoRunning(Running);
            agregarInfoReady(Ready);
            agregarInfoWaiting(Waiting);
            agregarInfoIdle(Idle);
        }
        else if (strcmp(scheduler, "roundrobin" ) == 0){
            if(CPU_libre == 1){
                sacar_de_running = revisarRunningRobin(Running); //el proceso de rrunning ya termino??
            }
            if (sacar_de_running != -1){ // el proceso sigue ocupando la cpu
                if (Running -> primerProceso -> quantum == 0){
                    Running -> primerProceso -> veces_bloqueo ++;
                    cambiarProceso(sacar_de_running, Running, Ready);
                    //printf("el proceso %i ha salido de Running y paso a Ready\n", sacar_de_running);
                    //imprimir(Running, Waiting, Ready, Idle, Finished);
                    CPU_libre = 0;
                }
                else{
                    int a = Running -> primerProceso -> N;
                    int b = Running -> primerProceso -> pasos_robin;
                    if (a == b){
                        //printf("el proceso PID: %i, ha finalizado\n", sacar_de_running);
                        cambiarProceso(sacar_de_running, Running, Finished);
                        CPU_libre = 0;
                    }
                    else{
                        Running -> primerProceso -> veces_bloqueo ++;
                        cambiarProceso(sacar_de_running,Running,Waiting);
                        //printf("el proceso %i ha salido de Running y paso a waiting\n", sacar_de_running);
                        //imprimir(Running, Waiting, Ready, Idle, Finished);
                        CPU_libre = 0;
                    }
                }
            }
            // posiblemnet hacer do while
            SACOWAIT3:
            sacar_de_waiting = revisarWaitingRobin(Waiting);
            if (sacar_de_waiting != -1){
                cambiarProceso(sacar_de_waiting, Waiting, Ready);
                //printf("el proceso %i ha salido de Waiting y ha pasado a Ready\n", sacar_de_waiting);
                //imprimir(Running, Waiting, Ready, Idle, Finished);
                goto SACOWAIT3;
            }

            if(CPU_libre == 0){
                sacar_de_ready = revisarReadyFCFS(Ready);
                if (sacar_de_ready != -1){
                    cambiarProceso(sacar_de_ready,Ready,Running);
                    //printf("el proceso %i ha salido de Ready y Paso a Running\n", sacar_de_ready);
                    //imprimir(Running, Waiting, Ready, Idle, Finished);
                    CPU_libre = 1;
                    Running -> primerProceso -> pasos_cpu ++;
                    agregarQuantum(Running, quantum);
                }
            }

            agregarInfoRunningRobin(Running);
            agregarInfoReadyRobin(Ready);
            agregarInfoWaitingRobin(Waiting);
            agregarInfoIdle(Idle);
        }

        ++clock;
    }
    if (strcmp(scheduler, "roundrobin") == 0){
        terminate(Running, Waiting, Ready, Idle, Finished, 1);
    }
    else
    {
        terminate(Running, Waiting, Ready, Idle, Finished, 0);   
    }
	return 0;
}