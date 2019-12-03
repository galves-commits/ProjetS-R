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
    int prix;
    char *reduc;
}Train;

typedef struct cellule{
    Train leTrain;
    struct cellule* suivant;
}Cellule;
typedef Cellule * PCellule;
//inserTete(&(dernier->suivant), train)

bool inferieur(Temps t1, Temps t2);
bool superieur(Temps t1, Temps t2);

void inserTete(Cellule** , Train);

void traiterFic();

void echangeClient(int, int);
void requeteTrajet(int, int);
Cellule * getTrains(char* , char* , char* , Cellule* );
Temps stringToTemps(char*);
