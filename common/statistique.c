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
#include "../headers/statistique.h"

void admin(){
	char reponse[10];
	printf("Accès Administration(A) ou Client(C) ?\n");
	fscanf(stdin, "%s", reponse);
	if (strcmp(reponse, "A") == 0)
	{
		printf("Veuillez indiquer le mot de passe\n");
		fscanf(stdin, "%s", reponse);
		if (strcmp(reponse, "admin") == 0){
             CelluleT *req = malloc(sizeof(CelluleT));
			req=statServer();
			comparaison(req);
		}
        exit(EXIT_SUCCESS);
	}
}

CelluleT * statServer(){
    char str[MaxStr];
    char *s = malloc(sizeof(char));
    char stat[MaxStr][NBMaxTrain];
    FILE *file = fopen("../server/stat.txt", "r");
    int i = 0;
    CelluleT *req = malloc(sizeof(CelluleT));
    if (file != NULL)
	{       

		while (fgets(str, NBMax, file) != NULL)
		{
			s = strchr(str, '\n');
			if (s != 0)
			{
				*s = '\0';
			}
			strcpy(stat[i], str);
			i++;
            
		}
		fclose(file);
		int a = 0;

		while (a < i)
		{
			int y = 1;
			char *tabTrain[MaxVille][MaxStr];
			char *p;
			p = strtok(stat[a], "-");
			tabTrain[a][0] = p;
			while (y < 100 && tabTrain[a][y - 1] != NULL)
			{
				p = strtok(NULL, "-"); 
				tabTrain[a][y] = p;
				y++;
			}
            req = remplirStat(tabTrain[a], req);
            
			a++;
           
		}
    }
    return req;
}

CelluleT * remplirStat(char **tab, CelluleT *requete)
{
	Stat *s = malloc(sizeof(Stat));
	s->proto = tab[0];
	s->villeDepart = tab[1];
	s->villeArrivee = tab[2];
    if(( strcmp(s->proto,"TH")==0 )||(strcmp(s->proto,"TB")==0)){
	s->h1 = stringToTemps(tab[3]);
    }
    if(strcmp(s->proto,"TB")==0){
	s->h2 = stringToTemps(tab[4]);
    }
	inserTete2(&requete, *s);
	free(s);
	return requete;
}

void inserTete2(CelluleT ** tete, Stat t){
    CelluleT *nouv = malloc(sizeof(CelluleT));
    nouv->laStat=t;
    nouv->suivant = *tete;
    *tete = nouv;
}

void comparaison(CelluleT *stat)
{
	CelluleT *t = stat;
	int nbedeReq = 0 ;
	int nbeTV = 0 ;
	int nbeTH = 0 ;
	int nbeTB = 0 ;
	int nbeVG = 0 ;
	int nbeVM = 0 ;
	int nbeVP = 0 ;
	int nbeGV = 0 ;
	int nbeMV = 0 ;
	int nbePV = 0 ;
	Temps treq;
	treq.heure=12;
	treq.minute=00;
	int matin=0;
	int soir=0;
	while (t->suivant!=NULL)
	{
        if(strcmp(t->laStat.proto,"TV")==0){
            nbeTV ++ ;
        }
        if(strcmp(t->laStat.proto,"TH")==0){
            nbeTH ++ ;
			if (superieur(t->laStat.h1, treq)){
                 soir++;
            }else{
                matin++;
            }
		}
        if(strcmp(t->laStat.proto,"TB")==0){
			nbeTB ++ ;
			 if (superieur(t->laStat.h1, treq)){
                 soir++;
            }else{
                matin++;
            }
            if (superieur(t->laStat.h2, treq)){
                 soir++;
            }else{
                matin++;
            }	
        }
		if(strcmp(t->laStat.villeDepart,"Valence")==0){
			if(strcmp(t->laStat.villeArrivee,"Grenoble")==0){
            	nbeVG ++ ;
			}
			if(strcmp(t->laStat.villeArrivee,"Montelimar")==0){
            	nbeVM ++ ;
        	}
			if(strcmp(t->laStat.villeArrivee,"Paris Gare de Lyon")==0){
            	nbeVP ++ ;
        	} 
        }
		if(strcmp(t->laStat.villeDepart,"Grenoble")==0){
            if(strcmp(t->laStat.villeArrivee,"Valence")==0){
            	nbeGV ++ ;
			}
        }
		if(strcmp(t->laStat.villeDepart,"Montelimar")==0){
            if(strcmp(t->laStat.villeArrivee,"Valence")==0){
            	nbeMV ++ ;
			}
        }
		if(strcmp(t->laStat.villeDepart,"Paris Gare de Lyon")==0){
            if(strcmp(t->laStat.villeArrivee,"Valence")==0){
            	nbePV ++ ;
			}
        }       
		nbedeReq ++ ;
        t = t->suivant;
	}
	int tr=max(max(max(nbeVG , nbeVM), max(nbeVP, nbeGV)),max(nbeMV,nbePV));
	int m =max(max(nbeTB , nbeTH) , nbeTV);
	int h=max(matin,soir);
	double moyP= moyenne(m, nbedeReq);
	double moyT= moyenne(tr, nbedeReq);
	char a='%';
		printf("Nous avons reçus aujourd'hui %d requêtes\n",nbedeReq);
		if (m == nbeTV) {
			printf("Le protocole TV est le plus utilisé avec %d requete soit une moyenne de %0.3f %c \n" , nbeTV , moyP,a) ;
		}
		if (m == nbeTH) {
			printf("Le protocole TH est le plus utilisé avec %d requete soit une moyenne de %0.3f %c \n" , nbeTH , moyP,a);
		}
		if (m == nbeTB) {
			printf("Le protocole TB est le plus utilisé avec %d requete soit une moyenne de %0.3f %c \n" , nbeTB , moyP,a) ;
		}
		if (tr == nbeVG) {
			printf("Le trajet le plus recherché est Valence --> Grenoble avec %d requete soit une moyenne de %0.3f %c \n" , nbeVG ,  moyT,a) ;
		}
		if (tr == nbeVM) {
			printf("Le trajet le plus recherché est Valence --> Montelimar avec %d requete soit une moyenne de %0.3f %c \n" , nbeVG ,  moyT,a) ;
		}
		if (tr == nbeVP) {
			printf("Le trajet le plus recherché est Valence --> Paris Gare de Lyon avec %d requete soit une moyenne de %0.3f %c \n" , nbeVG ,  moyT,a) ;
		}
		if (tr == nbeGV) {
			printf("Le trajet le plus recherché est Grenoble --> Valence avec %d requete soit une moyenne de %0.3f %c \n" , nbeVG ,  moyT,a) ;
		}
		if (tr == nbeMV) {
			printf("Le trajet le plus recherché est Montelimar --> Valence avec %d requete soit une moyenne de %0.3f %c \n" , nbeVG ,  moyT,a) ;
		}
		if (tr == nbePV) {
			printf("Le trajet le plus recherché est Paris Gare de Lyon --> Valence avec %d requete soit une moyenne de %0.3f %c \n" , nbeVG ,  moyT,a) ;
		}
		if (matin==soir){
			printf("Il y a autant de requêtes le matin que l'après-midi\n");
		}else{
			if (h==matin){
				printf("La majorité des requêtes sont faites le matin\n");
			}else{
				printf("La majorité des requêtes sont faites l'après-midi\n");
		}
	}
}


double moyenne(int a , int b ) {
		double c =  (double)a / (double) b  ; 
		return c*100 ;
}

int max(int a , int b) {
	int max = a;
 	if (b>a){
		max=b;
	}
	return max ;
}