CC=gcc
CFLAGS=-I.

make: server/server.o server/temps.o server/req.o server/getTrains.o server/cellule.o common/statistique.o common/dialogue.o  cLient/client.o
	gcc -o server/server server/server.o server/temps.o server/req.o server/getTrains.o server/cellule.o common/dialogue.o common/statistique.o -I.
		gcc -o cLient/client cLient/client.o server/temps.o server/cellule.o server/getTrains.o common/dialogue.o common/statistique.o -I.

	
	