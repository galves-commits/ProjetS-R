typedef struct Stat
{
    char *proto;
    char *villeDepart;
    char *villeArrivee;
    Temps h1;
    Temps h2;

}Stat;

typedef struct celluleT{
    Stat laStat;
    struct celluleT* suivant;
}CelluleT;



CelluleT * remplirStat(char **tab, CelluleT *requete);
CelluleT * statServer();
void admin();
void inserTete2(CelluleT ** tete, Stat t);
void afficherStat(CelluleT *trains);
void comparaison(CelluleT *stat) ; 
double moyenne(int a , int b ) ;
int max(int a , int b , int c ) ;

