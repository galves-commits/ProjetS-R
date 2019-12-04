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

Cellule * getTrains(char* protocol, char *depart, char *arrivee, char *heure, Cellule *trains)
{
    printf("Requete : %s : %s\n", depart, arrivee);

    if(strcmp(protocol,"TV")==0){
        return getTVTrains(depart,arrivee,trains);
    }
    if(strcmp(protocol,"TH")==0){
        return getTHTrains(depart,arrivee,heure,trains);
    }
    if(strcmp(protocol,"TB")==0){
        return getTBTrains(depart,arrivee,heure,trains);
    }
}

Cellule * getTVTrains(char *depart, char *arrivee, Cellule *trains){
    Cellule *t = trains;
    Cellule *bonTrains = malloc(sizeof(Cellule));
    while (t->suivant != NULL)
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0)
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0)
            {               
            inserTete(&bonTrains, t->leTrain);                   
            }
        }
        t = t->suivant;
    }
    return bonTrains;

}

Cellule * getTHTrains(char *depart, char *arrivee, char* heure ,Cellule *trains){
    Cellule *t = trains;
    Cellule *bonTrains = malloc(sizeof(Cellule));
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
                    nonTrouve=0;
                }
            }
        }
        t = t->suivant;
    }
    return bonTrains;
}

Cellule * getTBTrains(char *depart, char *arrivee, char* heure ,Cellule *trains){
    Cellule *t = trains;
    Cellule *bonTrains = malloc(sizeof(Cellule));

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
                    }
                }
            }
        }
        t = t->suivant;
    }
    return bonTrains;
}

Temps duree(Train t)
{
    int tDepMin = t.heureDep.heure * 60 + t.heureDep.minute;
    int tArrMin = t.heureArr.heure * 60 + t.heureArr.minute;
    int durre = tArrMin - tDepMin;
    Temps *dure = malloc(sizeof(Temps));
    ;
    dure->heure = durre / 60;
    dure->minute = durre % 60;
    return *dure;
}

double getReduc(Train t){
    double prix = -1;
    if(strcmp(t.reduc,"REDUC")==0){
        prix = t.prix * 0.8;
    }
    if(strcmp(t.reduc,"SUPPL")==0){
        prix = t.prix * 1.1;
    }
    return prix;
}

