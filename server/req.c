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

#define NBMaxTrain 100
#define NBMax 1000
#define MaxStr 100
#define MaxVille 100

Cellule * traiterFic()
{
	char ligneActuel[NBMaxTrain][NBMax];
	char *s;
	int i = 0;
	Cellule *trains = malloc(sizeof(Cellule));
	char str[MaxStr];
	FILE *file = fopen("train.txt", "r");
	if (file != NULL)
	{
		while (fgets(str, NBMax, file) != NULL)
		{
			s = strchr(str, '\n');
			if (s != 0)
			{
				*s = '\0';
			}
			strcpy(ligneActuel[i], str);
			i++;
		}
		fclose(file);

		int a = 0;

		while (a < i)
		{
			int y = 1;
			char *tabTrain[MaxVille][MaxStr];
			char *p;
			p = strtok(ligneActuel[a], ";");
			tabTrain[a][0] = p;
			while (y < 100 && tabTrain[a][y - 1] != NULL)
			{
				p = strtok(NULL, ";");
				tabTrain[a][y] = p;
				y++;
			}
			trains = remplirStruc(tabTrain[a], trains);
			a++;
		}
	}
	return trains;
}

Cellule * remplirStruc(char **tab, Cellule *trains)
{
	char *ptr;
	double ret;
	Train *t = malloc(sizeof(Train));;
	t->numero = atoi(tab[0]);
	t->villeDepart = tab[1];
	t->villeArrivee = tab[2];
	t->heureDep = stringToTemps(tab[3]);
	t->heureArr = stringToTemps(tab[4]);
	ret = strtod(tab[5], &ptr);
	t->prix = ret;
	t->reduc = tab[6];
	inserTete(&trains, *t);
	return trains;
}
