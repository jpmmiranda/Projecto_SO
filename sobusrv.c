#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TAM 1024

int main(int args, char* argv[]){
	int t;
	char buffer[TAM];
	printf("\033c");
	printf("A aguardar...\n");
	mkfifo("/tmp/sobusrv_fifo",0666);

	int fs = open("/tmp/sobusrv_fifo", O_RDONLY);
    int fc = open("/tmp/sobucli_fifo", O_WRONLY);
	
	while((t=read(fs,buffer,TAM)) > 0){
        write(1,buffer,t);
        strcat(buffer,"-> Efectuado com sucesso.");
        write(fc,buffer , t+26);
	}
    
    close(fs);
    close(fc);

    return 0;

}