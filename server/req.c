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

#include "../headers/server.h"

#define NBMaxTrain 100
#define NBMax 1000
#define MaxStr 100
#define MaxVille 100
 
		 
		 
int main ()
{
	traiterFic();
}

   
   
void traiterFic ( ) {
	
	char ligneActuel [NBMaxTrain][NBMax];
	char * s;
	int i = 0 ;
	//char *tab[NBMax];
	char str[MaxStr] ;
	FILE *file = fopen ( "train.txt", "r" );
	if ( file != NULL ){ 
		while(fgets(str , NBMax , file) != NULL ) {
			s = strchr(str,'\n');
			if(s!=0){
				*s = '\0';
			}
			strcpy(ligneActuel[i],str);
            i++;
		}
		fclose ( file );
		
		int a =0;
		
		while(a<i){	
			int y=1 ;
			char* tabTrain [MaxVille][MaxStr];
			char *p;
			p = strtok(ligneActuel[a],  ";") ;
			tabTrain[a][0] = p;
			while(y < 100 && tabTrain[a][y-1] != NULL ) {
				p = strtok(NULL, ";" ) ;
				tabTrain[a][y] = p;
				y++;
			}
			a++;
		
		}
	}
 }
 
 void remplirStruc(char[NBMax][NBMax]) {
 }
 
 

		
  



