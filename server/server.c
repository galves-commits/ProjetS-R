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
	char req[MAX];
	char protocole[MAX];
	char reste[MAX];
	char villeD[MAX];
	char villeA[MAX];
	char heure[MAX] ;
	int nblus = read(echange,req,MAX) ;
	int  nbtrains;
	
	sscanf(req,"%[^;\n];%[^;\n];%[^;\n];%[^;\n]" , protocole , villeD, villeA , heure) ;
	printf("%s",req);
	printf("'%d';'%s';'%s';'%s';'%s'\n" ,nblus, protocole , villeD, villeA , heure);
		if (strcmp(protocole , "TV") ==0 ) {
			Cellule * goodTrains = getTrains(protocole , villeD,villeA, "",trains ,&nbtrains) ;
			reponseRequete(goodTrains, echange,nbtrains);

		}else{ 
			if(strcmp(protocole , "TH" ) == 0 ) {
				Cellule * goodTrains = getTrains(protocole , villeD, villeA , heure,trains ,&nbtrains) ;
				reponseRequete(goodTrains, echange, nbtrains);

				
			}else{
				if(strcmp(protocole , "TB" ) == 0 ) {
					Cellule * goodTrains = getTrains(protocole , villeD, villeA , heure, trains,&nbtrains ) ;
					reponseRequete(goodTrains, echange, nbtrains);
				}
			}
		}
}



void reponseRequete(Cellule * trains, int echange,int nbtrains){
	char reponse[MAX];
	char nbt[MAX];
	printf("%d\n",nbtrains);
	sprintf(nbt, "%d",nbtrains);
	printf("%s\n",nbt);
	write(echange,nbt,sizeof(nbt));
	if(nbtrains=0){
		sprintf(reponse, "%d",nbtrains);
	}else{
		int i=1;
		while(i!=nbtrains){
			char chaine[MAX];

			Temps dureeTraj = duree(trains->leTrain);

			sprintf(chaine, "%d;%s;%s;%d:%d;%d:%d;%f;%s\n", 
			trains->leTrain.numero,trains->leTrain.villeDepart,trains->leTrain.villeArrivee,
			trains->leTrain.heureDep.heure,trains->leTrain.heureDep.minute,  
			trains->leTrain.heureArr.heure,trains->leTrain.heureArr.minute,
			trains->leTrain.prix,trains->leTrain.reduc);
			strcat(reponse,chaine);
			trains=trains->suivant;
			write(echange,reponse,sizeof(reponse));
			i++;
		}
	}
}
