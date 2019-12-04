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

Cellule *getTrains(char* protocol, char *depart, char *arrivee, char *heure, Cellule *trains)
{
    int borne;
    Temps *hDebReq = malloc(sizeof(Temps));
    Temps *hFinReq = malloc(sizeof(Temps));
    printf("Requete : %s : %s\n", depart, arrivee);
    
    if (strcmp(heure, "\n") == 0)
    {
        borne = 2;
    }
    else
    {   
        char * s = strchr(heure, '\n');
        if (s != 0)
        {
            *s = '\0';
        }

        char *p = strtok(heure, ";");
        
        borne = 1;
        *hDebReq = stringToTemps(p);
        p = strtok(NULL, ";");
        if (p != NULL)
        {
            *hFinReq = stringToTemps(p);
            borne = 0;
        }
    }

    Cellule *t = trains;
    Cellule *bonTrains = malloc(sizeof(Cellule));
    int i = 0;
    while (t->suivant != NULL)
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0)
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0)
            {
                if ((borne == 2) || superieur(t->leTrain.heureDep, *hDebReq))
                {
                    if ((borne > 0) || (inferieur(t->leTrain.heureDep, *hFinReq)))
                    {
                        inserTete(&bonTrains, t->leTrain);
                    }
                }
            }
        }
        i++;
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
