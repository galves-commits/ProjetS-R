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

void reponseRequete(Cellule *trains, int echange, int nbtrains)
{
	char reponse[10];
	sprintf(reponse, "%d\n", nbtrains);
	write(echange, reponse, sizeof(reponse));
	while (trains->suivant != NULL)
	{
		char chaine[MAX];
		double prixReduc = getReduc(trains->leTrain);

		sprintf(chaine, "%d;%s;%s;%d:%d;%d:%d;%f;",
				trains->leTrain.numero, trains->leTrain.villeDepart, trains->leTrain.villeArrivee,
				trains->leTrain.heureDep.heure, trains->leTrain.heureDep.minute,
				trains->leTrain.heureArr.heure, trains->leTrain.heureArr.minute,
				prixReduc);
		trains = trains->suivant;
		printf("%s\n", chaine);
		write(echange, chaine, sizeof(chaine));
	}
}

Cellule *recupTrain(int nbTrains, int connection, Cellule **trains)
{
	int i = 0;

	while (i < nbTrains)
	{
		Train *t = malloc(sizeof(Train));
		char tampon[MAX];

		char h1[5];
		char h2[5];
		char numero[MAX];
		char vDep[MAX];
		char vAr[MAX];
		char prix[MAX];

		char *ptr;

		int nbLus = read(connection, tampon, MAX);
		sscanf(tampon, "%[^;\n];%[^;\n];%[^;\n];%[^;\n];%[^;\n];%[^;\n]",
			   numero, vDep, vAr, h1, h2, prix);
		t->villeDepart = vDep;
		t->villeArrivee = vAr;
		t->numero = atoi(numero);
		t->prix = strtod(prix, &ptr);
		t->heureArr = stringToTemps(h2);
		t->heureDep = stringToTemps(h1);

		inserTete(trains, *t);
		free(t);
		i++;
		free(t);
	}
	return *trains;
}

void sendRequete(int *connections, int nbserv)
{
	char reponse[MAX];
	char hor[MAX];
	char req[MAX];
	char dep[MAX];
	char arr[MAX];

	printf("Recherche avec ville seul(TV), avec une horraire (TH), avec deux horraires(TB) ? ");
	fscanf(stdin, "%s", req);
	printf("Ville de départ : ");
	fscanf(stdin, "%s;", dep);
	printf("Ville d'arrivé : ");
	fscanf(stdin, "%s", arr);
	if (strcmp(req, "TV") == 0)
	{
		sprintf(reponse, "%s;%s;%s\n", req, dep, arr);
	}
	if (strcmp(req, "TH") == 0)
	{
		printf("Horraire (XX:XX): ");
		fscanf(stdin, "%s", hor);
		sprintf(reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
	if (strcmp(req, "TB") == 0)
	{
		printf("Horraire (XX:XX-XX:XX) : ");
		fscanf(stdin, "%s", hor);
		sprintf(reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}

	for (int i = 0; i < nbserv; i++)
	{
		write(connections[i], reponse, strlen(reponse) + 1);
	}
}

void getRequete(int *connections, int nbserv)
{
	int nbLus;
	Cellule *trains = {0};

	int nbtrainsAdd = 0;

	for (int i = 0; i < nbserv; i++)
	{
		char tampon[MAX];
		nbLus = read(connections[i], tampon, MAX);
		int nbTrains = atoi(tampon);
		nbtrainsAdd +=nbTrains;

		printf("nb train i: %d \n", nbTrains);
		trains = recupTrain(nbTrains, connections[i], &trains);


	}
	printf("tous les trains son recupere\n");

	if(nbserv==1){
		afficherTrains("Le train le plus rapide est", trains, nbtrainsAdd);
	}

	if (nbtrainsAdd > 1)
	{
		char ans[MAX];
		printf("Voulez vous le trajet le plus rapide(R) ? Le moins cher(P) ? Q pour quitter\n");
		fscanf(stdin, "%s", ans);
		if (strcmp(ans, "R") == 0)
		{
			Cellule *tMin = malloc(sizeof(Cellule));
			Train t1 = trierParTemps(trains, nbtrainsAdd);
			inserTete(&tMin, t1);
			afficherTrains("Le train le plus rapide est", tMin, 1);
		}
		if (strcmp(ans, "P") == 0)
		{
			Train t1 = trierParPrix(trains, nbtrainsAdd);
			Cellule *tMinprix = malloc(sizeof(Cellule));
			inserTete(&tMinprix, t1);
			afficherTrains("Le train le moins cher est", tMinprix, 1);
		}
		if (strcmp(ans, "Q") == 0)
		{
			//close(connection);
			exit(EXIT_SUCCESS);
		}
	}
}