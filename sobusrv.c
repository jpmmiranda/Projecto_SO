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
	char buffer[TAM], directoria[TAM], username[TAM], opcao[TAM], ficheiro[TAM];
	printf("\033c");
	printf("A aguardar...\n");
	getlogin_r(username,TAM);
	sprintf(directoria,"/home/%s/.Backup/",username);
	mkfifo("/tmp/sobusrv_fifo",0666);

	int fs = open("/tmp/sobusrv_fifo", O_RDONLY);
	
	while((t=read(fs,buffer,TAM)) > 0){
        write(1,buffer,t);
        sscanf(buffer,"%s %s", opcao, ficheiro);
        printf("\n");
        if(strcmp(opcao, "backup")==0){
        	printf("lol\n");
	        	execlp("gzip","gzip", ficheiro, NULL);
	        	execlp("sha1sum","sha1sum", strcat(ficheiro,".gz"), NULL);
		}






	}
    
    close(fs);

    return 0;

}