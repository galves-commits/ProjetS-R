struct temps
{
    int heure;
    int minute;
};

struct train
{
    int numero;
    char *villeDepart;
    char *villeArrivee;
    struct temps heureDebut;
    struct temps heureFin;
    int prix;
    char *reduc;
};

void traiterFic();
void echangeClient(int, int);
void requeteTrajet(int, int);
char **getTrain(char, char, char);