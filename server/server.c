#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 40000


void echangeClient(int);

int main(int argc,char *argv[],char *arge[]){
	int ecoute = socket (AF_INET , SOCK_STREAM , 0  ) ;
	struct sockaddr_in sin ;
	sin.sin_family= AF_INET ;
	sin.sin_port = htons(7777) ;
	sin.sin_addr.s_addr = htonl(INADDR_ANY ) ;
	bind(ecoute , (struct sockaddr*) &sin , sizeof(sin) ) ;
	listen(ecoute, 5 );
	while(true){

		int echange = accept (ecoute ,NULL,NULL);
		pid_t pid = fork();

		switch(pid){
			case 0:	printf("Un client s'est connecté\n" ) ;
					echangeClient(echange);
					break;

			case -1:perror("fork");
					exit(EXIT_FAILURE);
					break;
		}
	} 
}

void echangeClient(int echange){
	char tampon[MAX];
	int nbLus= read(echange,tampon,MAX) ;
	printf("J'ai lus '%s'\n" , tampon ) ;
}