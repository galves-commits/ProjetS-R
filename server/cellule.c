#include "./../headers/trains.h"
#include "./../headers/server.h"


void inserTete(Cellule ** tete, Train t){
    //creation de la nouvelle cellule
    Cellule *nouv = malloc(sizeof(Cellule));
    //on lui attribut le train
    nouv->leTrain=t;
    //l'ancienne tete de train est attribuée en tant que cellule suivante de la nouvelle cellule
    nouv->suivant = *tete;
    //la nouvelle cellule devient la tete de la chaine
    *tete = nouv;
}