#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TAM 1024

int backup(char* ficheiro, char* directoriaData, char* directoriaMetadata){
	int f1,f2, sucesso, bytes;
	int p[2];
	char buffer[TAM];
	char *ficheiroZip = strdup(ficheiro); strcat(ficheiroZip,".gz");

	f1 = fork();
	if(f1==0){
		printf("Processo filho a executar o backup..\n");
		if(execlp("gzip","gzip", ficheiro, NULL)==-1){
			perror("Gzip");
			return -1;
		}
	}
	else{
		wait();
		sucesso = pipe(p);
		f2 = fork();
		if(f2==0 && sucesso == 0){
			dup2(p[1],1);
			close(p[0]);
			if(execlp("sha1sum","sha1sum", ficheiroZip, NULL)==-1){
				perror("sha1sum");
				return -1;
			}
			close(p[1]);
		}
		dup2(p[0],p[1]);
		close(p[1]);
		bytes = read(p[0], buffer, TAM);
		close(p[0]);
	}
	
	char *nomeSum = strtok(buffer, " ");
	char* nomeData = (char*)malloc(sizeof(directoriaData)+sizeof(nomeSum));
	strcat(nomeData,directoriaData);
	strcat(nomeData, nomeSum);
	if(fork() == 0){
		if(fork() == 0){
			if(execlp("mv","mv",ficheiroZip, nomeSum, NULL)==-1){
			 perror("Renomear o ficheiro");
			 return -1;
			}
		}
		wait();
		if(execlp("mv","mv",nomeSum, directoriaData, NULL)==-1){
			perror("Mover para data"); 
			return -1;
		}
	}
	else{
		wait();
		if(fork()==0){
			if(execlp("ln", "ln", "-s", nomeData, ficheiro, NULL)==-1){
				perror("Criar link");	
				return -1;
			}
		}
		else{
			wait();
			if(fork()==0){
				if(execlp("mv","mv",ficheiro, directoriaMetadata, NULL)==-1){
				perror("Mover para metadata");
				return -1;
				}
			}
		}
	}
	return 0;
}




int restore(char* ficheiro, char* directoriaMetadata){
	int sucesso, bytes;
	int p[2];
	char buffer[TAM];
	char *ficheiroZip = strdup(ficheiro); strcat(ficheiroZip,".gz");
	char* nomeLink = (char*) malloc(sizeof(directoriaMetadata)+sizeof(ficheiro));
	strcat(nomeLink, directoriaMetadata);
	strcat(nomeLink, ficheiro);

	sucesso = pipe(p);
	if(fork()==0){
		printf("Processo filho a executar o restore..\n");
		dup2(p[1],1);
		close(p[0]);
		if(execlp("readlink","readlink", nomeLink, NULL)==-1){
			perror("readlink falhou.");
			return -1;
		}
		close(p[1]);
	}
	dup2(p[0],0);
	close(p[1]);
	bytes = read(p[0],buffer,TAM);
	close(p[0]);

	char* pathLink = strtok(buffer,"\n");
	printf("Link: %s, bytes: %d\n",pathLink, bytes );

	if(fork()==0){
		if(execlp("mv","mv",pathLink, ficheiroZip,NULL)==-1){
			perror("Restore move falhou.");
			return -1;
		}
	}
	else{
		wait();
		if(fork()==0){
			if(execlp("gunzip","gunzip",ficheiroZip,NULL)==-1){
				perror("Unzip falhou.");
				return -1;
			}
		}
	}
	return 0;

}






int main(int args, char* argv[]){
	int t, resultado;
	char buffer[TAM], directoriaData[TAM], directoriaMetadata[TAM], username[TAM], opcao[TAM], ficheiro[TAM];
	printf("\033c");
	printf("A aguardar...\n");
	getlogin_r(username,TAM);
	sprintf(directoriaData,"/home/%s/.Backup/data/",username);
	sprintf(directoriaMetadata,"/home/%s/.Backup/metadata/",username);
	mkfifo("/tmp/sobusrv_fifo",0666);

	int fs = open("/tmp/sobusrv_fifo", O_RDWR);
	while((t=read(fs,buffer,TAM)) >= 0){
		resultado = 1;
		memset(opcao, 0, sizeof(opcao));
		memset(ficheiro, 0, sizeof(ficheiro));
        sscanf(buffer,"%s %s", opcao, ficheiro);
        if(strcmp(opcao, "backup")==0){
        	resultado = backup(ficheiro, directoriaData, directoriaMetadata);
        	if(resultado == 0){
        		printf("Backup executado com sucesso.\n");
        	}
		}
		else if(strcmp(opcao, "restore") == 0){
			resultado = restore(ficheiro, directoriaMetadata);
			if(resultado == 0){
        		printf("Restore executado com sucesso.\n");
        	}
		}
	} 

    return 0;

}