#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct Node{
    int viva;
    struct Node *sgte;   
};

struct NodeLis{
    int x;
    int y;
    struct NodeLis *sgte;   
};

struct Fila{
    struct Node *primerNode;
    struct Node *ultimoNode;
    struct Fila *sgteFila;
};

struct Columna
{
    struct Fila *primeraFila;
    struct Fila *ultimaFila;
};

struct Lista
{
    struct NodeLis *primerNode;
    struct NodeLis *ultimoNode;
};

void liberarMemoriaFil(struct Fila *lista) {
    struct Node *aux;
    while (lista -> primerNode != NULL) {
        aux = lista -> primerNode;
        lista -> primerNode = lista -> primerNode -> sgte;
        free(aux);
    }
}

void liberarMemoriaCol(struct Columna *lista) {
    struct Fila *aux;
    while (lista -> primeraFila != NULL) {
        aux = lista -> primeraFila;
        lista -> primeraFila = lista -> primeraFila -> sgteFila;
        free(aux);
    }
}
void liberarMemoriaLis(struct Lista *lista) {
    struct NodeLis *aux;
    while (lista -> primerNode != NULL) {
        aux = lista -> primerNode;
        lista -> primerNode = lista -> primerNode -> sgte;
        free(aux);
    }
}

void agregarFila(struct Columna *col){
    struct Fila *nuevaFila = (struct Fila *)malloc(sizeof(struct Fila));
    nuevaFila -> sgteFila = NULL;

    if (col -> primeraFila == NULL) {

        col -> primeraFila = nuevaFila;
        col -> ultimaFila = nuevaFila;

    } else {

        col -> ultimaFila -> sgteFila = nuevaFila;
        col -> ultimaFila = nuevaFila;

    }

    col -> ultimaFila -> sgteFila = NULL;

}

void agregarNodo(struct Fila *fil){
    struct Node *nuevoNodo = (struct Node *)malloc(sizeof(struct Node));
    nuevoNodo -> sgte = NULL;

    if (fil -> primerNode == NULL) {

        fil -> primerNode = nuevoNodo;
        fil -> ultimoNode = nuevoNodo;

    } else {

        fil -> ultimoNode -> sgte = nuevoNodo;
        fil -> ultimoNode = nuevoNodo;

    }

    fil -> ultimoNode -> sgte = NULL;

}

void agregarFichaViva(struct Columna *col, int fila, int columna){
    //printf("%i\n", fila);
    //printf("%i\n", columna);

    struct Fila *i = col -> primeraFila;
    struct Node *x;

    for(int f = 0; f < fila; f++ ) {
        //printf("f %i\n", f);
        i = i -> sgteFila;
    }
    x = i -> primerNode;
    for(int c = 0; c < columna; c ++){
        //printf("c %i\n", c);
        x = x -> sgte;
    }
    x -> viva = 1;
}
void agregarFichaMuerta(struct Columna *col, int fila, int columna){
    //printf("%i\n", fila);
    //printf("%i\n", columna);

    struct Fila *i = col -> primeraFila;
    struct Node *x;

    for(int f = 0; f < fila; f++ ) {
        //printf("f %i\n", f);
        i = i -> sgteFila;
    }
    x = i -> primerNode;
    for(int c = 0; c < columna; c ++){
        //printf("c %i\n", c);
        x = x -> sgte;
    }
    x -> viva = 0;
}

void imprimirMatrix(struct Columna *col){
    struct Fila *i = col -> primeraFila;
    struct Node *x;
    while(i != NULL) {
        x = i -> primerNode;
        while(x != NULL){
            printf("%i ", x->viva);
            x = x -> sgte;
        }
        printf("\n");
        i = i -> sgteFila;
    }
}

void input(char *prompt, char *buffer, size_t length)
{
    printf("%s ", prompt);
    fflush(stdout);
    fgets(buffer, length, stdin);
}

