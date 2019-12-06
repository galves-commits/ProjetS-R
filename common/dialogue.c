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
	//envois d'un paquet contenant le nombre de trains
	char reponse[10];
	sprintf(reponse, "%d\n", nbtrains);
	write(echange, reponse, sizeof(reponse));

	//envois des resultats de la requete un à un
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
		write(echange, chaine, sizeof(chaine));
	}
}

Cellule *recupTrain(int nbTrains, int connection, Cellule **trains)
{
	int i = 0;

	//On passe dans la boucle autant de fois qu'il y a de trains
	for (int i = 0; i < nbTrains; i++)
	{
		Train *t = malloc(sizeof(Train));
		char tampon[MAX];
		char h1[MAX];
		char h2[MAX];
		char numero[MAX];
		char vDep[MAX];
		char vAr[MAX];
		char prix[MAX];
		char *ptr = malloc(sizeof(char));

		//recuperation du paquet et decomposition du trains
		int nbLus = read(connection, tampon, MAX);
		sscanf(tampon, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;\n]",
			   numero, vDep, vAr, h1, h2, prix);
		//contruction de la structure train
		t->villeDepart = vDep;
		t->villeArrivee = vAr;
		t->numero = atoi(numero);
		t->prix = strtod(prix, &ptr);
		t->heureArr = stringToTemps(h2);
		t->heureDep = stringToTemps(h1);

		//insertion du train dans la chaine
		inserTete(trains, *t);
	}
}
// permet de stocker la requête du client dans reponse
void makeRequete(char **reponse)
{
	char hor[MAX];
	char req[MAX];
	char dep[MAX];
	char arr[MAX];
	char rep[MAX];

	printf(PURPLE "Recherche avec ville seul(" YELLOW "TV" PURPLE "), avec une horraire (" YELLOW "TH" PURPLE "), avec deux horraires(" YELLOW "TB" PURPLE ") ? ");
	printf(YELLOW);
	fscanf(stdin, "%s", req);// on recupere dans req le protocol
	while ((strcmp(req, "TV") != 0) && (strcmp(req, "TH") != 0) && (strcmp(req, "TB") != 0)) //si le client se trompe on redemande
	{
		printf(RED "Veuillez écrire un protocol correct\n");
		printf(PURPLE "Recherche avec villes seuls(" YELLOW "TV" PURPLE "), avec une horraire (" YELLOW "TH" PURPLE "), avec deux horraires(" YELLOW "TB" PURPLE ") ? ");
		printf(YELLOW);
		fscanf(stdin, "%s", req);
	}
	printf(PURPLE "Ville de départ : ");
	printf(DEFAULT);

	setbuf(stdin, NULL);
	fgets(dep, sizeof(dep), stdin);// on recupere dans dep la ville de depart
	char *s = strchr(dep, '\n'); //strchr chercher l'occurence de retour a la ligne
	if (s != 0)
	{
		*s = '\0';
	}
	while ((strcmp(dep, "Valence") != 0) && (strcmp(dep, "Grenoble") != 0) && (strcmp(dep, "Montelimar") != 0) && (strcmp(dep, "Paris Gare de Lyon") != 0))
	{
		printf("Veuillez vérifier l'orthographe, nous ne prenons pas ne charge cette ville\n");//si le client se trompe on redemande
		printf(PURPLE "Ville de départ : ");
		printf(DEFAULT);
		fscanf(stdin, "%s;", dep);
	}
	printf(PURPLE "Ville d'arrivé : ");
	printf(DEFAULT);

	setbuf(stdin, NULL);
	fgets(arr, sizeof(arr), stdin);// on recupere dans arr la ville d'arrivée
	s = strchr(arr, '\n'); //strchr chervher l'occurence de retour a la ligne
	if (s != 0)
	{
		*s = '\0';
	}

	while ((strcmp(arr, "Valence") != 0) && (strcmp(arr, "Grenoble") != 0) && (strcmp(arr, "Montelimar") != 0) && (strcmp(arr, "Paris Gare de Lyon") != 0))
	{
		printf("Veuillez vérifier l'orthographe, nous ne prenons pas ne charge cette ville\n");//si le client se trompe on redemande
		printf(PURPLE "Ville d'arrivé : ");
		printf(DEFAULT);
		fscanf(stdin, "%s;", arr);
	}
	if (strcmp(arr, dep) == 0)
	{//si le client se trompe et dit la même ville de depart et d'arrivé on quitte le programme

		printf(RED "Vous avez indiqué la même ville de départ et d'arrivé veuillez recommencer\n" DEFAULT);
		exit(EXIT_SUCCESS);
	}
	if (strcmp(req, "TV") == 0)// si le protocol est TV on met dans reponse la requete sous la forme req;dep;arr
	{
		sprintf(*reponse, "%s;%s;%s\n", req, dep, arr);
	}
	if (strcmp(req, "TH") == 0)// si le protocol est TH on met dans reponse la requete sous la forme req;dep;arr;horr
	{
		printf(PURPLE "Horraire (XX:XX): ");
		printf(DEFAULT);
		fscanf(stdin, "%s", hor);
		sprintf(*reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
	if (strcmp(req, "TB") == 0)// si le protocol est TH on met dans reponse la requete sous la forme req;dep;arr;horr
	{
		printf(PURPLE "Horraire (XX:XX-XX:XX) : ");
		printf(DEFAULT);
		fscanf(stdin, "%s", hor);
		sprintf(*reponse, "%s;%s;%s;%s\n", req, dep, arr, hor);
	}
}

Cellule *getRequete(int connection, Cellule **trains, int *nbtrains)
{
	int nbLus;
	int nbtrainsAdd = 0;
	char tampon[MAX];
	//recuperation du nombre de trains
	nbLus = read(connection, tampon, MAX);
	*nbtrains = atoi(tampon);

	//cas ou le mauvais protocole a été selectionné
	if (*nbtrains == -1)
	{
		printf(RED "Erreur : protocole invalide\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		//recuperation des trains
		recupTrain(*nbtrains, connection, trains);
	}
}
// regle les differents cas d'affichage de la requete
void printRequete(int nbserv, Cellule *trains, int nbtrains)
{
	switch (nbtrains)
	{

	case 0:	// cas ou il n'y a pas de train
		printf(CYAN "Il n'y a aucun train qui repond à vos attentes\n" DEFAULT);
		break;

	case 1: // cas ou il y a un train
		afficherTrains(CYAN "Seulement 1 train disponible", trains, nbtrains);
		break;

	default:
		if (nbserv == 1)
		{	// cas ou il y a plusieurs trains et 1serveur
			afficherTrains(CYAN "Voici Les trains disponibles ", trains, nbtrains);
		}
		else
		{	// cas ou il y a plusieurs trains et  plusieurs serveur
			printf(CYAN "Il y a %d trains disponible.\n" DEFAULT, nbtrains);
		}

		if (nbtrains > 1)
		{
			char ans[MAX];
			printf(CYAN "Voulez vous le trajet le plus rapide(" YELLOW "R" CYAN ") ? Le moins cher(" YELLOW "P" CYAN ") ? " YELLOW "Q" CYAN " pour quitter\n");
			printf(YELLOW);
			fscanf(stdin, "%s", ans);// on recupere la réponse du client dans ans
			while ((strcmp(ans, "R") != 0) && (strcmp(ans, "P") != 0) && (strcmp(ans, "Q") != 0))
			{	//si il s'est trompé on lui demande de reecrire
				printf(RED "Veuillez rentrer un caractère correcte\n");
				printf(CYAN "Voulez vous le trajet le plus rapide(" YELLOW "R" CYAN ") ? Le moins cher(" YELLOW "P" CYAN ") ? " YELLOW "Q" CYAN " pour quitter\n");
				printf(YELLOW);
				fscanf(stdin, "%s", ans);
			}

			if (strcmp(ans, "R") == 0) // cas ou il entre R
			{	
				Cellule *tMin = malloc(sizeof(Cellule));
				Train t1 = trierParTemps(trains, nbtrains); // trie par temps les trains
				inserTete(&tMin, t1);						// on met le train dans une nouvelle liste chainée
				afficherTrains(CYAN "Le train le plus rapide est", tMin, 1); // on affiche le train
			}
			if (strcmp(ans, "P") == 0)// cas ou il entre P
			{
				Train t1 = trierParPrix(trains, nbtrains);// trie par prix les trains
				Cellule *tMinprix = malloc(sizeof(Cellule));
				inserTete(&tMinprix, t1);				// on met le train dans une nouvelle liste chainée
				afficherTrains(YELLOW "Le train le moins cher est", tMinprix, 1);// on affiche le train
			}
			if (strcmp(ans, "Q") == 0)// cas ou il entre q
			{
				exit(EXIT_SUCCESS);//on arrête le programme
			}
		}
	}
}