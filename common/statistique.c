#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define MaxStr 100
#define NBMax 1000
#define NBMaxTrain 100
#define MaxVille 100

#include "../headers/trains.h"
#include "../headers/server.h"
#include "../headers/dialogue.h"

void admin(){
	char reponse[10];
	printf("Accès Administration(A) ou Client(C) ?\n");
	fscanf(stdin, "%s", reponse);
	if (strcmp(reponse, "A") == 0)
	{
		printf("Veuillez indiquer le mot de passe\n");
		fscanf(stdin, "%s", reponse);
		if (strcmp(reponse, "admin") == 0){
			statServer();
		}	
	}
}
void statServer(){

}