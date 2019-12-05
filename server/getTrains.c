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

Cellule * getTrains(char* protocol, char *depart, char *arrivee, char *heure, Cellule *trains, int* nbtrains)
{
    printf("Requete : %s : %s : %s\n",protocol, depart, arrivee);

    if(strcmp(protocol,"TV")==0){
        return getTVTrains(depart,arrivee,trains,nbtrains);
    }
    if(strcmp(protocol,"TH")==0){
        return getTHTrains(depart,arrivee,heure,trains,nbtrains);
    }
    if(strcmp(protocol,"TB")==0){
        return getTBTrains(depart,arrivee,heure,trains,nbtrains);
    }
}

Cellule * getTVTrains(char *depart, char *arrivee, Cellule *trains,int *nbtrains){
    Cellule *t = trains;
    int i = 0;
    Cellule *bonTrains = malloc(sizeof(Cellule));
    while (t->suivant != NULL)
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0)
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0)
            {               
            inserTete(&bonTrains, t->leTrain); 
            i++;                  
            }
        }
        t = t->suivant;
    }
    free(t);
    *nbtrains = i;
    return bonTrains;

}

Cellule * getTHTrains(char *depart, char *arrivee, char* heure ,Cellule *trains, int* nbtrains){
    Cellule *t = trains;
    Cellule *bonTrains = malloc(sizeof(Cellule));
    int i = 0;
    Temps hDebReq = stringToTemps(heure);
    int nonTrouve = 1;
    while ((t->suivant != NULL)&&(nonTrouve))
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0)
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0)
            {
                if (superieur(t->leTrain.heureDep, hDebReq))
                {
                    inserTete(&bonTrains, t->leTrain);
                    i++;
                    nonTrouve=0;
                }
            }
        }
        t = t->suivant;
    }
    free(t);
    *nbtrains = i;
    return bonTrains;
}

Cellule * getTBTrains(char *depart, char *arrivee, char* heure ,Cellule *trains, int *nbtrains){
    Cellule *t = trains;
    Cellule *bonTrains = malloc(sizeof(Cellule));
    int i = 0;
    char h1[MAX];
    char h2[MAX];
    sscanf(heure,"%[^-]-%[^-]",h1,h2);
    printf("'%s' '%s'\n",h1,h2);
    Temps *hDebReq = malloc(sizeof(Temps)); 
    *hDebReq = stringToTemps(h1);
    Temps *hFinReq = malloc(sizeof(Temps));  
    *hFinReq = stringToTemps(h2);
    printf("Bob\n");

    while (t->suivant != NULL)
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0)
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0)
            {
                if (superieur(t->leTrain.heureDep, *hDebReq))
                {
                    if ((inferieur(t->leTrain.heureDep, *hFinReq)))
                    {
                        inserTete(&bonTrains, t->leTrain);
                        i++;
                    }
                }
            }
        }
        t = t->suivant;
    }
    free(t);
    free(hDebReq);
    free(hFinReq);
    *nbtrains = i;
    return bonTrains;
}

Temps dureeVoy(Train t)
{
    int tDepMin = t.heureDep.heure * 60 + t.heureDep.minute;
    int tArrMin = t.heureArr.heure * 60 + t.heureArr.minute;
    int durre = tArrMin - tDepMin;
    Temps *dure = malloc(sizeof(Temps));
    dure->heure = durre / 60;
    dure->minute = durre % 60;
    return *dure;
}

double getReduc(Train t){
    double prix = t.prix;

    if(t.reduc!=NULL){
        if(strcmp(t.reduc,"REDUC")==0){
            prix = t.prix * 0.8;
        }
        if(strcmp(t.reduc,"SUPPL")==0){
            prix = t.prix * 1.1;
        }
    }
    return prix;
}

Train trierParPrix(Cellule *trains,int nbtrains)
{
	Train tMin = trains->leTrain;
	Cellule *t = trains;
    int i = 0;
	while (i<nbtrains)
	{
		if (t->leTrain.prix < tMin.prix)
		{
			tMin = t->leTrain;
		}
		t = t->suivant;
        i++;
	}
	return tMin;
}


Train trierParTemps(Cellule *trains,int nbtrains)
{
	Train tMin = trains->leTrain;
	Cellule *t = trains;
    int i = 0;
	while (i< nbtrains)
	{

		if (inferieur(dureeVoy(t->leTrain), dureeVoy(tMin)))
		{
			tMin = t->leTrain;
		}
		t = t->suivant;
        i++;
	}
	return tMin;
}


void afficherTrains(char* message,Cellule *trains,int nbtrains)
{
	Cellule *t = trains;
    int i = 0;

	printf( "%s : \n  N \t\tDepart \t\t   Arrivee\t\tHeure D \t Heure A \t  Prix\n",message);

	while (i<nbtrains)
	{
		printf("%d %20s  %20s \t %02d:%02d \t\t  %02d:%02d \t %3.2f€\n",
				t->leTrain.numero, t->leTrain.villeDepart, t->leTrain.villeArrivee,
				t->leTrain.heureDep.heure, t->leTrain.heureDep.minute,
				t->leTrain.heureArr.heure, t->leTrain.heureArr.minute,
				t->leTrain.prix);
		t = t->suivant;
        i++;
	}
    free(t);
}
