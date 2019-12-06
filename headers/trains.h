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

void inserTete(Cellule** , Train);

Train trierParTemps(Cellule *,int nbtrains);
Train trierParPrix(Cellule *,int nbtrains);
Train trierDepart(Cellule *trains,int nbtrains);
Temps stringToTemps(char*);
Temps dureeVoy(Train );
Cellule * getTrains(char *,char* , char* , char* , Cellule*,int* );
Cellule * getTBTrains(char *, char *, char* ,Cellule *, int*);
Cellule * getTHTrains(char *, char *, char* ,Cellule *, int*);
Cellule * getTVTrains(char *, char *, Cellule *, int*);
void afficherTrains(char*,Cellule *trains,int nbtrains);
double getReduc(Train t);
int inferieur(Temps t1, Temps t2);
int superieur(Temps t1, Temps t2);