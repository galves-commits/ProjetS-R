#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/trains.h"
#include "../headers/server.h"
#include "../headers/dialogue.h"
Cellule *getTrains(char *protocol, char *depart, char *arrivee, char *heure, Cellule *trains, int *nbtrains)
{
    printf("Requete : %s : %s : %s\n", protocol, depart, arrivee);
    FILE *fil = fopen("stat.txt", "a");                              //ouverture de stat.txt en mode ecriture a la fin du fichier
    fprintf(fil, "%s-%s-%s-%s\n", protocol, depart, arrivee, heure); // on met la requete du client sous cette forme dans le fichier
    fclose(fil);                                                     // fermeture du fichier

    // celon le protocole choisis on execute un get*Trains
    if (strcmp(protocol, "TV") == 0)
    {
        return getTVTrains(depart, arrivee, trains, nbtrains);
    }
    if (strcmp(protocol, "TH") == 0)
    {
        return getTHTrains(depart, arrivee, heure, trains, nbtrains);
    }
    if (strcmp(protocol, "TB") == 0)
    {
        return getTBTrains(depart, arrivee, heure, trains, nbtrains);
    }
}
// renvoie une liste chainé des trains satisfaisant les critères du client
Cellule *getTVTrains(char *depart, char *arrivee, Cellule *trains, int *nbtrains)
{
    Cellule *t = trains; // liste chainé des trains en parametre
    int i = 0;
    Cellule *bonTrains = malloc(sizeof(Cellule)); // liste chainé des bons trains a renvoyer
    while (t->suivant != NULL)                    // tant qu'on est pas a la fin de la liste chainé
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0) // on regarde si le train a la même ville de depart que la requete
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0) // on regarde si le train a la même ville d'arrivée que la requete
            {
                inserTete(&bonTrains, t->leTrain); // alors on insere le train dans la liste des bons trains
                i++;                               // on incremente le compteur de bon trains
            }
        }
        t = t->suivant; // on passe au train suivant
    }
    free(t);
    *nbtrains = i;    // nombre de trains
    return bonTrains; // retourne la liste des trains satisfaisant la requete
}
// renvoie le train qui a l'heure de depart la plus proche de la requete
Cellule *getTHTrains(char *depart, char *arrivee, char *heure, Cellule *trains, int *nbtrains)
{
    Cellule *t = trains;                          // liste chainé des trains en parametre
    Cellule *bonTrains = malloc(sizeof(Cellule)); // liste chainé des bons trains a renvoyer
    int i = 0;
    Temps hDebReq = stringToTemps(heure);
    while ((t->suivant != NULL))
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0) // on regarde si le train a la même ville de depart que la requete
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0) // on regarde si le train a la même ville d'arrivée que la requete
            {
                if (superieur(t->leTrain.heureDep, hDebReq)) //grace a la methode superieur on regarde si l'heure du train est superieur a l'heure de la requete
                {
                    inserTete(&bonTrains, t->leTrain);
                    i++;
                }
            }
        }
        t = t->suivant; //on passe au train suivant
    }
    free(t);
    Cellule *retour = malloc(sizeof(Cellule));
    if (i == 0)
    {
        *nbtrains = 0;
    }
    else
    {
        inserTete(&retour, trierDepart(bonTrains, i));
        *nbtrains = 1;
    }
    return retour;
}
// renvoie les trains qui satisfont la requête et qui sont entre les deux horraires donné par le client
Cellule *getTBTrains(char *depart, char *arrivee, char *heure, Cellule *trains, int *nbtrains)
{
    Cellule *t = trains;                          // liste chainé des trains en parametre
    Cellule *bonTrains = malloc(sizeof(Cellule)); // liste chainé des bons trains a renvoyer
    int i = 0;
    char h1[MAX];
    char h2[MAX];
    sscanf(heure, "%[^-]-%[^-]", h1, h2); //recupere la plage horraire du client et la sépart en 2horraire
    printf("'%s' '%s'\n", h1, h2);
    Temps *hDebReq = malloc(sizeof(Temps));
    *hDebReq = stringToTemps(h1); // convertit la première horraire en structure Temps
    Temps *hFinReq = malloc(sizeof(Temps));
    *hFinReq = stringToTemps(h2); // convertit la seconde horraire en structure Temps

    while (t->suivant != NULL) // tant qu'on est pas a la fin de la liste chainé
    {
        if (strcmp(t->leTrain.villeDepart, depart) == 0) // on regarde si le train a la même ville de depart que la requete
        {
            if (strcmp(t->leTrain.villeArrivee, arrivee) == 0) // on regarde si le train a la même ville d'arrivée que la requete
            {
                if (superieur(t->leTrain.heureDep, *hDebReq)) //grace a la methode superieur on regarde si l'heure du train est superieur a la premiere horraire
                {
                    if ((inferieur(t->leTrain.heureDep, *hFinReq))) //grace a la methode superieur on regarde si l'heure du train est inferier a la seconde horraire
                    {
                        inserTete(&bonTrains, t->leTrain); // alors on insere le train dans la liste des bons trains
                        i++;                               // on incremente le compteur de bon trains
                    }
                }
            }
        }
        t = t->suivant; //on passe au train suivant
    }
    free(t);
    free(hDebReq);
    free(hFinReq);
    *nbtrains = i;    // nombre de trains
    return bonTrains; // retourne la liste des trains satisfaisant la requete
}

