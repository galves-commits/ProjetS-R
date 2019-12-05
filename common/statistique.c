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
             CelluleT *req = malloc(sizeof(CelluleT));
			req=statServer();
            afficherStat(req);
		}
        exit(EXIT_SUCCESS);
	}
}
CelluleT * statServer(){
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
    return req;
}
CelluleT * remplirStat(char **tab, CelluleT *requete)
{
	Stat *s = malloc(sizeof(Stat));
	s->proto = tab[0];
	s->villeDepart = tab[1];
	s->villeArrivee = tab[2];
    if(( strcmp(s->proto,"TH")==0 )||(strcmp(s->proto,"TB")==0)){
	s->h1 = stringToTemps(tab[3]);
    }
    if(strcmp(s->proto,"TB")==0){
	s->h2 = stringToTemps(tab[4]);
    }
	inserTete2(&requete, *s);
	free(s);
	return requete;
}

void inserTete2(CelluleT ** tete, Stat t){
    CelluleT *nouv = malloc(sizeof(CelluleT));
    nouv->laStat=t;
    nouv->suivant = *tete;
    *tete = nouv;
}
void afficherStat(CelluleT *stat)
{
	CelluleT *t = stat;
	while (t->suivant!=NULL)
	{
        if(strcmp(t->laStat.proto,"TV")==0){
             printf("%s \t %s \t %s \n",
				t->laStat.proto,t->laStat.villeDepart,t->laStat.villeArrivee);
        }
        if(strcmp(t->laStat.proto,"TH")==0){
            printf("%s \t %s \t %s \t %02d:%02d \n",
				t->laStat.proto,t->laStat.villeDepart,t->laStat.villeArrivee,t->laStat.h1.heure,t->laStat.h1.minute);
        }
        if(strcmp(t->laStat.proto,"TB")==0){
		printf("%s \t %s \t %s \t %02d:%02d \t\t  %02d:%02d \n",
				t->laStat.proto,t->laStat.villeDepart,t->laStat.villeArrivee,t->laStat.h1.heure,t->laStat.h1.minute,t->laStat.h2.heure,t->laStat.h2.minute);
        }       
        t = t->suivant;
	}
    free(t);
}
