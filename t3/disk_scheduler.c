#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
struct nodo
{
    int id;
    int value;
    struct nodo *sgte;
};

struct Queue
{
    struct nodo *firstNode;
    struct nodo *lastNode;
};

void add_nodo(struct Queue *list){
    struct nodo *new = (struct nodo *)malloc(sizeof(struct nodo));
    new -> sgte = NULL;
    if (list -> firstNode == NULL) {
        list -> firstNode = new;
        list -> lastNode = new;
    } else {
        list -> lastNode -> sgte = new;
        list -> lastNode = new;
    }
    list -> lastNode -> sgte = NULL;
};

void remove_nodo(struct Queue *list, int id){
    struct nodo *i = list -> firstNode;
    while(i != NULL) {
        struct nodo *aux = NULL;
        if (i -> sgte != NULL && i -> sgte -> id == id) {
            if (i -> sgte == list -> lastNode) {
                aux = i -> sgte;
                i -> sgte = NULL;
                list -> lastNode = i;
            } else {
                aux = i -> sgte;
                i -> sgte = i -> sgte -> sgte;
            }
        }
        if (i -> id == id && i == list -> firstNode) {
            aux = i;
            list -> firstNode = i -> sgte;
        }
        i = i -> sgte;
        if (aux != NULL){
            free(aux);
        }
    }
}



void fcfs (struct Queue *request){
    struct nodo *node = request -> firstNode;
    char full[500];   
    char aux[20];
    int path_len = 0;
    int changes = 0;
    int prev = request -> firstNode -> value;
    int bef_prev = prev;


    if (node -> value > node -> sgte -> value){
        changes ++;
    }
    path_len += abs(node -> value - prev);
    node = node -> sgte;
    while (node != NULL){
        sprintf(aux, "%d", node -> value);
        strcat(full, aux);
        path_len += abs(node -> value - prev);
        //printf("%i - %i = %i\n", node -> value, prev, node -> value - prev);
        if (bef_prev < prev && prev > node -> value){
            changes ++;
        }
        if (bef_prev > prev && prev < node -> value){
            changes ++;
        }
        bef_prev = prev;
        prev = node -> value;
        node = node -> sgte;
        if (node != NULL){
            strcat(full, ",");
        }   
    }

    printf("%s\n", full);
    printf("%i\n", path_len);
    printf("%iT+%iD msec\n",path_len, changes);
}
void sstf (struct Queue *request){
    struct nodo *node;
    char full[500];   
    char aux[20];
    int path_len = 0;
    int changes = 0;
    int prev = -1;

    node = request -> firstNode;
    int val_act = node -> value;
    int id_actt = node -> id;
    node = node -> sgte;
    while (request -> firstNode != NULL){
        int comp = 256;
        int val_comp = node -> value;
        int id_comp = node -> id;

        while (node != NULL){
            if (node -> id != id_actt){
                if (abs(node -> value - val_act) <= comp){
                    comp = abs(node -> value - val_act);
                    id_comp = node -> id;
                    val_comp = node -> value;
                }
            }
            node = node -> sgte;
        }   
        if (id_actt != id_comp){
            //printf("valor final: %i\n", val_comp);
            //printf("id final: %i\n", id_comp);
            sprintf(aux, "%d", val_comp);
            strcat(full, aux);
            if (request -> firstNode -> sgte -> sgte != NULL){
                strcat(full, ",");
            }
            path_len += comp;
            if (prev == -1 && val_comp - val_act < 0){
                changes ++;
            }
            if (prev < val_act && val_act > val_comp){
                changes ++;
            }
            if (prev > val_act && val_act < val_comp){
                changes ++;
            }
            remove_nodo(request, id_actt);
            id_actt = id_comp;
            prev = val_act;
            val_act = val_comp;
            node = request -> firstNode;
        }
        else if (id_actt == id_comp){
            remove_nodo(request, id_actt);
        }
        
    }

    printf("%s\n", full);
    printf("%i\n", path_len);
    printf("%iT+%iD msec\n",path_len, changes);
}

