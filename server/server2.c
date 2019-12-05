#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/trains.h"
#include "../headers/server.h"
#include "../headers/dialogue.h"

int main(int argc, char *argv[], char *arge[])
{
	Cellule *trains = traiterFic("train2.txt");
	printf("Runing\n");

	int ecoute2 = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin2;
	sin2.sin_family = AF_INET;
	sin2.sin_port = htons(7778);
	sin2.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(ecoute2, (struct sockaddr *)&sin2, sizeof(sin2));
	listen(ecoute2, 5);

	while (true)
	{
		int echange = accept(ecoute2, NULL, NULL);
		pid_t pid = fork();
		switch (pid)
		{
		case 0:
			printf("Un client s'est connecté\n");
			requeteTrajet(echange, trains);
			break;

		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
			break;
		}
	}
}


void requeteTrajet(int echange, Cellule *trains)
{
	char req[MAX];
	char protocole[MAX];
	char reste[MAX];
	char villeD[MAX];
	char villeA[MAX];
	char heure[MAX];
	int nblus = read(echange, req, MAX);
	int nbtrains;

	sscanf(req, "%[^;\n];%[^;\n];%[^;\n];%[^;\n]", protocole, villeD, villeA, heure);
	printf("%s", req);
	printf("'%d';'%s';'%s';'%s';'%s'\n", nblus, protocole, villeD, villeA, heure);
	if (strcmp(protocole, "TV") == 0)
	{
		Cellule *goodTrains = getTrains(protocole, villeD, villeA, "", trains, &nbtrains);
		reponseRequete(goodTrains, echange, nbtrains);
	}
	else
	{
		if (strcmp(protocole, "TH") == 0)
		{
			Cellule *goodTrains = getTrains(protocole, villeD, villeA, heure, trains, &nbtrains);
			reponseRequete(goodTrains, echange, nbtrains);
		}
		else
		{
			if (strcmp(protocole, "TB") == 0)
			{
				Cellule *goodTrains = getTrains(protocole, villeD, villeA, heure, trains, &nbtrains);
				reponseRequete(goodTrains, echange, nbtrains);
			}
		}
	}
}


