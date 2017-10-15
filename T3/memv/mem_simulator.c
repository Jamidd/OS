#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

float cant_hit_tlb = 0;
float cant_pagefault = 0;
float cant_total = 0;

struct Tabla
{
	long int frame[255];
};

struct Tlb
{
	long int page_number[32];
	long int frame[32];
	long int freq[32]; // LRU -> indice corresponde al numero de entrada
	long int pos[32]; // FIFO -> indice corresponde al numero de entrada
};

struct Ram
{
	long int frame_used[128];
	long int freq[128]; // LRU -> indice corresponde al numero de frame
	long int pos[128]; // FIFO -> indice corresponde al numero de frame
};

struct Tabla TablaPagina;
struct Tlb TLB;
struct Ram RAM;


void appendCharToString(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void reverse_string(char *str) //https://stackoverflow.com/questions/784417/reversing-a-string-in-c
{
    /* skip null */
    if (str == 0)
    {
        return;
    }

    /* skip empty string */
    if (*str == 0)
    {
        return;
    }

    /* get range */
    char *start = str;
    char *end = start + strlen(str) - 1; /* -1 for \0 */
    char temp;

    /* reverse */
    while (end > start)
    {
        /* swap */
        temp = *start;
        *start = *end;
        *end = temp;

        /* move */
        ++start;
        --end;
    }
}

long int toBinary(long int num){
	if (num < 2){
		return num;
	}
	return toBinary(num / 2) * 10 + num % 2;
}

void toBinary16bitString(long int num, char *str){
	long int binary = toBinary(num);
	sprintf(str, "%li", binary);
	char aux[50];
	strcpy(aux, str);
	reverse_string(aux);
	int k;
	for(k = 0; aux[k] != '\0'; ++k);
	int fill = 16 - k;
	for (int i = 0; i < fill; ++i)
	{
		appendCharToString(aux, '0');
	}
	reverse_string(aux);
	strcpy(str, aux);
}

long int binary8bitToInt(char *str){
	long int n = 0;
	char aux[16];
	strcpy(aux, str);
	reverse_string(aux);
	
	for (int i = 0; i < 8; i++) {
			n += (aux[i] - '0') * pow(2, i);
	}
	return n;
}

void agregarTLB(int page, int frame, int politica){

	for (int i = 0; i < 32; ++i)
	{
		if (TLB.page_number[i] == -1)
		{
			TLB.page_number[i] = page;
			TLB.frame[i] = frame;
			TLB.freq[i] = 0;
			return;
		}
		
	}
	// aplicamos politica de reemplazo, 0 -> lru - 1 -> fifo
	if (politica == 0) {

		// agregamos por LRU

		int candidato = 0;
		int freq = TLB.freq[0];
		for (int i = 0; i < 32; ++i)
		{
			if (TLB.freq[i] < freq) {
				candidato = i;
				freq = TLB.freq[i];
			}
		}

		TLB.page_number[candidato] = page;
		TLB.frame[candidato] = frame;
		TLB.freq[candidato] = 0;

	}

	else {

		// agregamos por FIFO

		int candidato = 0;
		int pos = 0;
		for (int i = 0; i < 32; ++i)
		{
			if (TLB.pos[i] > pos) {
				candidato = i;
				pos = TLB.pos[i];
			}
		}

		TLB.page_number[candidato] = page;
		TLB.frame[candidato] = frame;
		TLB.pos[candidato] = 0;
	
	}

}

long int verificarFrameDisponible(int politica){

	for (int i = 0; i < 128; ++i)
	{
		if (RAM.frame_used[i] == 0){
			RAM.frame_used[i] = 1;
			RAM.freq[i] = 0;
			RAM.pos[i] = 0;
			return i;
		}
	}
	// si no hay disponible, aplicar política de reemplazo
	if (politica == 0) {
		// verificamos por LRU

		int candidato = 0;
		int freq = RAM.freq[0];
		for (int i = 0; i < 128; ++i)
		{
			if (RAM.freq[i] < freq) {
				candidato = i;
				freq = RAM.freq[i];
			}
		}

		RAM.freq[candidato] = 0;

		return candidato;


	}

	else {

		// verificamos por FIFO

		int candidato = 0;
		int pos = 0;
		for (int i = 0; i < 128; ++i)
		{
			if (RAM.pos[i] > pos) {
				candidato = i;
				pos = RAM.pos[i];
			}
		}

		RAM.pos[candidato] = 0;

		return candidato;
	
	}

	return 0;

}

void aumentarPos(){
	for (int i = 0; i < 32; ++i)
	{
		if (TLB.page_number[i] > -1) {
			TLB.pos[i] += 1;
		}
	}

	for (int i = 0; i < 128; ++i)
	{
		if (RAM.frame_used[i] == 1) {
			RAM.pos[i] += 1;
		}
	}
}

void handler(){
	printf("\nPORCENTAJE_PAGE_FAULTS = %.0f %% \n", 100*cant_pagefault/cant_total);
	printf("PORCENTAJE_TLB_HITS = %.0f %% \n", 100*cant_hit_tlb/cant_total);
	printf("TABLA DE PAGINAS\n");
	printf("page_number\tframe_number\n");
	for (int page = 0; page < 256; ++page)
	{
		if (TablaPagina.frame[page] == -1) {
			printf("%i\t\t\n", page);
		}
		else {
			printf("%i\t\t%li\n", page, TablaPagina.frame[page]);
		}
		
	}
	printf("TLB\n");
	printf("i\tpage_number\tframe_number\n");
	for (int i = 0; i < 32; ++i)
	{
		if (TLB.page_number[i] == -1) {
			printf("%i\t\t\t\n", i);
		}
		else {
			printf("%i\t%li\t\t%li\n", i, TLB.page_number[i], TLB.frame[i]);
		}
		
	}
    exit(0);
}



int main(int argc, char const *argv[])
{
	signal(SIGINT, handler);
	if (argc != 2) {
		printf("Error Argumentos\n");
		exit(1);
	}


	int politica;

	if (strcmp(argv[1], "lru") == 0){
		politica = 0;
	} else {
		politica = 1;
	}
	

	for (int i = 0; i < 256; ++i)
	{
		TablaPagina.frame[i] = -1;
	}

	

	for (int i = 0; i < 32; ++i)
	{
		TLB.page_number[i] = -1;
		TLB.frame[i] = -1;
		TLB.freq[i] = 0;
	}


	for (int i = 0; i < 128; ++i)
	{
		RAM.frame_used[i] = 0;
		RAM.freq[i] = 0;
	}

	while (1) {
		char str_binary[16] = "";
		char str_page[8] = "";
		char str_offset[8] = "";
		long int page = 0;
		long int offset = 0;
		char n_str[20] = "";
		if (fgets(n_str, 20, stdin) == NULL){
			handler();
		}
		fflush(NULL);
		cant_total += 1;
		long int n = atoi(n_str);
		toBinary16bitString(n, str_binary);
		for (int i = 0; i < 8; ++i)
		{
			appendCharToString(str_page, str_binary[i]);
		}
		page = binary8bitToInt(str_page);

		for (int i = 8; i < 16; ++i)
		{
			appendCharToString(str_offset, str_binary[i]);
		}
		offset = binary8bitToInt(str_offset);

		// Ir a TLB

		int miss = 1; // suponemos que hay miss
		for (int i = 0; i < 32; ++i)
		{
			if (TLB.page_number[i] == page){
				miss = 0; // no hay miss
				cant_hit_tlb += 1;
				TLB.freq[i] += 1;
				break; 
			}
		}
		if (miss) { // no está en tlb

			// Ir a tabla de pagina

			int page_fault = 1; // suponemos page fault
			if (TablaPagina.frame[page] > -1){
				page_fault = 0;
				RAM.freq[TablaPagina.frame[page]] += 1;
				agregarTLB(page, TablaPagina.frame[page], politica);
			}

			if (page_fault) {

				cant_pagefault += 1;
				int frame_disponible = verificarFrameDisponible(politica);
				// agregamos frame a TablaPagina
				TablaPagina.frame[page] = frame_disponible;
				// agregamos page y frame a TLB
				agregarTLB(page, frame_disponible, politica);
			}
		}

		aumentarPos();
	}

	return 0;
}

