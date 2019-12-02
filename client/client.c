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
	printf(" Je suis connecté\n" ) ; 
	char* message = "coucou" ;
	write(connection,message , strlen(message)+1) ;
}
