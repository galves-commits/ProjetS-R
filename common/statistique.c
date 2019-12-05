#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MaxStr 100
#define NBMax 1000
#define NBMaxTrain 100
#define MaxVille 100

#include "../headers/trains.h"
#include "../headers/server.h"
#include "../headers/dialogue.h"
#include "../headers/statistique.h"

void admin(){
	char reponse[10];
	printf("Accès Administration(A) ou Client(C) ?\n");
	fscanf(stdin, "%s", reponse);
	if (strcmp(reponse, "A") == 0)
	{
		printf("Veuillez indiquer le mot de passe\n");
		fscanf(stdin, "%s", reponse);
		if (strcmp(reponse, "admin") == 0){
			statServer();
		}
        exit(EXIT_SUCCESS);
	}
}
void statServer(){
    char str[MaxStr];
    char *s;
    char stat[MaxStr][NBMaxTrain];
    FILE *file = fopen("../server/stat.txt", "r");
    int i = 0;
    CelluleT *req = malloc(sizeof(CelluleT));
    if (file != NULL)
	{       

		while (fgets(str, NBMax, file) != NULL)
		{
			s = strchr(str, '\n');
			if (s != 0)
			{
				*s = '\0';
			}
			strcpy(stat[i], str);
			i++;
            
		}
		fclose(file);
		int a = 0;

		while (a < i)
		{
			int y = 1;
			char *tabTrain[MaxVille][MaxStr];
			char *p;
			p = strtok(stat[a], "-");
			tabTrain[a][0] = p;
			while (y < 100 && tabTrain[a][y - 1] != NULL)
			{
				p = strtok(NULL, "-"); 
				tabTrain[a][y] = p;
				y++;
			}
            req = remplirStat(tabTrain[a], req);
            
			a++;
		}
	}
}
CelluleT * remplirStat(char **tab, CelluleT *requete)
{

	Stat *s = malloc(sizeof(Stat));;
	s->proto = tab[0];
    printf("%s\n",s->proto);
	s->villeDepart = tab[1];
	s->villeArrivee = tab[2];
	s->h1 = stringToTemps(tab[3]);
	s->h2 = stringToTemps(tab[4]);
	inserTete2(&requete, *s);
	free(s);
	return requete;
}

void inserTete2(CelluleT ** tete, Stat t){
    CelluleT *nouv = malloc(sizeof(CelluleT));
    nouv->laStat;
    nouv->suivant = *tete;
    *tete = nouv;
}