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
    struct temps heureDep;
    struct temps heureArr;
    int prix;
    char *reduc;
};

void traiterFic();
void echangeClient(int, int);
void requeteTrajet(int, int);
struct train * getTrains(char* , char* , char* , struct train* );
struct temps stringToTemps(char*);
