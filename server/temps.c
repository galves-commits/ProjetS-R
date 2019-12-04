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

#define TRUE 1
#define FALSE 0


Temps stringToTemps(char *heure)
{
    Temps *h = malloc(sizeof(Temps));
    h->heure = atoi(strtok(heure, ":"));
    h->minute = atoi(strtok(NULL, ":"));
    return *h;
}

int inferieur(Temps t1, Temps t2)
{
    int t1min = t1.heure * 60 + t1.minute;
    int t2min = t2.heure * 60 + t2.minute;

    return t1min <= t2min;
}

int superieur(Temps t1, Temps t2)
{
    int t1min = t1.heure * 60 + t1.minute;
    int t2min = t2.heure * 60 + t2.minute;

    return t1min >= t2min;

}