#define MAX 40000

void inserTete(Cellule** , Train);

Cellule * traiterFic();
Cellule * remplirStruc(char **tab, Cellule *trains);

void requeteTrajet(int, Cellule*);
void echangeClient(int, Cellule*);
void reponseRequete(Cellule *, int, int);