void scan (struct Queue *request){
    struct nodo *node;
    char full[500];   
    char aux[20];
    int path_len = 0;
    int changes = 0;
    int prev = -1;
    int off = 0;

    node = request -> firstNode;
    int val_act = node -> value;
    int id_actt = node -> id;
    node = node -> sgte;
    while (request -> firstNode != NULL){
        int comp = 256;
        int val_comp = node -> value;
        int id_comp = node -> id;

        RET:
        while (node != NULL){
            if (node -> id != id_actt){

                if (abs(node -> value - val_act) < comp){
                    if (off == 0 && val_act <= node -> value){
                        comp = abs(node -> value - val_act);
                        id_comp = node -> id;
                        val_comp = node -> value;
                    }
                    else if (off ==1 && val_act >= node -> value){
                        comp = abs(node -> value - val_act);
                        id_comp = node -> id;
                        val_comp = node -> value;
                    }
                }
            }
            node = node -> sgte;
        }   
        if (id_actt != id_comp){
            if (val_comp < val_act && off == 0){ 
                remove_nodo(request, id_actt);
                changes ++;
                int old = val_act;
                
                val_act = 0;
                node = request -> firstNode;

                while (node != NULL){
                    if (node -> value >= val_act){
                        val_act = node -> value;
                        id_actt = node -> id;
                    }
                    node = node -> sgte;
                }
                off = 1;
                sprintf(aux, "%d", val_act);
                strcat(full, aux);
                strcat(full, ",");
                path_len += (old - val_act);
                comp = 256;
                node = request -> firstNode;
                goto RET;
            }
            //printf("valor final: %i\n", val_comp);
            //printf("id final: %i\n", id_comp);
            sprintf(aux, "%d", val_comp);
            strcat(full, aux);

            remove_nodo(request, id_actt);

            if (request -> firstNode -> sgte != NULL){
                strcat(full, ",");
            }
            path_len += comp;
           
            id_actt = id_comp;
            prev = val_act;
            val_act = val_comp;
            node = request -> firstNode;

        }
        else if (id_actt == id_comp){
            remove_nodo(request, id_actt);
            printf("%s\n", full);
            printf("%i\n", path_len);
            printf("%iT+%iD msec\n",path_len, changes);
            return;
        }
    }
}


void clook (struct Queue *request){
    struct nodo *node;
    char full[500];   
    char aux[20];
    int path_len = 0;
    int changes = 0;
    int prev = -1;
    int old = 0;
    int off = 0;
    node = request -> firstNode;
    int ini = node -> id;
    int val_act = node -> value;
    int id_actt = node -> id;
    node = node -> sgte;
    while (request -> firstNode != NULL){
        int comp = 256;
        int val_comp = node -> value;
        int id_comp = node -> id;

        RET:
        while (node != NULL){
            if (node -> id != id_actt){
                if (abs(node -> value - val_act) < comp && val_act <= node -> value){
                    comp = abs(node -> value - val_act);
                    id_comp = node -> id;
                    val_comp = node -> value;
                }
            }
            node = node -> sgte;
        }   
        if (id_actt != id_comp){
            if (val_comp < val_act){
                remove_nodo(request, id_actt);
                old = val_act;
                off = 1;
                id_actt = ini;
                val_act = 0;
                comp = 256;
                prev = 0;
                changes ++;
                changes ++;
                node = request -> firstNode;
                goto RET;
            }
            //printf("valor final: %i\n", val_comp);
            //printf("id final: %i\n", id_comp);
            sprintf(aux, "%d", val_comp);
            strcat(full, aux);

            remove_nodo(request, id_actt);

            if (request -> firstNode -> sgte != NULL){
                strcat(full, ",");
            }
            if (off == 1){
                path_len += (old - comp);
                off = 0;
            }else{
                path_len += comp;
            }
           
            id_actt = id_comp;
            prev = val_act;
            val_act = val_comp;
            node = request -> firstNode;

        }
        else if (id_actt == id_comp){
            remove_nodo(request, id_actt);
            printf("%s\n", full);
            printf("%i\n", path_len);
            printf("%iT+%iD msec\n",path_len, changes);
            return;
        }
    }
}

int main(int argc, char *argv[])
{   
    char scheduler[30];
    char filename[30];
    strcpy(scheduler, argv[1]);
    strcpy(filename, argv[2]);
    struct Queue *request = (struct Queue *)malloc(sizeof(struct Queue));
    request -> firstNode = NULL;
    request ->lastNode = NULL;
    FILE *fptr;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("No se puede abrir archivo \n");
        exit(0);
    }
    int value;
    int id = 0;
    while ((read = getline(&line, &len, fptr)) != -1) {
        value = atoi(line);
        id ++;
        add_nodo(request);
        request -> lastNode -> id = id;
        request -> lastNode -> value = value;    }
    fclose(fptr);
    //printf("%i\n", request -> firstNode -> id);
    if (strcmp(scheduler, "fcfs") == 0){
        fcfs(request);
    }
    else if (strcmp(scheduler, "sstf") == 0){
        sstf(request);
    }
    else if (strcmp(scheduler, "scan") == 0){
        scan(request);
    }
    else if (strcmp(scheduler, "c-look") == 0){
        clook(request);
    }
    return 0;
}