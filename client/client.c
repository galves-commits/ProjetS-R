#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX 40000


int main(int argc,char *argv[],char *arge[]){
	int connection = socket (AF_INET , SOCK_STREAM , 0  ) ;
	struct sockaddr_in sin ;
	sin.sin_family= AF_INET ;
	sin.sin_port = htons(7777) ;
	struct hostent * host = gethostbyname("localhost");
	memcpy(&sin.sin_addr.s_addr, host->h_addr_list[0], sizeof(sin.sin_addr.s_addr) ) ;
	int echange ;
	echange = connect(connection , (struct sockaddr*) &sin , sizeof(sin) ) ;
	printf("Je suis connecté\n" ) ; 

	char hor[MAX];
	char reponse[MAX];
	char req[MAX];
	char dep[MAX];
	char arr[MAX];
	printf("Recherche avec ville seul(TV), avec une horraire (TH), avec deux horraires(TB) ? ");
	fscanf(stdin,"%s",req);
	if(strcmp(req,"TV")==0){
		printf("Ville de départ : ");
		fscanf(stdin,"%s;",dep);
		printf("Ville d'arrivé : ");
		fscanf(stdin,"%s",arr);
		sprintf(reponse, "%s;%s\n", dep,arr);
	}
	if(strcmp(req,"TH")==0){
		printf("Ville de départ : ");
		fscanf(stdin,"%s;",dep);
		printf("Ville d'arrivé : ");
		fscanf(stdin,"%s",arr);
		printf("Horraire : ");
		fscanf(stdin,"%s",hor);
		sprintf(reponse, "%s;%s;%s\n", dep,arr,hor);
	}
	if(strcmp(req,"TB")==0){
		printf("Ville de départ : ");
		fscanf(stdin,"%s;",dep);
		printf("Ville d'arrivé : ");
		fscanf(stdin,"%s",arr);
		printf("Horraire : ");
		fscanf(stdin,"%s",hor);
		sprintf(reponse, "%s;%s;%s\n", dep,arr,hor);
	}
	write(connection,reponse , strlen(reponse)+1) ;
}
