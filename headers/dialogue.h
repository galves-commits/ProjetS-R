#define PORT 7770

void reponseRequete(Cellule *, int, int);
void sendRequete(char **);
Cellule *getRequete(int, Cellule **, int *);
void printRequete(int nbserv, Cellule * trains, int nbtrains);
Cellule * recupTrain(int , int , Cellule **);
void admin();
void statServer();