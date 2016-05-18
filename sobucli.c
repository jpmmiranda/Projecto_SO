#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define TAM 1024

int main(int args, char* argv[]){
	int cmp = 0, t, i = 0;
	char* opcao;
	char* ficheiro;
	char buffer[TAM];
	mkfifo("/tmp/sobucli_fifo",0666);

	if(args == 3){
		int fs = open("/tmp/sobusrv_fifo",O_WRONLY);
		int fc = open("/tmp/sobucli_fifo",O_RDONLY);
		opcao = strdup(argv[1]);
		ficheiro = strdup(argv[2]);
		
		strcat(opcao," ");
		strcat(opcao, ficheiro);
		t = strlen(opcao);
		if(cmp == 0){
			write(fs, opcao, t);
		}

		while((t=read(fc,buffer,TAM)) > 0){
        	write(1,buffer,t);
		}			
		close(fc);
		close(fs);
	}

	else if(args<3){
		printf("Argumentos insuficientes para a execução do programa.\n");
	}
	else if(args>3){
		printf("Demasiados argumentos para a execução do programa.\n");
	}

}