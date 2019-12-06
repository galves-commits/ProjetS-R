#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../headers/trains.h"
#include "../headers/dialogue.h"

#define MAX 400

int main(int argc, char *argv[], char *arge[])
{ //serv1
	int nbserv = argc - 1;
	int serv[nbserv];

	for (int i = 1; i < argc; i++)
	{
		serv[i - 1] = atoi(argv[i]);
	}

	int connection[nbserv];
	struct sockaddr_in sin[nbserv];
	struct hostent *host[nbserv];

	for (int i = 0; i < nbserv; i++)
	{
		connection[i] = socket(AF_INET, SOCK_STREAM, 0);
		sin[i].sin_family = AF_INET;
		sin[i].sin_port = htons(PORT + serv[i]);
		host[i] = gethostbyname("localhost");
		memcpy(&sin[i].sin_addr.s_addr, host[i]->h_addr_list[0], sizeof(sin[i].sin_addr.s_addr));
		int echange = connect(connection[i], (struct sockaddr *)&sin[i], sizeof(sin[i]));
	}

	//serv2

	int nbLus;
	char ans[MAX];
	admin();

	Cellule * trains = malloc(sizeof(Cellule));
	int nbtrains;
	char * reponse = malloc(sizeof(char));
	sendRequete(&reponse);
	for (int i = 0; i < nbserv; i++)
	{
	int nbtrainsserv;	
	write(connection[i], reponse, strlen(reponse) + 1);
	getRequete(connection[i],&trains,&nbtrainsserv);
	nbtrains += nbtrainsserv;
	}

	printRequete(nbserv,trains,nbtrains);
}
