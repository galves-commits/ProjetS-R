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
	struct sockaddr_in sin;
	int ecoute;
	Cellule *trains;
	char fichier[MAX];
	sprintf(fichier, "train%s.txt", argv[1]);
	printf("Reading '%s'\n", fichier);
	trains = traiterFic(fichier);
	printf("Runing\n");
	ecoute = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	int port = PORT + atoi(argv[1]);
	sin.sin_port = htons(port);

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(ecoute, (struct sockaddr *)&sin, sizeof(sin));
	listen(ecoute, 5);

	while (true)
	{
		int echange = accept(ecoute, NULL, NULL);
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
