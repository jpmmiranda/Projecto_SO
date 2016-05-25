#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define TAM 1024

void erro(int br){
	printf("Erro a executar.\n");
	_exit(0);
}

void backup(int b){
	printf("Backup concluido.\n");
}

void restore(int r){
	printf("Restore concluido.\n");
}

void naoExiste(int r){
	printf("Link não existe.\n");
	_exit(0);
}

int main(int args, char* argv[]){
	int cmp = 0, t, i = 0;
	char* opcao;
	char* ficheiro;
	char pidCliente[10];
	sprintf(pidCliente," %d",getpid());
	char buffer[TAM];

	if(args >= 3){
		int fs = open("/tmp/sobusrv_fifo",O_WRONLY);
		opcao = strdup(argv[1]);
		ficheiro = strdup(argv[2]);
		

		if(strcmp(opcao,"backup")==0){
			int existe = access(ficheiro, F_OK);
			if(existe==0){
				signal(SIGKILL, erro);
				signal(SIGUSR1, backup);
				strcat(opcao," ");
				strcat(opcao, ficheiro);
				strcat(opcao, pidCliente);
				t = strlen(opcao);
				if(cmp == 0){
					int i = write(fs, opcao, t);
					pause();
				}
				close(fs);
			}
			else printf("Ficheiro não existe.\n");
		}
		else if(strcmp(opcao,"restore")==0){
			int existe = access(ficheiro, F_OK);
			if(existe!=0){	
				signal(SIGQUIT, erro);
				signal(SIGUSR2, restore);
				signal(SIGTERM, naoExiste);
				strcat(opcao," ");
				strcat(opcao, ficheiro);
				strcat(opcao, pidCliente);
				t = strlen(opcao);
				if(cmp == 0){
					int i =write(fs, opcao, t);
					pause();
				}
				close(fs);
			}
			else printf("Ficheiro já existe com esse nome.\n");
		}
	}

	else if(args<3){
		printf("Argumentos insuficientes para a execução do programa.\n");
	}
	else if(args>3){
		printf("Demasiados argumentos para a execução do programa.\n");
	}
	return 0;
}