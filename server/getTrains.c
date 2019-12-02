#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../headers/server.h"

Cellule * getTrains(char* depart, char* arrivee, char* heure, Cellule* trains){
    Temps hDebReq;
    Temps hFinReq;
    
    char* p = strtok(heure,';');
    hDebReq = stringToTemps(p);
    p = strtok(NULL,';');
    hFinReq = stringToTemps(p);
    

    int i = 0;
    while(trains!=NULL){

    }
}