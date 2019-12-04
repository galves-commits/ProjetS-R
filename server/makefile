CC=gcc
CFLAGS=-I.

servermake: server.o temps.o req.o getTrains.o cellule.o ../common/dialogue.o
	gcc -o server server.o temps.o req.o getTrains.o cellule.o ../common/dialogue.o -I.