// permet de calculer la durée d'un voyage
Temps dureeVoy(Train t)
{
    int tDepMin = t.heureDep.heure * 60 + t.heureDep.minute; // on met l'horraire de depart en minute
    int tArrMin = t.heureArr.heure * 60 + t.heureArr.minute; // on met l'horraire d'arrive en minute
    int durre = tArrMin - tDepMin;
    Temps *dure = malloc(sizeof(Temps)); // on creer une structure Temps pour ranger la durée dedans
    dure->heure = durre / 60;
    dure->minute = durre % 60;
    return *dure;
}
// permet de renvoyer le prix d'un voyage après réduction
double getReduc(Train t)
{
    double prix = t.prix;

    if (t.reduc != NULL)
    {
        if (strcmp(t.reduc, "REDUC") == 0)
        {
            prix = t.prix * 0.8; // en cas de reduc on biasse le prix de 20%
        }
        if (strcmp(t.reduc, "SUPPL") == 0)
        {
            prix = t.prix * 1.1; // en cas de suppl on augmente le prix de 20%
        }
    }
    return prix;
}
// permet de trier une liste chainée de train par prix
Train trierParPrix(Cellule *trains, int nbtrains)
{
    Train tMin = trains->leTrain; // on met le premier train
    Cellule *t = trains;
    int i = 0;
    while (i < nbtrains) // tant qu'on a pas traité tout les trains
    {
        if (t->leTrain.prix < tMin.prix) // on compare le prix du train actuel avec le train le moins cher sur le moment
        {
            tMin = t->leTrain;
        }
        t = t->suivant;
        i++;
    }
    return tMin;
}

// permet de trier une liste chainée de train par temps
Train trierParTemps(Cellule *trains, int nbtrains)
{
    Train tMin = trains->leTrain; // on met le premier train
    Cellule *t = trains;
    int i = 0;
    while (i < nbtrains) // tant qu'on a pas traité tout les trains
    {

        if (inferieur(dureeVoy(t->leTrain), dureeVoy(tMin))) // on compare la durée de voyage du train actuel avec le train le plus rapide sur le moment
        {
            tMin = t->leTrain;
        }
        t = t->suivant;
        i++;
    }
    return tMin;
}

Train trierDepart(Cellule *trains, int nbtrains)
{
    Cellule *minDepart = malloc(sizeof(Cellule));
    Train tMin = trains->leTrain;
    Cellule *t = trains;
    int i = 0;
    int dep;
    while (i < nbtrains)
    {
        if (inferieur(t->leTrain.heureDep, tMin.heureDep))
        {
            tMin = t->leTrain;
        }
        t = t->suivant;
        i++;
    }
    return tMin;
}

void afficherTrains(char *message, Cellule *trains, int nbtrains)
{
    Cellule *t = trains;
    int i = 0;

    printf("%s : \n", message); // permet d'afficher le message avec lequel est appelé la fonction

    while (i < nbtrains) //tant qu'on a pas affiché tout les trains
    {
        printf(PURPLE "Train n° : %d | Départ (%02d:%02d) : %s | Arrivé (%02d:%02d) : %s | Prix : %3.2f€\n" DEFAULT,
               t->leTrain.numero, t->leTrain.heureDep.heure, t->leTrain.heureDep.minute, t->leTrain.villeDepart,
               t->leTrain.heureArr.heure, t->leTrain.heureArr.minute, t->leTrain.villeArrivee, t->leTrain.prix);
        t = t->suivant;
        i++; // on incremente i après l'affichage d'un train
    }
    free(t);
}
