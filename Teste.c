#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(){
	printf("ola\n");
	int p[2];
	char buffer[128];
	pipe(p);

	if(fork()==0){
		dup2(p[1],1);
		close(p[0]);
		execlp("sha1sum","sha1sum", "testes.txt.gz", NULL);
		close(p[1]);
	
	}
	else{
		dup2(p[0],p[1]);
		close(p[1]);
		read(p[0],buffer,128);
		close(p[0]);
		printf("%s\n", buffer);
	}


}