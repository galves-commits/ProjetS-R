typedef struct Stat
{
    char *proto;
    char *villeDepart;
    char *villeArrivee;
    Temps h1;
    Temps h2;
    double prix;
    char *reduc;
}Stat;

typedef struct celluleT{
    Stat laStat;
    struct celluleT* suivant;
}CelluleT;



CelluleT * remplirStat(char **tab, CelluleT *requete);
void statServer();
void admin();
void inserTete2(CelluleT ** tete, Stat t);

