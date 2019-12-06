#define PORT 7770

#define DEFAULT "\033[0m"
#define HIGHLIGHT "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK "\033[5m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define BOLDRED "\033[1;31m"
#define GREEN "\033[32m"
#define BOLDGREEN "\033[1;32m"
#define YELLOW "\033[33m"
#define BOLDYELLOW "\033[1;33m"
#define BLUE "\033[34m"
#define BOLDBLUE "\033[1;34m"
#define PURPLE "\033[35m"
#define BOLDPURPLE "\033[1;35m"
#define CYAN "\033[36m"
#define BOLDCYAN "\033[1;36m"
#define WHITE "\033[37m"

void readSTDIN(char **dep);
void reponseRequete(Cellule *, int, int);
void makeRequete(char **);
Cellule *getRequete(int, Cellule **, int *);
void printRequete(int nbserv, Cellule * trains, int nbtrains);
Cellule * recupTrain(int , int , Cellule **);
void admin();

