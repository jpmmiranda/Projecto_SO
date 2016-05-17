#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int args, char* argv[]){
	int cmp = 0;
	
	cmp = strcmp(argv[1],"backup");
	if(cmp == 0){
		execlp() // executa um comando para mover os ficheiros para a pasta backup
	}

}