#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

pid_t termpid = 0;
int status = 0;


int leer_comando(char comando[], char *argv[], int *argc) {
	char line[1024], *array[100], *pch;
	int i = 0, j = 0;

	fgets(line, sizeof line, stdin);
	memset(array, 0, sizeof array);

	pch = strtok (line, " \n");
	while (pch != NULL) {
		array[i] = strdup(pch);
		strcpy(array[i], pch);
		pch = strtok (NULL, " \n");
		i++;
	}
	if (array[0] == NULL) {
		return 1;
	}
	strcpy(comando, array[0]);
	*argc = i;

	for(j = 0; j < i; j++)
		argv[j] = array[j];
	while (i < 20) {
		argv[i] = NULL;
		i++;
	}

	return 0;
}

void childHandler(int sig)
{
	wait(&status);  
}

void intHandler(int sig)
{
	if(termpid == 0)
		exit(0);
}

int main()
{
	char cmd[100], comando[100], *argv[20];
	int argc = 0;
	int *p_argc = &argc;
	char dirPath[100] = "";
	char prompt[100] = "> Ingrese un comando:";
	signal(SIGCHLD, childHandler);
	signal(SIGINT, intHandler);
	
	while(1) {
		char usePath[120] = "PATH=";
		strcat(usePath, dirPath);
		printf("%s ", prompt);
		strcpy(comando, ":");
		int pass = leer_comando(comando, argv, p_argc);
		if (pass != 0) {
			continue;
		}
		if (strcmp(argv[argc-1], "&") == 0) {
			// background
			argv[argc-1] = NULL;
			if (fork() == 0) {
				strcpy(cmd, dirPath);
				strcat(cmd, comando);
				execv(cmd, argv);
				exit(0);
			}
		} else if (argv[argc-1][0] == '&') {
			// multiple background
			char *p_num = argv[argc-1] + 1;
			int num = atoi(p_num);
			argv[argc-1] = NULL;
			for (int n = 0; n < num; n++) {
			
				if (fork() == 0) {
					strcpy(cmd, dirPath);
					strcat(cmd, comando);
					execv(cmd, argv);
					exit(0);
				}

			}

		} else {
			// comandos built-in
			if (strcmp(comando, "setPath") == 0){
				if (argv[1]){
					strcpy(dirPath, argv[1]);
				}
			} else if (strcmp(comando, "setPrompt") == 0){
				if (argv[1]) {
					if (argv[1][0] == '*'){
						char strexit[20];
						sprintf(strexit, "%d", status);
						strcpy(prompt, strexit);
					} else {
						strcpy(prompt, argv[1]);
					}
				}
			} else {
				// ejecuta comando y hace wait()
				pid_t childpid = fork();
				if ( childpid == 0 ) {
					strcpy(cmd, dirPath);
					strcat(cmd, comando);
					execv(cmd, argv);
					exit(0);
				} else {
					termpid = childpid;
					wait(&status);
					termpid = 0;
				}
			}
			
			if (strcmp(comando, "exit") == 0) 
				break;
		}	
	}

	return 0;
}