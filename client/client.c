#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../headers/trains.h"
#include "../headers/dialogue.h"



#define MAX 400

Cellule *recupTrain(int, int);

int main(int argc, char *argv[], char *arge[])
{	//serv1
	int connection = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(7777);
	struct hostent *host = gethostbyname("localhost");
	memcpy(&sin.sin_addr.s_addr, host->h_addr_list[0], sizeof(sin.sin_addr.s_addr));
	int echange;
	echange = connect(connection, (struct sockaddr *)&sin, sizeof(sin));
	//serv2
	int connection2 = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin2;
	sin2.sin_family = AF_INET;
	sin2.sin_port = htons(7778);
	struct hostent *host2 = gethostbyname("localhost");
	memcpy(&sin2.sin_addr.s_addr, host2->h_addr_list[0], sizeof(sin2.sin_addr.s_addr));
	int echange2;
	echange2 = connect(connection2, (struct sockaddr *)&sin2, sizeof(sin2));

	printf("Je suis connecté\n");
	char ans[MAX];
	int nbLus;
	printf("Connexion a un(1) ou plusieurs serveur(2)?\n");
	fscanf(stdin, "%s", ans);
	if (strcmp(ans, "1") == 0)
			{
		sendRequete(connection);
		getRequete(connection);
			}else{	
	sendRequete2(connection,connection2);
	getRequete2(connection,connection2);
	
	
		
	
	
			}
}



