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

#define MAX 40000

int main(int argc,char *argv[],char *arge[]){
	Cellule *trains = traiterFic();
	printf("Runing\n");

	int ecoute = socket (AF_INET , SOCK_STREAM , 0  ) ;
	struct sockaddr_in sin ;
	sin.sin_family= AF_INET ;
	sin.sin_port = htons(7777) ;
	sin.sin_addr.s_addr = htonl(INADDR_ANY ) ;
	bind(ecoute , (struct sockaddr*) &sin , sizeof(sin) ) ;
	listen(ecoute, 5 );

	while(true){
		int echange = accept (ecoute ,NULL,NULL);
		pid_t pid = fork();
		switch(pid){
			case 0:	printf("Un client s'est connecté\n" ) ;
					echangeClient(echange,trains);
					break;

			case -1:perror("fork");
					exit(EXIT_FAILURE);
					break;
		}
	} 
}

void echangeClient(int echange, Cellule* trains){
	requeteTrajet(echange,trains);
}

void requeteTrajet(int echange,Cellule* trains){
	char villeD[MAX];
	char villeA[MAX];
	char heure[MAX];

	char* message = "Ville de départ : " ;
	write(echange,message , strlen(message)+1);
	int nbLus= read(echange,villeD,MAX) ;
	message = "Ville d'arrive : ";
	write(echange, message , strlen(message)+1);
	nbLus= read(echange,villeA,MAX) ;
	message = "Horraires : ";
	write(echange,message , strlen(message)+1);
	nbLus= read(echange,heure,MAX) ;

	Cellule * goodTrains = getTrains("TV",villeD, villeA, heure, trains);

	reponseRequete(goodTrains, echange);
}

void reponseRequete(Cellule * trains, int echange){
	char reponse[MAX];
	if(trains->suivant==NULL){
		sprintf(reponse, "Aucun trains ne reponds a votre recherche\n");
		printf("test");
	}else{
		sprintf(reponse,  "Trains disponibles : \n  N \t\tDepart \t\t   Arrivee\t\tHeure D \t Heure A \t  Prix \t\tREDUC \t\n");

		while(trains->suivant!=NULL){
			char chaine[MAX];

			Temps dureeTraj = duree(trains->leTrain);


			sprintf(chaine, "%d %20s  %20s \t %02d:%02d \t\t  %02d:%02d \t %3.2f€ \t %s\n", 
			trains->leTrain.numero,trains->leTrain.villeDepart,trains->leTrain.villeArrivee,
			trains->leTrain.heureDep.heure,trains->leTrain.heureDep.minute,  
			trains->leTrain.heureArr.heure,trains->leTrain.heureArr.minute,
			trains->leTrain.prix,trains->leTrain.reduc);
			strcat(reponse,chaine);
			trains=trains->suivant;
		}
	}
	write(echange,reponse,sizeof(reponse));
}
