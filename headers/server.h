#include <stdlib.h>
#include <stdbool.h>

typedef struct temps
{
    int heure;
    int minute;
}Temps;

typedef struct train
{
    int numero;
    char *villeDepart;
    char *villeArrivee;
    Temps heureDep;
    Temps heureArr;
    double prix;
    char *reduc;
}Train;

typedef struct cellule{
    Train leTrain;
    struct cellule* suivant;
}Cellule;
typedef Cellule * PCellule;
//inserTete(&(dernier->suivant), train)

int inferieur(Temps t1, Temps t2);
int superieur(Temps t1, Temps t2);

void inserTete(Cellule** , Train);

Cellule * traiterFic();
Cellule * remplirStruc(char **tab, Cellule *trains);

void requeteTrajet(int, Cellule*);
void echangeClient(int, Cellule*);
void reponseRequete(Cellule * trains, int echange);
Cellule * getTrains(char* , char* , char* , Cellule* );
Temps stringToTemps(char*);
Temps duree(Train );
