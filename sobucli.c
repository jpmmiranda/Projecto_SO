#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define TAM 1024

void erro(int br){
	printf("Erro a executar.\n");
}

void backup(int b){
	printf("Backup concluido.\n");
}

void restore(int r){
	printf("Restore concluido.\n");
}

int main(int args, char* argv[]){
	int cmp = 0, t, i = 0;
	char* opcao;
	char* ficheiro;
	char pidCliente[10];
	sprintf(pidCliente," %d",getpid());
	char buffer[TAM];

	if(args == 3){
		int fs = open("/tmp/sobusrv_fifo",O_RDWR);
		opcao = strdup(argv[1]);
		ficheiro = strdup(argv[2]);
		
		strcat(opcao," ");
		strcat(opcao, ficheiro);
		strcat(opcao, pidCliente);
		t = strlen(opcao);
		signal(SIGQUIT, erro);
		signal(SIGUSR1, backup);
		signal(SIGUSR2, restore);
		if(cmp == 0){
			write(fs, opcao, t);
			pause();
		}
		close(fs);
	}

	else if(args<3){
		printf("Argumentos insuficientes para a execução do programa.\n");
	}
	else if(args>3){
		printf("Demasiados argumentos para a execução do programa.\n");
	}

}