#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../headers/trains.h"
#include "../headers/server.h"
#include "../headers/dialogue.h"


#define NBMaxTrain 100
#define NBMax 1000
#define MaxStr 100
#define MaxVille 100

// traiterFic permet de traiter le fichier passer en param et stocke les elements dans une liste chainée
Cellule * traiterFic(char* t)
{
	char ligneActuel[NBMaxTrain][NBMax];
	char *s;
	int i = 0;
	Cellule *trains = malloc(sizeof(Cellule)); // cellule train
	char str[MaxStr];
	FILE *file = fopen(t, "r"); // ouverture du fichier t
	if(file == NULL){
		printf(RED"ERREUR : fichier invalide\n"DEFAULT);
		exit(EXIT_SUCCESS);
	}
	else
	{
		while (fgets(str, NBMax, file) != NULL)// recupere ligne par ligne et sotck dans str
		{
			s = strchr(str, '\n');//strchr chervher l'occurence de retour a la ligne
			if (s != 0)
			{
				*s = '\0';
			}
			strcpy(ligneActuel[i], str);// copie de la ligne dans ligneActuel
			i++;
		}
		fclose(file); //fermeture fichier texte

		int a = 0;

		while (a < i)
		{
			int y = 1;
			char *tabTrain[MaxVille][MaxStr];
			char *p;
			p = strtok(ligneActuel[a], ";"); // p prend une ligne stocke dans ligneActuel et la decompose au ; 
			tabTrain[a][0] = p;
			while (y < 100 && tabTrain[a][y - 1] != NULL)
			{
				p = strtok(NULL, ";");
				tabTrain[a][y] = p;
				y++;
			}
			trains = remplirStruc(tabTrain[a], trains); // remplis la structure train
			a++;
		}
	}
	return trains;
}

// remplirStruc rempli la strucutre Train
Cellule * remplirStruc(char **tab, Cellule *trains)
{
	char *ptr;
	double ret;
	Train *t = malloc(sizeof(Train));;
	t->numero = atoi(tab[0]); // tab[0] est un string donc utilisation fct atoi pour le mettre en int
	t->villeDepart = tab[1]; // tab[1] contient la ville de Depart 
	t->villeArrivee = tab[2]; // tab[2] contient la ville d'arrivée 
	t->heureDep = stringToTemps(tab[3]); // tab[3] est un string. stringtoTemps renvoi un Temps
	t->heureArr = stringToTemps(tab[4]); // de meme que tab[3]
	ret = strtod(tab[5], &ptr); //tab[5] contient un string donc strtod le met en double 
	t->prix = ret;// prix prend un double
	t->reduc = tab[6];
	inserTete(&trains, *t); // insere la structure Train
	free(t);
	return trains;
}
