CC=gcc
CFLAGS=-I.

make: server/server.o server/temps.o server/req.o server/getTrains.o server/cellule.o common/dialogue.o server/server2.o cLient/client.o
	gcc -o server/server server/server.o server/temps.o server/req.o server/getTrains.o server/cellule.o common/dialogue.o -I.
	gcc -o server/server2 server/server2.o server/temps.o server/req.o server/getTrains.o server/cellule.o common/dialogue.o -I.
	gcc -o cLient/client cLient/client.o server/temps.o server/cellule.o server/getTrains.o common/dialogue.o -I.

	
	