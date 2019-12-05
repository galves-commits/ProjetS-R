CC=gcc
CFLAGS=-I.

clientmake: client.o ../server/temps.o ../server/cellule.o ../server/getTrains.o ../common/dialogue.o
	gcc -o client client.o ../server/temps.o ../server/cellule.o ../server/getTrains.o ../common/dialogue.o -I.
	