#include "./../headers/trains.h"
#include "./../headers/server.h"


void inserTete(Cellule ** tete, Train t){
    Cellule *nouv = malloc(sizeof(Cellule));
    nouv->leTrain=t;
    nouv->suivant = *tete;
    *tete = nouv;
}