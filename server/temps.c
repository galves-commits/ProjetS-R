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

Temps stringToTemps(char *heure)
{
    Temps h;
    h.heure = atoi(strtok(heure, ":"));
    h.minute = atoi(strtok(NULL, ":"));
    return h;
}

bool inferieur(Temps t1, Temps t2)
{
    if ((t1.heure <= t2.heure) && (t1.minute <= t2.minute))
    {
        return true;
    }else{
        return false;
    }
}

bool superieur(Temps t1, Temps t2)
{
    if ((t1.heure >= t2.heure) && (t1.minute >= t2.minute))
    {
        return true;
    }else{
        return false;
    }

}