void agregarNodoLis(int pos1, int pos2, struct Lista *buscado){
    struct NodeLis *nuevoNodo = (struct NodeLis *)malloc(sizeof(struct NodeLis));
    nuevoNodo -> sgte = NULL;
    nuevoNodo -> x = pos1;
    nuevoNodo -> y = pos2;
    if (buscado -> primerNode == NULL) {
        buscado -> primerNode = nuevoNodo;
        buscado -> ultimoNode = nuevoNodo;

    } else {
        buscado -> ultimoNode -> sgte = nuevoNodo;
        buscado -> ultimoNode = nuevoNodo;
    }
    buscado -> ultimoNode -> sgte = NULL;
}

void imprimirLis(struct Lista *lis){
    struct NodeLis *i = lis -> primerNode;
    while(i != NULL) {
        printf("%i, %i\n", i->x, i->y);
        i = i -> sgte;
    }
}

void buscarVivas(struct Columna *Matrix, struct Lista *Vivas){
    struct Fila *i = Matrix -> primeraFila;
    struct Node *nuevoNodo = (struct Node *)malloc(sizeof(struct Node));
    nuevoNodo -> sgte = NULL;
    struct Node *x;
    int esp1;
    int esp2;
    esp1 = 0;
    esp2 = 0;
    Vivas -> primerNode == NULL;
    Vivas -> ultimoNode == NULL;
    while(i != NULL) {
        x = i -> primerNode;
        esp2 = 0;
        while(x != NULL){
            if (x -> viva == 1){
                agregarNodoLis(esp1, esp2, Vivas);
            }
            x = x -> sgte;
            esp2++;
        }
        i = i -> sgteFila;
        esp1++;
    }  
}

void buscarMuertas(struct Columna *Matrix, struct Lista *Muertas){
    struct Fila *i = Matrix -> primeraFila;
    struct Node *nuevoNodo = (struct Node *)malloc(sizeof(struct Node));
    nuevoNodo -> sgte = NULL;
    struct Node *x;
    int esp1;
    int esp2;
    esp1 = 0;
    esp2 = 0;
    Muertas -> primerNode == NULL;
    Muertas -> ultimoNode == NULL;
    while(i != NULL) {
        x = i -> primerNode;
        esp2 = 0;
        while(x != NULL){
            if (x -> viva == 0){
                agregarNodoLis(esp1, esp2, Muertas);
            }
            x = x -> sgte;
            esp2++;
        }
        i = i -> sgteFila;
        esp1++;
    }  
}

