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
//Recuperation des trains dans le fichier en parametre 
	Cellule *trains;
	//cas ou pas de fichier en parametre
	if (argv[2]==NULL){
		printf(RED"Veuillez entrer un nom de fichier en deuxieme argument\n");
		exit(EXIT_SUCCESS);
	}
	printf("Reading '%s'\n", argv[2]);
	trains = traiterFic(argv[2]);
	printf("Runing\n");

//Creation du socket et configuration du port
	struct sockaddr_in sin;
	int ecoute;
	ecoute = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_family = AF_INET;
	int port = PORT + atoi(argv[1]);
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(ecoute, (struct sockaddr *)&sin, sizeof(sin));
	listen(ecoute, 5);

//boucle d'acceptation de clients
	while (true)
	{
		int echange = accept(ecoute, NULL, NULL);
		//creation d'un fils a chaque nouvelle connection
		pid_t pid = fork();
		switch (pid)
		{
		case 0:
			printf("Un client s'est connecté\n");
			//appel de la fonction de prise en charge de requetes 
			requeteTrajet(echange, trains);
			break;

		case -1:
			//exit en cas d'erreur lors de la creation du fils
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
	//recuperation de la requete envoyée par le client
	int nblus = read(echange, req, MAX);
	int nbtrains;

	//decomposition de la requete
	sscanf(req, "%[^;\n];%[^;\n];%[^;\n];%[^;\n]", protocole, villeD, villeA, heure);
	printf("%s", req);
	printf("'%d';'%s';'%s';'%s';'%s'\n", nblus, protocole, villeD, villeA, heure);

	//Verification du protocol utilisé
	if (strcmp(protocole, "TV") == 0 || strcmp(protocole, "TH") == 0 || strcmp(protocole, "TB") == 0)
	{
		//recuperation des trains correspondant à la requete
		Cellule *goodTrains = getTrains(protocole, villeD, villeA, heure, trains, &nbtrains);
		//fonction envoyant les resultats au client
		reponseRequete(goodTrains, echange, nbtrains);
	}else{
		//envoie d'un marqueur d'erreur
		write(echange,"-1",3);
	}
}
