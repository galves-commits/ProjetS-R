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
// fonction admin
void admin(){
	char reponse[10];
	printf(PURPLE"Accès Administration("YELLOW"A"PURPLE") ou Client("YELLOW"C"PURPLE") ?\n");// demande si on se connecte en Admin ou Client
	printf(YELLOW);
	fscanf( stdin, "%s", reponse);  //reponse stocke la reponse du terminal
	while(strcmp(reponse, "A")&&strcmp(reponse, "C")){ // si on se trompe
		printf("Veuillez rentrer un caractère correcte\n");
		printf(PURPLE"Accès Administration("YELLOW"A"PURPLE") ou Client("YELLOW"C"PURPLE") ?\n");
		printf(YELLOW);
		fscanf( stdin, "%s", reponse);
	}
	if (strcmp(reponse, "A") == 0)// si reponse est A alors demande de mdp
	{
		printf(PURPLE"Veuillez indiquer le mot de passe\n");
		printf(YELLOW);
		fscanf(stdin,"%s", reponse);
		while(strcmp(reponse, "admin")){// si on se trompe

			printf(RED"Mot de passe incorrect\n");
			printf(PURPLE"Veuillez indiquer le mot de passe\n");
			printf(YELLOW);
			fscanf(stdin,"%s", reponse);
		}
		if (strcmp(reponse, "admin") == 0){ // si mdp = admin alors creation CelluleT 
            CelluleT *req = malloc(sizeof(CelluleT));
			req=statServer(); // utilisation fct StatServer ( statistique.c)
			comparaison(req); // utilisation fct comparaison (satistique.c)
		}
        exit(EXIT_SUCCESS);
	}
}
// statServer permet de traiter stat.txt et donc de recup les requetes des clients
CelluleT * statServer(){
    char str[MaxStr];
    char *s = malloc(sizeof(char));
    char stat[MaxStr][NBMaxTrain];
    FILE *file = fopen("../server/stat.txt", "r");// ouverture du fichier stat.txt
    int i = 0;
    CelluleT *req = malloc(sizeof(CelluleT));
    if (file != NULL)
	{       

		while (fgets(str, NBMax, file) != NULL)// recuperation de ligne par ligne stocke dans str
		{
			s = strchr(str, '\n');
			if (s != 0)
			{
				*s = '\0';
			}
			strcpy(stat[i], str);// copie de str dans stat
			i++;
            
		}
		fclose(file);
		int a = 0;

		while (a < i)
		{
			int y = 1;
			char *tabTrain[MaxVille][MaxStr];
			char *p;
			p = strtok(stat[a], "-");// decompose la chaine par rapport a -
			tabTrain[a][0] = p;
			while (y < 100 && tabTrain[a][y - 1] != NULL)
			{
				p = strtok(NULL, "-"); 
				tabTrain[a][y] = p;
				y++;
			}
            req = remplirStat(tabTrain[a], req);// utilisation de remplirStat (satistique.c)
            
			a++;
           
		}
    }
    return req;
}
// remplirStat permet de remplir la liste chainée de Stat
CelluleT * remplirStat(char **tab, CelluleT *requete)
{
	Stat *s = malloc(sizeof(Stat));
	s->proto = tab[0];//tab[0] contient le type de protocole ( TV / TH / TB)
	s->villeDepart = tab[1];// tab[1] contient la ville de depart
	s->villeArrivee = tab[2];//tab[2] contient la ville d'arrivée
    if(( strcmp(s->proto,"TH")==0 )||(strcmp(s->proto,"TB")==0)){// on verifie le protocole 
	s->h1 = stringToTemps(tab[3]);//tab[3] contient la première heure entré que l'on transforme en structure temps
    }
    if(strcmp(s->proto,"TB")==0){
	s->h2 = stringToTemps(tab[4]);//tab[4] contient la deuxieme heure entré que l'on transforme en structure temps
    }
	inserTete2(&requete, *s);// insere struct Stat dans la liste chainé
	free(s);
	return requete;
}
//fonction permet d'inserer une structure Stat dans une liste chainée
void inserTete2(CelluleT ** tete, Stat t){
    CelluleT *nouv = malloc(sizeof(CelluleT));
    nouv->laStat=t;
    nouv->suivant = *tete;
    *tete = nouv;
}
// la fct comparaison renvoie le protocole le + utilisé , 
// le trajet le + demande et si les demandes de train sont du matin ou du soir (avant 12:00 -> matin)
void comparaison(CelluleT *stat)
{
	CelluleT *t = stat;
	int nbedeReq = 0 ; //nbe de req total
	int nbeTV = 0 ; // nbe de protocole TV
	int nbeTH = 0 ;// nbe de protocole TH
	int nbeTB = 0 ;// nbe de protocole TB
	int nbeVG = 0 ;// nbe de trajet Valence-->Grenoble
	int nbeVM = 0 ;// nbe de trajet Valence-->Montelimar
	int nbeVP = 0 ;// nbe de trajet Valence-->Paris
	int nbeGV = 0 ;// nbe de trajet Grenoble-->Valence
	int nbeMV = 0 ;// nbe de trajet Montelimar-->Valence
	int nbePV = 0 ;// nbe de trajet Paris-->Valence
	Temps treq; //Temps treq = 12:00 (pour comparer avec les horraires des requetes)
	treq.heure=12; 
	treq.minute=00;
	int matin=0; // nbe de demande de train avant treq ( avant midi)
	int soir=0; // nbe de demande de train après treq ( apres midi)
	while (t->suivant!=NULL) // parcourir toute la liste chaine jusqu'a la fin
	{
        if(strcmp(t->laStat.proto,"TV")==0){
            nbeTV ++ ;	// si  le protocole TV est utilisé alors nbeTV +1
        }
        if(strcmp(t->laStat.proto,"TH")==0){
            nbeTH ++ ; // si protocole TH est utilisé alors nbeTH +1
			if (superieur(t->laStat.h1, treq)){
                 soir++; // si l'heure de la demande est superieur a 12:00 soir++ sinon matin++
            }else{
                matin++;
            }
		}
        if(strcmp(t->laStat.proto,"TB")==0){
			nbeTB ++ ; // si protocole TB est utilisé alors nbeTB +1
			 if (superieur(t->laStat.h1, treq)){
                 soir++; // si la première heure de la demande est superieur a 12:00 soir++ sinon matin++
            }else{
                matin++;
            }
            if (superieur(t->laStat.h2, treq)){
                 soir++; // si la seconde heure de la demande est superieur a 12:00 soir++ sinon matin++
            }else{
                matin++;
            }	
        }
		if(strcmp(t->laStat.villeDepart,"Valence")==0){
			if(strcmp(t->laStat.villeArrivee,"Grenoble")==0){
            	nbeVG ++ ;  // si trajet Valence-->Grenoble nbeVG++
			}
			if(strcmp(t->laStat.villeArrivee,"Montelimar")==0){
            	nbeVM ++ ; // si trajet Valence-->Montelimar nbeVM++
        	}
			if(strcmp(t->laStat.villeArrivee,"Paris Gare de Lyon")==0){
            	nbeVP ++ ; // si trajet Valence-->Paris nbeVP++
        	} 
        }
		if(strcmp(t->laStat.villeDepart,"Grenoble")==0){
            if(strcmp(t->laStat.villeArrivee,"Valence")==0){
            	nbeGV ++ ; // si trajet Grenoble-->Valence nbeGV++
			}
        }
		if(strcmp(t->laStat.villeDepart,"Montelimar")==0){
            if(strcmp(t->laStat.villeArrivee,"Valence")==0){
            	nbeMV ++ ; // si trajet Montelimar-->Valence nbeMV++
			}
        }
		if(strcmp(t->laStat.villeDepart,"Paris Gare de Lyon")==0){
            if(strcmp(t->laStat.villeArrivee,"Valence")==0){
            	nbePV ++ ; // si trajet Paris-->Valence nbePV++
			}
        }       
		nbedeReq ++ ; // on incremente de 1 le compte de requete
        t = t->suivant; // on avance dans la liste chainé
	}
	int tr=max(max(max(nbeVG , nbeVM), max(nbeVP, nbeGV)),max(nbeMV,nbePV));// on regarde le trajet avec le plus de demande et on stock dans tr
	int m =max(max(nbeTB , nbeTH) , nbeTV);// on regarde le protocole avec le + de demande et stock dans m
	int h=max(matin,soir);// on regarde horraire avec le + de demande 
	double moyP= moyenne(m, nbedeReq); // moyenne de m
	double moyT= moyenne(tr, nbedeReq);// moyenne de tr
	char a='%';
		printf(GREEN"Nous avons reçus aujourd'hui %d requêtes\n",nbedeReq);
		if (m == nbeTV) {
			printf(GREEN"Le protocole TV est le plus utilisé avec %d requete soit une moyenne de %0.3f %c \n" , nbeTV , moyP,a) ;
		}
		if (m == nbeTH) {
			printf(GREEN"Le protocole TH est le plus utilisé avec %d requete soit une moyenne de %0.3f %c \n" , nbeTH , moyP,a);
		}
		if (m == nbeTB) {
			printf(GREEN"Le protocole TB est le plus utilisé avec %d requete soit une moyenne de %0.3f %c \n" , nbeTB , moyP,a) ;
		}
		if (tr == nbeVG) {
			printf(GREEN"Le trajet le plus recherché est Valence --> Grenoble avec %d requete soit une moyenne de %0.3f %c \n" , tr ,  moyT,a) ;
		}
		if (tr == nbeVM) {
			printf(GREEN"Le trajet le plus recherché est Valence --> Montelimar avec %d requete soit une moyenne de %0.3f %c \n" , tr ,  moyT,a) ;
		}
		if (tr == nbeVP) {
			printf(GREEN"Le trajet le plus recherché est Valence --> Paris Gare de Lyon avec %d requete soit une moyenne de %0.3f %c \n" , tr ,  moyT,a) ;
		}
		if (tr == nbeGV) {
			printf(GREEN"Le trajet le plus recherché est Grenoble --> Valence avec %d requete soit une moyenne de %0.3f %c \n" , tr ,  moyT,a) ;
		}
		if (tr == nbeMV) {
			printf(GREEN"Le trajet le plus recherché est Montelimar --> Valence avec %d requete soit une moyenne de %0.3f %c \n" , tr ,  moyT,a) ;
		}
		if (tr == nbePV) {
			printf(GREEN"Le trajet le plus recherché est Paris Gare de Lyon --> Valence avec %d requete soit une moyenne de %0.3f %c \n" , tr ,  moyT,a) ;
		}
		if (matin==soir){
			printf(YELLOW"Il y a autant de requêtes le matin que l'après-midi\n");
		}else{
			if (h==matin){
				printf(YELLOW"La majorité des requêtes sont faites le matin\n");
			}else{
				printf(YELLOW"La majorité des requêtes sont faites l'après-midi\n");
		}
	}
}


double moyenne(int a , int b ) { //moyenne en double de 2 int
		double c =  (double)a / (double) b  ; 
		return c*100 ;
}

int max(int a , int b) { // max de 2 int
	int max = a;
 	if (b>a){
		max=b;
	}
	return max ;
}