void revisar_cambio_ficha_viva(struct Lista *Vivas, struct Columna *Matrix, int filas, int columnas, struct Lista *Mueren){
    struct NodeLis *Nod = Vivas -> primerNode;
    int arax;
    int aray;
    int adx;
    int ady;
    int ddx;
    int ddy;
    int dax;
    int day;
    int aax;
    int aay;
    int aix;
    int aiy;
    int iix;
    int iiy;
    int iax;
    int iay;
    int x;
    int y;
    int contador;
    Mueren -> primerNode == NULL;
    Mueren -> ultimoNode == NULL;
    while (Nod != NULL){
        //imprimirMatrix(Matrix);
        contador = 0;
        x = Nod -> x;
        y = Nod -> y;

        arax = x - 1;
        aray = y;
        if (arax >= 0 && arax < filas  && aray >= 0 && aray < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, arax, aray);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (arax == 0 && aray == 0){
                x = i->primerNode;
            }
            else if (arax == 0){
                x = i -> primerNode;
                for(int z = 0; z < aray; z++){
                    x = x -> sgte;
                }
            }
            else if (aray == 0){
                for(int p = 0; p < arax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < arax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < aray; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        adx = x - 1;
        ady = y + 1;
        if (adx >= 0 && adx < filas  && ady >= 0 && ady < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, adx, ady);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (adx == 0 && ady == 0){
                x = i->primerNode;
            }
            else if (adx == 0){
                x = i -> primerNode;
                for(int z = 0; z < ady; z++){
                    x = x -> sgte;
                }
            }
            else if (ady == 0){
                for(int p = 0; p < adx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < adx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < ady; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        ddx = x;
        ddy = y + 1;
        if (ddx >= 0 && ddx < filas  && ddy >= 0 && ddy < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, ddx, ddy);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (ddx == 0 && ddy == 0){
                x = i->primerNode;
            }
            else if (ddx == 0){
                x = i -> primerNode;
                for(int z = 0; z < ddy; z++){
                    x = x -> sgte;
                }
            }
            else if (ddy == 0){
                for(int p = 0; p < ddx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < ddx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < ddy; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        dax = x + 1;
        day = y + 1;
        if (dax >= 0 && dax < filas  && day >= 0 && day < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, dax, day);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (dax == 0 && day == 0){
                x = i->primerNode;
            }
            else if (dax == 0){
                x = i -> primerNode;
                for(int z = 0; z < day; z++){
                    x = x -> sgte;
                }
            }
            else if (day == 0){
                for(int p = 0; p < dax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < dax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < day; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        aax = x + 1;
        aay = y;
        if (aax >= 0 && aax < filas  && aay >= 0 && aay < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, aax, aay);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (aax == 0 && aay == 0){
                x = i->primerNode;
            }
            else if (aax == 0){
                x = i -> primerNode;
                for(int z = 0; z < aay; z++){
                    x = x -> sgte;
                }
            }
            else if (aay == 0){
                for(int p = 0; p < aax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < aax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < aay; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        aix = x + 1;
        aiy = y - 1;
        if (aix >= 0 && aix < filas  && aiy >= 0 && aiy < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, aix, aiy);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (aix == 0 && aiy == 0){
                x = i->primerNode;
            }
            else if (aix == 0){
                x = i -> primerNode;
                for(int z = 0; z < aiy; z++){
                    x = x -> sgte;
                }
            }
            else if (aiy == 0){
                for(int p = 0; p < aix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < aix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < aiy; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        iix = x;
        iiy = y - 1;
        if (iix >= 0 && iix < filas  && iiy >= 0 && iiy < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, iix, iiy);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (iix == 0 && iiy == 0){
                x = i->primerNode;
            }
            else if (iix == 0){
                x = i -> primerNode;
                for(int z = 0; z < iiy; z++){
                    x = x -> sgte;
                }
            }
            else if (iiy == 0){
                for(int p = 0; p < iix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < iix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < iiy; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        iax = x - 1;
        iay = y - 1;
        if (iax >= 0 && iax < filas  && iay >= 0 && iay < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, iax, iay);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (iax == 0 && iay == 0){
                x = i->primerNode;
            }
            else if (iax == 0){
                x = i -> primerNode;
                for(int z = 0; z < iay; z++){
                    x = x -> sgte;
                }
            }
            else if (iay == 0){
                for(int p = 0; p < iax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < iax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < iay; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        //printf("contador: %i\n", contador);
        if (contador != 2 && contador != 3){
            agregarNodoLis(x, y, Mueren);
        }
        Nod = Nod -> sgte;
        //printf("posiciones:\n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i\n", arax, aray, adx, ady, ddx, ddy, dax, day, aax, aay, aix, aiy, iix, iiy, iax, iay);
    }
}

void revisar_cambio_ficha_muerta(struct Lista *Muertas, struct Columna *Matrix, int filas, int columnas, struct Lista *Viven){
    struct NodeLis *Nod = Muertas -> primerNode;
    int arax;
    int aray;
    int adx;
    int ady;
    int ddx;
    int ddy;
    int dax;
    int day;
    int aax;
    int aay;
    int aix;
    int aiy;
    int iix;
    int iiy;
    int iax;
    int iay;
    int x;
    int y;
    int contador;
    Viven -> primerNode == NULL;
    Viven -> ultimoNode == NULL;
    while (Nod != NULL){
        //imprimirMatrix(Matrix);
        contador = 0;
        x = Nod -> x;
        y = Nod -> y;
        arax = x - 1;
        aray = y;
        if (arax >= 0 && arax < filas  && aray >= 0 && aray < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, arax, aray);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (arax == 0 && aray == 0){
                x = i->primerNode;
            }
            else if (arax == 0){
                x = i -> primerNode;
                for(int z = 0; z < aray; z++){
                    x = x -> sgte;
                }
            }
            else if (aray == 0){
                for(int p = 0; p < arax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < arax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < aray; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        adx = x - 1;
        ady = y + 1;
        if (adx >= 0 && adx < filas  && ady >= 0 && ady < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, adx, ady);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (adx == 0 && ady == 0){
                x = i->primerNode;
            }
            else if (adx == 0){
                x = i -> primerNode;
                for(int z = 0; z < ady; z++){
                    x = x -> sgte;
                }
            }
            else if (ady == 0){
                for(int p = 0; p < adx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < adx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < ady; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        ddx = x;
        ddy = y + 1;
        if (ddx >= 0 && ddx < filas  && ddy >= 0 && ddy < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, ddx, ddy);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (ddx == 0 && ddy == 0){
                x = i->primerNode;
            }
            else if (ddx == 0){
                x = i -> primerNode;
                for(int z = 0; z < ddy; z++){
                    x = x -> sgte;
                }
            }
            else if (ddy == 0){
                for(int p = 0; p < ddx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < ddx; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < ddy; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        dax = x + 1;
        day = y + 1;
        if (dax >= 0 && dax < filas  && day >= 0 && day < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, dax, day);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (dax == 0 && day == 0){
                x = i->primerNode;
            }
            else if (dax == 0){
                x = i -> primerNode;
                for(int z = 0; z < day; z++){
                    x = x -> sgte;
                }
            }
            else if (day == 0){
                for(int p = 0; p < dax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < dax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < day; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        aax = x + 1;
        aay = y;
        if (aax >= 0 && aax < filas  && aay >= 0 && aay < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, aax, aay);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (aax == 0 && aay == 0){
                x = i->primerNode;
            }
            else if (aax == 0){
                x = i -> primerNode;
                for(int z = 0; z < aay; z++){
                    x = x -> sgte;
                }
            }
            else if (aay == 0){
                for(int p = 0; p < aax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < aax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < aay; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        aix = x + 1;
        aiy = y - 1;
        if (aix >= 0 && aix < filas  && aiy >= 0 && aiy < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, aix, aiy);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (aix == 0 && aiy == 0){
                x = i->primerNode;
            }
            else if (aix == 0){
                x = i -> primerNode;
                for(int z = 0; z < aiy; z++){
                    x = x -> sgte;
                }
            }
            else if (aiy == 0){
                for(int p = 0; p < aix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < aix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < aiy; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        iix = x;
        iiy = y - 1;
        if (iix >= 0 && iix < filas  && iiy >= 0 && iiy < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, iix, iiy);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (iix == 0 && iiy == 0){
                x = i->primerNode;
            }
            else if (iix == 0){
                x = i -> primerNode;
                for(int z = 0; z < iiy; z++){
                    x = x -> sgte;
                }
            }
            else if (iiy == 0){
                for(int p = 0; p < iix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < iix; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < iiy; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        iax = x - 1;
        iay = y - 1;
        if (iax >= 0 && iax < filas  && iay >= 0 && iay < columnas ){
            //printf("entro %i, %i  ###  revisando: %i, %i\n", x, y, iax, iay);
            struct Fila *i = Matrix -> primeraFila;
            struct Node *x;
            if (iax == 0 && iay == 0){
                x = i->primerNode;
            }
            else if (iax == 0){
                x = i -> primerNode;
                for(int z = 0; z < iay; z++){
                    x = x -> sgte;
                }
            }
            else if (iay == 0){
                for(int p = 0; p < iax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
            }
            else{
                for(int p = 0; p < iax; p++) {
                    i = i -> sgteFila;
                }
                x = i -> primerNode;
                for(int z = 0; z < iay; z++){
                    x = x -> sgte;
                }
            }
            //printf("viva: %i\n", x->viva);
            if (x->viva == 1){
                contador ++;
            }
        }

        //printf("contador: %i\n", contador);
        if (contador == 3){
            agregarNodoLis(x, y, Viven);
        }
        Nod = Nod -> sgte;
        //printf("posiciones:\n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i \n%i, %i\n", arax, aray, adx, ady, ddx, ddy, dax, day, aax, aay, aix, aiy, iix, iiy, iax, iay);
    }
}

void cambiar_matrix(struct Lista *Viven, struct Lista *Mueren, struct Columna *Matrix, int filas, int columnas){
    struct NodeLis *Nod = Viven -> primerNode;
    int xx;
    int yy;

    while (Nod != NULL){
        xx = Nod -> x;
        yy = Nod -> y;
        agregarFichaViva(Matrix, xx, yy);
        Nod = Nod -> sgte;
    }
    Nod = Mueren -> primerNode;
    while (Nod != NULL){
        xx = Nod -> x;
        yy = Nod -> y;
        agregarFichaMuerta(Matrix, xx, yy);
        Nod = Nod -> sgte;
    }
    
}
int main(int argc, char *argv[]){
    int iteraciones = atoi(argv[1]);
    int filas = atoi(argv[2]);
    int columnas = atoi(argv[3]);
    int vivas = atoi(argv[4]);
    int threads = atoi(argv[5]);
    char numero_viva[10];
    struct Columna *Matrix = (struct Columna *)malloc(sizeof(struct Columna));
    struct Lista *Vivas = (struct Lista *)malloc(sizeof(struct Lista));
    struct Lista *Mueren = (struct Lista *)malloc(sizeof(struct Lista));
    struct Lista *Muertas = (struct Lista *)malloc(sizeof(struct Lista));
    struct Lista *Viven = (struct Lista *)malloc(sizeof(struct Lista));
    Matrix -> primeraFila = NULL;
    Matrix -> ultimaFila = NULL;

    for (int i = 0; i < filas; i++) 
    {
        agregarFila(Matrix);
        if (i == 0)
        {
            Matrix -> ultimaFila -> primerNode = NULL;
            Matrix -> ultimaFila -> ultimoNode = NULL;
        }
        for (int o = 0; o < columnas; o++)
        {
             agregarNodo(Matrix -> ultimaFila);
             Matrix -> ultimaFila -> ultimoNode -> viva = 0; 
        }
    }

    for (int i = 0; i < vivas; i++)
    {
        input("Agregue la posicion de una ficha viva:", numero_viva, sizeof(numero_viva));
        char a = numero_viva[0];
        char b = numero_viva[2];
        agregarFichaViva(Matrix, a - '0', b - '0');
    }
    printf("\n\nTablero Inicial:\n");
    imprimirMatrix(Matrix);
    printf("\n\n\n");
    for (int rep = 0; rep < iteraciones; rep ++){
        // imprimirMatrix(Matrix);
        buscarVivas(Matrix, Vivas);
        // printf("vivas\n");
        // imprimirLis(Vivas);
        // printf("vivas\n");
        revisar_cambio_ficha_viva(Vivas, Matrix, filas, columnas, Mueren);
        // printf("mueren\n");
        // imprimirLis(Mueren);
        // printf("mueren\n");
        buscarMuertas(Matrix, Muertas);
        // printf("muertas\n");
        // imprimirLis(Muertas);
        // printf("muertas\n");
        revisar_cambio_ficha_muerta(Muertas, Matrix, filas, columnas, Viven);
        // printf("viven\n");
        // imprimirLis(Viven);
        // printf("viven\n");
        cambiar_matrix(Viven, Mueren, Matrix, filas, columnas);
        // imprimirMatrix(Matrix);
    }
    printf("Tablero Final:\n");
    imprimirMatrix(Matrix);

    liberarMemoriaCol(Matrix);
    liberarMemoriaLis(Vivas);
    liberarMemoriaLis(Muertas);
    liberarMemoriaLis(Viven);
    liberarMemoriaLis(Mueren);
    // printf("%i %i %i %i %i\n", iteraciones, filas, columnas, vivas, threads);  
}
